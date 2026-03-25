#include <iostream>
#include <fstream>
#include <set>
#include <sys/inotify.h>
#include <unistd.h>
#include <cstring>

#define EVENT_BUF_LEN (1024 * (sizeof(struct inotify_event) + 16))

#define DPKG_STATUS "/var/lib/dpkg/status"
#define APPLICATIONS_DIR "/usr/share/applications"

std::set<std::string> knownPackages;

void loadInstalledPackages()
{
    std::ifstream file(DPKG_STATUS);
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

void updateSoftwareList(const std::string &pkg)
{
    std::ofstream sw("/opt/MicroWorld/etc/softwares.list", std::ios::app);
    sw << pkg << std::endl;
}

void detectNewPackages()
{
    std::ifstream file(DPKG_STATUS);
    std::string line;

    while (getline(file, line))
    {
        if (line.find("Package:") == 0)
        {
            std::string pkg = line.substr(9);

            if (knownPackages.find(pkg) == knownPackages.end())
            {
                std::cout << "New package installed: " << pkg << std::endl;

                updateSoftwareList(pkg);

                knownPackages.insert(pkg);

                // sendhwevent() or server notification can be added here
            }
        }
    }
}

int main()
{
    int fd = inotify_init();

    if (fd < 0)
    {
        perror("inotify_init");
        return 1;
    }

    int wd1 = inotify_add_watch(fd, DPKG_STATUS, IN_MODIFY);
    int wd2 = inotify_add_watch(fd, APPLICATIONS_DIR, IN_CREATE);

    if (wd1 == -1 || wd2 == -1)
    {
        perror("inotify_add_watch");
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

            if (event->wd == wd1 && (event->mask & IN_MODIFY))
            {
                std::cout << "dpkg database modified\n";
                detectNewPackages();
            }

            if (event->wd == wd2 && (event->mask & IN_CREATE))
            {
                std::cout << "New application file created: "
                          << event->name << std::endl;
            }

            i += sizeof(struct inotify_event) + event->len;
        }
    }

    close(fd);
}
