#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <cstring>
#include <sys/stat.h>


#define EVENT_SIZE ( sizeof (struct inotify_event))
#define BUF_LEN (1024 * ( EVENT_SIZE + 16) )

void processFile (const std::string& filePath)
{
	std::cout << "Scannig file :   "<< filePath<<std::endl;

	/**Add scanning logic here 
	 * example : open file, read content , analyze ,etc.
	 */

}

int main(int argc, char* argv[])
{
	if(argc !=2 )
	{
		std::cout << "Usage :"<< argv[0]<<"<directory_path\n>";
		return 1;
	}

	const char* directory = argv[1];

	struct stat sb;
	// check if directory exists
	if(stat(directory, &sb) == -1 || !S_ISDIR(sb.st_mode))
	{
		std::cerr<< "Error : Directory does not exist!\n";
		return 1;
	}

	int fd= inotify_init();
	if(fd<0)
	{
		perror("inotify_init");
		return 1;
	}

	int wd = inotify_add_watch(fd, directory, IN_CREATE | IN_MODIFY | IN_DELETE);
	if(wd== -1)
	{
		perror ("inotify_add_watch");
		return 1;
	}

	char buffer[BUF_LEN];

// first scanning




	std::cout<<"Monitoring directory :" << directory <<std::endl;

	while(true)
	{
		int length = read (fd,buffer,BUF_LEN);

		if(length < 0)
		{
			perror("read");
			return 1;
		}

	int i=0;
	while (i< length)
	{
		struct inotify_event *event = (struct inotify_event*) &buffer[i];

		if(event->len)
		{
			std::string filePath = std::string(directory) + "/" + event->name;
				processFile(filePath);
			if(event->mask & IN_CREATE)
			{
				std::cout<<"File created : 	"<<event->name << std::endl;
				processFile(filePath);
			}
			else if( event->mask & IN_MODIFY)
			{
				std::cout<<"File modified:	"<<event->name<<std::endl;	
				processFile(filePath);
			}
			else if (event->mask &IN_DELETE)
			{
				std::cout<<"File deleted :w	"<<event->name<<std::endl;
			}

		}

		i+= EVENT_SIZE +event->len;


	}

		}
inotify_rm_watch(fd,wd);
close(fd);
return 0;

}
