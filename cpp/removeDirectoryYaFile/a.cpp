
#include <iostream>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cerrno>

// Recursively remove a directory and all its contents, including symlinks
bool removeDirectory(const char* path)
{
	if (path == NULL || *path == '\0')
	{
		return false;
	}

	struct stat st;
	// Use lstat() to NOT follow symlinks
	if (lstat(path, &st) != 0)
	{
		return false; // doesn't exist
	}
	// If it's a symlink, remove it directly
	if (S_ISLNK(st.st_mode))
	{
		return (remove(path) == 0);
	}

	//If it's a regular file, remove it
	if (S_ISREG(st.st_mode))
	{
		return (remove(path) == 0);
	}

	// If it's a directory, process it contents
	if (S_ISDIR(st.st_mode))
	{
		DIR* dir = opendir(path);
		if (dir == NULL)
		{
			return false;
		}

		struct dirent* entry;
		bool success = true;
		while((entry = readdir(dir)) != NULL)
		{
			if(strcmp(entry ->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0)
			{
				continue;
			}
			std::string fullPath = std::string(path) + "/" + entry->d_name;
			if(!removeDirectory(fullPath.c_str()))
			{
				success = false;
			}
		}
		closedir(dir);

		// Remove the now-empty directory
		if(rmdir(path) != 0)
		{
			success = false;
		}	
		return success;

	}

	// Other file types (fifo, socket, etc.) - try to remove anyway

	return(remove(path) == 0);


}





void removePath(const char* path)
{
	if(path == NULL || *path == '\0')
	{
		return;
	}

	struct stat st;
	if(lstat(path,&st) != 0)
	{
		return; // does not exist
	}

	removeDirectory(path);
}
int main()
{
	removePath("/tmp/aaa") ;



	return 0;
}

