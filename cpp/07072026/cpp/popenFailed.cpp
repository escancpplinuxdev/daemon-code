
#include <iostream>
#include <cstring>

int main()
{
	
	char cmd [200] = "";
//	snprintf(cmd,sizeof(cmd),"%s","dpkg -S /usr/share/applications/qwerty.desktop  2>/dev/null");
//	snprintf(cmd,sizeof(cmd),"%s","dpkg --search /usr/share/applications/qwerty.desktop  2>/dev/null");
//	snprintf(cmd,sizeof(cmd),"%s","which ls 2>/dev/null");
	snprintf(cmd,sizeof(cmd),"%s","dpkg --search /usr/share/applications/vlc.desktop  2>/dev/null");

	FILE *fp = popen(cmd,"r");
	
	if(fp != NULL)
	{
		char buffer[256] = "";
		std::string output;
		if ( fgets(buffer,sizeof(buffer),fp) != NULL)
		{

			int status = pclose(fp);
			bool commandSucceeded = (status != -1 && WIFEXITED(status) && WEXITSTATUS(status) == 0);
			
			if(commandSucceeded)
			{
				std::cout<<"command pass \n";
			}
			else
			{
				std::cout<<"command failed if exit code '"<<WIFEXITED(status)<<"'. File not found or dpkg error \n";
			}

/*			for(int i = 0; i< sizeof(buffer);++i)
			{
				if (buffer[i] == '\n')
				buffer[i] = '\0';
			}
*/


			output = buffer;
			size_t pos = output.find(":");
			if(pos == std::string::npos)
			{
				std::cout<<"not found\n";
				goto NOVERSION;
			}



			std::cout<<"sizeof(buffer) = '"<<sizeof(buffer)<<"'\n";
			std::cout<<"strlen(buffer) = '"<<strlen(buffer)<<"'\n";
			buffer[strlen(buffer) - 1] = '\0';
			std::cout<<"buffer = '"<<buffer<<"'\n";
			char * token = strtok(buffer,":");
			if(token == NULL)
			{
				std::cout<<"token == NULL\n";
			}
			else
			{
				std::cout<<"token = '"<<token<<"'\n";
			}

		}
		else
		{
			NOVERSION :
			std::cout<<"No o/p form command. Empty buffer\n";
			
		}
	}
	else
	{
		std::cerr<<"Command failed"<<strerror(errno)<<"\n";
	}

	return 0;
}
