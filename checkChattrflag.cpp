
#include <iostream> // for cin, cout
#include <fcntl.h>  // for file contol options -> O_RDONLY, O_NONBLOCK 
#include <sys/ioctl.h> //for input/output control fuction
#include <linux/fs.h> // for linux-specific filesystem flags (FS_IMMUTABLE_FL)
#include <unistd.h> // for posix system calls (open,close)
#include <errno.h> //for system error numbers
#include <string.h>		    // for C-style string manipulation

//check if immutable (+i) flag set on pathP
//if yes -> return true (+i) is set , if not return false
bool isImmutable(const char* path)
{
	//O_NONBLOCK prevents the process from hanging if the path is a special file or FIFO.
	int fd = open(path,O_RDONLY | O_NONBLOCK);
	if(fd<0) return false; // Return false if file not be opend i.e. file not exist.
			       
			       
	int flags;
	    // FS_IOC_GETFLAGS: Interrogates the filesystem to get current attributes of the file
	if(ioctl(fd,FS_IOC_GETFLAGS,&flags) < 0)
	{
		close(fd); // always close file descriptor to avoid leaks.
		return false;
	}

	close(fd);
	// Use bitwise AND to check if the immutable bit (FS_IMMUTABLE_FL) is set to 1 
	
	return (flags & FS_IMMUTABLE_FL);
}


//set or removes immutable flag.
//enable=true (+1) , enable=flase (-i)

void setImmutable(const char* path, bool enable)
{
	int fd=open(path,O_RDONLY | O_NONBLOCK );
	if(fd<0) return;

	int flags;
	// step 1 : get current flag so we don't overwrite other attributes like +a or +d
	if(ioctl(fd,FS_IOC_GETFLAGS,&flags)<0)
	{
		close(fd);
		return ;
	}

	if(enable)
		flags |= FS_IMMUTABLE_FL; // bitwise OR to turn the flags ON
	else
		flags &= ~FS_IMMUTABLE_FL; // bitwise AND with NOT to turn flag OFF
					   
	//step 2 : Push modification flag back to filesystem
	ioctl(fd, FS_IOC_SETFLAGS,&flags);

	close(fd);


}
int main()
{
	const char* pathP = "/rushi/chrome/policies";
	const char* pathM = "/rushi/chrome/policies/managed";

	bool p=false;
	bool m=false;

//---------------  check and make mutable -----------------//

	if(isImmutable(pathP))
	{
		p =true; //remember that it was locked
		setImmutable(pathP,false); //Unlock it (chatter -i)
		std::cout<<"[DEBUG] Path P was Immutable. Made mutable \n";

	}

	if(isImmutable(pathM))
	{
		m=true;//rember that it was locked 
		setImmutable(pathM,false);// Unlock it  (chattr -i)
		std::cout<<"[DEBUG] Path M was Immutable. Made mutable\n";
	}

	//----------Critical Zone ------------//
	// At this point, the folders are unlocked.
	// you can now write your JSON policy file safely here.
	//
	//------------- Restore original flags-----------
	
	//If P was originally locked, kock it again (chattr +i)
	
	if(p)
	{
		setImmutable(pathP,true);
		std::cout<<"[DEBUG] Restored +i on Path P\n";

	}

	//If M was originally locked, lock it again (chattr +i)
	
	if(m)
	{
		setImmutable(pathM,true);
		std::cout<<"[DEBUG] Restored +i on Path M\n";
	}

	return 0;
}
