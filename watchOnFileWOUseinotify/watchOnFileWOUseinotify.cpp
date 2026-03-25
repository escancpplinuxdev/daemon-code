/*
 * 	g++ watchOnFileWOUseinotify.cpp -o watchOnFileWOUseinotify
 * **/

#include <iostream>
#include <dirent.h>
#include <set>
#include <string>
#include <unistd.h>
#include <ctime>


using namespace std;


void printTime() // print current system time
{
	time_t now = time(NULL);//  time_t -> data type for storing time
	cout << ctime(&now);
}


set<string> readDirectory(const string &path)
{
	set<string> files;
	DIR *dir = opendir(path.c_str());


	if (!dir)
		return files;


	struct dirent *entry;


	while ((entry = readdir(dir)) != NULL)
	{
		string name = entry->d_name;


		if (name != "." && name != "..")
			files.insert(name);
	}


	closedir(dir);
	return files;
}


int main()
{
	//   string directory = "/tmpdata"; rushi
	//   string directory = "//tmpdata/Rushikesh_27022026/watchOnFileWOUseinotify"; rushi
	string directory = "//usr/share/applications/";


	set<string> previousFiles = readDirectory(directory);


	cout << "Monitoring directory: " << directory << endl;


	while (1)
	{
		sleep(5);


		set<string> currentFiles = readDirectory(directory);


		set<string>::iterator it;
/**
 *
 * 1 read current directory file list
2 wait few seconds
3 read directory again
4 compare both sets
5 detect
   new files
   deleted files
 *
 * **/

		// Check for newly created files logic . Here current compare to pervious set
		for (it = currentFiles.begin(); it != currentFiles.end(); ++it)
		{
			if (previousFiles.find(*it) == previousFiles.end())
			{
				cout << "File Created: " << *it << " | Time: ";
				printTime();
			}
		}


		// Check for deleted files logic . Here pervious comapre to current set
		for (it = previousFiles.begin(); it != previousFiles.end(); ++it)
		{
			if (currentFiles.find(*it) == currentFiles.end())
			{
				cout << "File Deleted: " << *it << " | Time: ";
				printTime();
			}
		}


		previousFiles = currentFiles;
	}


	return 0;
}
