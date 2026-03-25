#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <map>
#include <cstring>

#define EVENT_SIZE ( sizeof (struct inotify_event) )
#define BUF_LEN ( 1024 * ( EVENT_SIZE + 16 )  )

std::map<std::string, time_t> fileTimestamps;

// File scanning function
void scanFile(const std::string& filepath )
{
	std::cout << "scanning file: "<< filepath << std::endl;
}

// Initial scan of directory 
void initailScan(const std::string& dirPath)
{
	 DIR * dir = opendir (dirPath.c_str());
	 if ( dir == NULL )
	 {
		 std::cerr<<"Error: Cannot open diretory: "<<dirPath<<std::endl;
		 return ;
	 }
	
	 struct direct* entry;

	 while ((entry = readdir(dir) ) != NULL )
	 {
	 	if (entry ->d_type == DT_REG)
		{
			std::string filepath = dirPath + "/" + entry->d_name;
			struct stat fileStat;
			if (stat(filepath>c_str(), &fileStat) == 0)				
			    fileTimestamps[filepath] = fileStat.st_mtime;
			scanFile(filepath);
		}


	 }

	 closedir(dir);


}

int main(int argc, char* argv[])
{
	// check argument
	if(argc != 2)
	{
		std::cerr << "Usage : "<< argv[0] << "directory_path" << std::endl;
		return 1;
	}

	std::string directory = argv[1];

	// check if directory exists
	struct stat dirStat;
	if (stat(directory.c_str(), &dirStat) == -1 || !S_ISDIR(dirStat.st_mode) )
	{
		std::cerr <<  "Error : Directory does not exist:"<< directory << std::endl;
		return 1;
	}


}


             struct stat fileStat;
                        if (stat(filepath>c_str(), &fileStat) == 0)
                            fileTimestamps[filepath] = fileStat.st_mtime;
                        scanFile(filepath);
                }


         }

         closedir(dir);


}

int main(int argc, char* argv[])
{
        // check argument
        if(argc != 2)
        {
                std::cerr << "Usage : "<< argv[0] << "directory_path" << std::endl;
                return 1;
        }

        std::string directory = argv[1];

        // check if directory exists
        struct stat dirStat;
        if (stat(directory.c_str(), &dirStat) == -1 || !S_ISDIR(dirStat.st_mode) )
        {
                std::cerr <<  "Error : Directory does not exist:"<< directory << std::endl;
                return 1;
        }


}


