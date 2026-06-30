
#include <iostream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cerrno>

using namespace std;

bool removeDirectory(const char * path)
{
	if (path == NULL || *path == '\0')
		return false;

	struct stat st;
	// Use lstat() to NOT follow symlinks
	if (lstat(path, &st) != 0)
	{
		cout<<path<<"\n";
		return false; // path cannot be accessed 
	}

	if (S_ISLNK(st.st_mode))
	{
		cout<<path<<"\n";
		return (remove(path) == 0);
	}

	if(S_ISREG(st.st_mode))
	{
		cout<<path<<"\n";
		return (remove(path) == 0);
	}

	if(S_ISDIR(st.st_mode))
	{
		DIR *dir = opendir(path);
		if (dir == NULL)
		{return false; }
		struct dirent* entry;
		bool success = true;
		while (( entry = readdir(dir)) != NULL)
		{
			if(strcmp(entry ->d_name,".") == 0 || strcmp(entry->d_name,"..")==0)
			{
				continue;
			}

			std::string fullPath = std::string(path) + "/" + entry->d_name;
			if(!removeDirectory(fullPath.c_str()))
			{
				success = false;
			}

		}
		if(rmdir(path) != 0)
		{
			success = false;
		}
		cout<<path<<"\n";
		return success;
	}
	cout<<path<<"\n";
	return (remove(path) ==0 );
}

void removePath(const char * path)
{
	if (path == NULL || *path == '\0')
		return;

	struct stat st;
	if (lstat(path, &st) != 0)
	{
		return;
	}

	removeDirectory(path);
}

int main()
{

	removePath("/tmp/aaa");

	return 0;
}
