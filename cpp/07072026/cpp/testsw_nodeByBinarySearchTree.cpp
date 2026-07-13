
#include <iostream>  // cout endl 
#include <cstdio>  // fopen fclose fgtes
#include <cstring>	// strcpy strlen strcmp
int main()
{
	std::cout<<"start program\n"<<std::endl;

	const char * temp = "/tmpdata/cpp/07072026/cpp/softwares.list";
	FILE *fp = fopen(temp,"r");
	if(fp != NULL)
	{
		char buffer[512] = "";
		char AppName[56] = "";
		int enterAppList = 0;	
		int endAppList = 0;	
		char Version[50] = "";
		char Filename[256] = "";
		char PkgName[59] = "";
		while( fgets(buffer,sizeof(buffer),fp) )
		{
			size_t len = strlen(buffer);
			if ( len > 0 && buffer[len-1] == '\n' )
			{
				buffer[len - 1] = '\0';
			}

			if(buffer[0] == '\n')
			{
				continue;
			}

			char * token = strtok(buffer,"|");
			if(token != NULL)
			{
				strcpy(AppName,token);
				token=strtok(NULL,"|");
				if(token != NULL)
				{
					strcpy(Version,token);	
					token=strtok(NULL,"|");
					if(token != NULL)
					{
						strcpy(Filename,token);
						std::cout<<AppName<<"|"<<Version<<"|"<<Filename<<"\n";
					}
					else
					{
						std::cout<<AppName<<"|"<<Version<<"|"<<"Filename"<<"\n";
					}

				}

			}


			//			if ( ((strcmp(buffer,"[BlackList]") == 0) || enterAppList == 1)   )
			if ( ((strcmp(buffer,"[BlackList]") == 0) || enterAppList == 1) && (endAppList == 0)   )
			{
				//				std::cout<<buffer;
				enterAppList = 1;
				if (strcmp(buffer,"TotalPackages") == 0)
				{
					endAppList = 1;
				}
				char *tok = strtok(buffer," =");
				if(tok != NULL)
				{
					strcmp(PkgName,tok);
					//					std::cout<<tok<<"\n";
					tok = strtok(NULL," =");
					if(tok!= NULL)
					{
						strcmp(PkgName,tok);
						std::cout<<tok<<"\n";

					}
				}
				else
				{
					std::cout<<"tok failed\n";
				}
			}
			else
			{
				std::cout<<"BlackList failed\n";
			}

		}
		int status = fclose(fp);
		std::cout<<"status = '"<<status<<"'\n";
		bool commandSucceeded = (status != -1 && WIFEXITED(status) && (WEXITSTATUS(status) == 0 ) );
		std::cout<<"commandSucceeded = '"<<commandSucceeded<<"'  WIFEXITED(status) = '"<<WIFEXITED(status)<<"'   (WEXITSTATUS(status)) = '"<< WEXITSTATUS(status)<<"'\n";

	}
	else
	{
		std::cerr<<"Error : '"<<errno<<"' = '"<<strerror(errno)<<"'\n";
	}
	return 0;
}
