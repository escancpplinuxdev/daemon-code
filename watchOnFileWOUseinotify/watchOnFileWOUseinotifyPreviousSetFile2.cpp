#include <iostream>
#include <dirent.h>
#include <set>
#include <string>
#include <unistd.h>
#include <fstream>
#include <ctime>

using namespace std;

string SNAPSHOT_FILE = "previousset1.txt";

/* Print human readable time */
string getCurrentTime()
{
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M:%S", lt);

    return string(buffer);
}

/* Read directory */
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
        {
            files.insert(name);
        }
    }

    closedir(dir);
    return files;
}

/* Load snapshot from file */
set<string> loadSnapshot()
{
    set<string> files;

    ifstream fin(SNAPSHOT_FILE.c_str());
    string name;

    while (getline(fin, name))
    {
        files.insert(name);
    }

    fin.close();
    return files;
}

/* Save snapshot */
void saveSnapshot(const set<string> &files)
{
    ofstream fout(SNAPSHOT_FILE.c_str());

    for (set<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        fout << *it << endl;
    }

    fout.close();
}

int main()
{
    string directory = "/usr/share/applications/";

    cout << "Monitoring directory: " << directory << endl;

    set<string> previousFiles = loadSnapshot();

    if (previousFiles.empty())
    {
        previousFiles = readDirectory(directory);
        saveSnapshot(previousFiles);
    }

    while (1)
    {
        sleep(5);

        set<string> currentFiles = readDirectory(directory);

        set<string>::iterator it;

        /* Detect created files */
        for (it = currentFiles.begin(); it != currentFiles.end(); ++it)
        {
            if (previousFiles.find(*it) == previousFiles.end())
            {
                cout << "File Created: " << *it
                     << " : Time " << getCurrentTime() << endl;
            }
        }

        /* Detect deleted files */
        for (it = previousFiles.begin(); it != previousFiles.end(); ++it)
        {
            if (currentFiles.find(*it) == currentFiles.end())
            {
                cout << "File Deleted: " << *it
                     << " : Time " << getCurrentTime() << endl;
            }
        }

        saveSnapshot(currentFiles);
        previousFiles = currentFiles;
    }

    return 0;
}
