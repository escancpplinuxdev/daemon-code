#include <iostream>
#include <fstream>
#include <set>
#include <sys/inotify.h>
#include <unistd.h>
#include <cstring>

#define EVENT_BUF_LEN (1024 * (sizeof(struct inotify_event) + 16))

std::set<std::string> knownPackages;

void loadInstalledPackages()
{
	std::ifstream file("/var/lib/dpkg/status");
	std::string line;

	while (getline(file, line))
	{
		if (line.find("Package:") == 0)
		{
			std::string pkg = line.substr(9);
			knownPackages.insert(pkg);
		}
	}
}

void detectNewPackages()
{
	std::ifstream file("/var/lib/dpkg/status");
	std::string line;

	while (getline(file, line))
	{
		if (line.find("Package:") == 0)
		{
			std::string pkg = line.substr(9);

			if (knownPackages.find(pkg) == knownPackages.end())
			{
				std::cout << "New package installed: " << pkg << std::endl;

				// Add to software.list
				std::ofstream sw("software.list", std::ios::app);
				sw << pkg << std::endl;

				knownPackages.insert(pkg);
			}
		}
	}
}

int main()
{
	int fd = inotify_init();

	int wd = inotify_add_watch(
			fd,
			"/var/lib/dpkg/status",
			IN_MODIFY);

	if (wd == -1)
	{
		perror("watch");
		return 1;
	}

	std::cout << "Watching dpkg install events...\n";

	loadInstalledPackages();

	char buffer[EVENT_BUF_LEN];

	while (true)
	{
		int length = read(fd, buffer, EVENT_BUF_LEN);

		if (length < 0)
		{
			perror("read");
			break;
		}

		int i = 0;

		while (i < length)
		{
			struct inotify_event *event =
				(struct inotify_event *)&buffer[i];

			if (event->mask & IN_MODIFY)
			{
				std::cout << "dpkg database modified\n";
				detectNewPackages();
			}

			i += sizeof(struct inotify_event) + event->len;
		}
	}

	inotify_rm_watch(fd, wd);
	close(fd);
}
