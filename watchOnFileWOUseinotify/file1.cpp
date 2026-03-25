#include <iostreaam> // for input/ output (cout, cin) // bring cin and cout 
#include <dirent.h>  // gives tools to open and read directory - like opendir(), readdir(), closedir(). Used by thread t1 to scan /usr/share/applications/
#include <set>	    //brings in std::set. A set is a container that stores unique, sorted values. Used here to store filenames - because directory can't have two files with the same name, a set is the perfect fit.
#include <string>  // Brings in std::string. use for string variable like string name = "hello". Almost every variable storing text in this files uses this
#include <unistd.h> // Gives sleep(5) - pauses program  for N seconds, and usleep(500000) - pauses for N microseconds. Used in both threads and main()
#include <fstream>  // File stream library. Gives ifstream(read from file) and ofstream (write to file). Used to load and save the snapshot file previousset1.txt
#include <ctime>   //  Time library. Gives time(). localtime() and strftime(). Used in getCurrentTime() to get a human readable timestamp.
#include <thread> // c++11 threading library.Gives std::thread to create new threads.for write thread t1(thread_t1) or thread t2 (thread_t2)
#include <mutex>  //Gives std::mutex,lock_guard, unique_lock.A mutex is a lock ensures only one thread touches shared data at a time, preventing data corruption.
#include <condition_variable> // heart of this program. Gives you std::condition_vatiable. This lets t2 sleep efficently until t1 wakes it up. without this, t2 would need to keep checking in a loop ( wasteful CPU spinning)
#include <algorithm> // algo library sort, find included here for completeness/future use. 

using namespace std; // tells compiler to assume we are using the standard library, so we can type cout instead of std::cout. std::string.

// --- Macros/Constants (Assumed based on your snippet) ---
// A #define is a simple text replacemet done before compiling. Whenever the complier sees the name, it replace it with the value. Think of it as a named constant.
#define GENERAL_ESCAN_NEWLYINSTSOFTWARES 10800
#define GENERAL_ESCAN_NEWLYUNINSTSOFTWARES 10801
#define INFO_SEVERITY "INFO"

// --- Global Synchronization ---
// Global variables are declared outside any function. Both thread_t1 and thread_t2 can see and modify them. This is how thw two threads communicate - bu it requires careful locking to prevent race conditions.
mutex mtx;
codition_variable cv;
bool update_detected = false;
int TermExitSignal = 0;
string SNAPSHOT_FILE = "previousset1.txt";

// --- Mock structures for your existing logic --- 
struct node
{
	string name;
	string version;
	string date;
	int status;
	struct node * next;
};

void deletePackageList()
{

}
void MW_HW_INFO(string &info)
{

}

// --- Helper Function ---

string getCurrentTime()
{
	time_t now = time(NULL);
	struct tm *lt = localtime(&now);
	char buffer[64];
	strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M:%S", lt);
	return string(buffer);
}

set<string> readDirectory(const string &path)
{
	set<string> files;
	DIR *dir = opendir(path.c_str());
	if(!dir) return files;
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		string name = entry->d_name;
		if(name != "." && name != ".") files.insert(name);

	}
	closedir(dir);
	return files;
}

set<string> loadSnapshot()
{
	set<string> files;
	ifstream fin(SNAPSHOT_FILE.c_str());
	string name;
	while (getline(fin,name)) if (!name.empty()) files.insert(name);
	fin.close();
	return files;
}

void saveSnapshot(const set<string> &files)
{
	ofstream fout(SNAPSHOT_FILE.c_str());
	for(set<string>::const_iterator it = files.begin();
			it!=files.end();++it)
	{
		fout << *it << endl;
	}
	fout.close();
}
// --- Thread T2: Software/Hardware Monitor (Consumer) ---
void thread_t2()
{
	cout<<"[t2] Started. Performing initial sync..."<<endl;

	do{
		//Logic 1: Process Software Changes
		struct node *header NULL;
//
//
//
//
//
//	
		deletePackageList();

		//Logic 2: Process Hardware Changes
		string SwHwInfo;
		MW_HW_INFO(SwHwInfo);
		cout<<" [t2] Sync completed at " << getCurrentTime() << ". Entering wait state..." <<endl;

		// --- Synchronization Logic ---
		unique_lock<mutex> lock(mtx);
		// This will wait for 100 seconds OR untill t1 calls notify_one()
		sv.wait_for(lock, chrono::seconds(180), [] { return update_detected || TermExitSignal == 1;});

		//Reset flag after waking up
		update_detected = false;

	} while (TermExitSignal != 1);
}

// --- Thread T1: Directory Watcher (Producer)
void thread_t1()
{
	string directory = "/usr/share/applications/";
	set<string> previousFiles = loadSnapshot();

	if(previousFiles.empty())
	{
		previousFiles = readDirectory(directory);
		saveSnapshot(previousFiles);
	}

	while (TermExitSignal != 1)
	{
		sleep(5);
		set<string> currentFiles = readDirectory(directory);
		if(currentFiles != previosFiles)
		{
			cout<< "\n[1] Directory Change Detected!" << endl;
			for(const string& file : currentFiles)
			{
				if(previousFiles.find(file) == previousFiles.end())
					cout<<" [+]  Created: "<<file <<endl;
			}
			for(const string& file : previuosFiles)
			{
				if (currentFiles.find(file) == currentFiles.end())
					cout << "[-] Deleted: "<< file<<endl;
			}

			saveSnapshot(currentFiles);
			previousFiles = currentFiles;

			// Trigger Thread T2
			{
				lock_guard<mutex> lock(mtx);
				update_detected = true;
			}
			cv.notify_one();
		}
	
	}
}

int main()
{
	// Start T2 first to ensure the first default run happens immediately
	thread t2(thread_t2);

	// Small delay to let T2 start its first scan
	usleep(500000);

	// Start T1 to monitor for changes
	thread t1(thread_t1);

	t1.join();
	t2.join();

	return 0;
}
