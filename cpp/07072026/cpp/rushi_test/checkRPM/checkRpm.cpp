
#include <iostream>
#include <unistd.h>

int checkRPM()
{
	if((access("/usr/bin/rpm", F_OK) == 0) || ( access("/bin/rpm", F_OK) == 0))
	{
		if((access("/usr/bin/rpm", X_OK) == 0) || ( access("/bin/rpm", X_OK) == 0))	
		{
			return 1;
		}
		
			return 0;
	}
			return 0;
}

int main()
{
	if(checkRPM())
	{
		std::cout<<"This is rpm m/c\n";
		
	}
	else
	{
		std::cout<<"This is not m/c\n";
	}
	return 0;
}

/*


*/
