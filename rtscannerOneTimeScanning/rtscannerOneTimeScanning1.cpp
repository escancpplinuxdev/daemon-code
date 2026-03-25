#include <iostream>		 // cout and cerr
#include <sys/inotify.h>	 //provide inotify api for file system monitoring
#include <unistd.h>		 //read() and close() system call
#include <limits.h>		 //general system utility
#include <cstring>		 //
#include <sys/stat.h>
#include <filesystem>

#define EVENT_SIZE ( sizeof(struct inotify_event) )
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

namespace fs = std::filesystem;

void processFile(const std::string& filePath)
{
	std::cout << "Scanning file: " << filePath << std::endl;

	// Add actual scanning logic here
}

// 🔹 First scan existing files
void initialScan(const std::string& directory)
{
	std::cout << "\nInitial scanning started...\n";

	for (const auto& entry : fs::directory_iterator(directory))
	{
		if (fs::is_regular_file(entry.path()))
		{
			processFile(entry.path().string());
		}
	}

	std::cout << "Initial scanning completed.\n\n";
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <directory_path>\n";
		return 1;
	}

	std::string directory = argv[1];

	struct stat sb;
	if (stat(directory.c_str(), &sb) == -1 || !S_ISDIR(sb.st_mode))
	{
		std::cerr << "Error: Directory does not exist!\n";
		return 1;
	}

	// 🔹 STEP 1: Initial Scan
	initialScan(directory);

	// 🔹 STEP 2: Start inotify
	int fd = inotify_init();
	if (fd < 0)
	{
		perror("inotify_init");
		return 1;
	}

	int wd = inotify_add_watch(fd, directory.c_str(),
			IN_CREATE | IN_MODIFY | IN_DELETE);

	if (wd == -1)
	{
		perror("inotify_add_watch");
		return 1;
	}

	char buffer[BUF_LEN];

	std::cout << "Monitoring directory: " << directory << std::endl;

	while (true)
	{
		int length = read(fd, buffer, BUF_LEN);

		if (length < 0)
		{
			perror("read");
			return 1;
		}

		int i = 0;
		while (i < length)
		{
			struct inotify_event* event =
				(struct inotify_event*)&buffer[i];

			if (event->len)
			{
				std::string filePath = directory + "/" + event->name;

				if (event->mask & IN_CREATE)
				{
					std::cout << "File created: " << event->name << std::endl;
					processFile(filePath);
				}
				else if (event->mask & IN_MODIFY)
				{
					std::cout << "File modified: " << event->name << std::endl;
					processFile(filePath);
				}
				else if (event->mask & IN_DELETE)
				{
					std::cout << "File deleted: " << event->name << std::endl;
				}
			}

			i += EVENT_SIZE + event->len;
		}
	}

	inotify_rm_watch(fd, wd);
	close(fd);
	return 0;
}
