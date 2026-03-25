#include <iostream>
#include <dirent.h>
#include <set>
#include <string>
#include <unistd.h>
#include <ctime>
#include <fstream>

using namespace std;

string snapshotFile = "previousset1";

/* Print current time */
void printTime()
{
    time_t now = time(NULL);
    cout << ctime(&now);
}

/* Read directory files */
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

/* Load snapshot from file */
set<string> loadSnapshot()
{
    set<string> files;
    ifstream fin(snapshotFile.c_str());

    string name;
    while (getline(fin, name))
    {
        files.insert(name);
    }

    fin.close();
    return files;
}

/* Save snapshot to file */
void saveSnapshot(const set<string> &files)
{
    ofstream fout(snapshotFile.c_str());

    for (set<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        fout << *it << endl;
    }

    fout.close();
}

int main()
{
    string directory = "/usr/share/applications/";

    /* Load previous snapshot from disk */
    set<string> previousFiles = loadSnapshot();

    cout << "Monitoring directory: " << directory << endl;

    while (1)
    {
        sleep(5);

        set<string> currentFiles = readDirectory(directory);

        set<string>::iterator it;

        /* Detect newly created files */
        for (it = currentFiles.begin(); it != currentFiles.end(); ++it)
        {
            if (previousFiles.find(*it) == previousFiles.end())
            {
                cout << "File Created: " << *it << " | Time: ";
                printTime();
            }
        }

        /* Detect deleted files */
        for (it = previousFiles.begin(); it != previousFiles.end(); ++it)
        {
            if (currentFiles.find(*it) == currentFiles.end())
            {
                cout << "File Deleted: " << *it << " | Time: ";
                printTime();
            }
        }

        /* Save new snapshot */
        saveSnapshot(currentFiles);

        previousFiles = currentFiles;
    }

    return 0;
}
