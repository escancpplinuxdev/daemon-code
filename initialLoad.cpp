
#include <iostream> // for input/output opeartion cin cout
#include <fstream> // file operation ifstream ofstream
#include <vector>  // dynamic array container
#include <algorithm> // sorting and transformation algo
#include <cstring> // c-style string function strlen strcpy
#include <unistd.h> // sleep , usleep like os api
#include <sys/inotify.h> // linux inotify - filesystem event
#include <sys/stat.h> // stat structure
#include <dirent.h> // opendir , readdir

using namespace std; // allow using std:: functions without std::prfix 

#define WATCH_DIR "/usr/share/applications" //directory to monitor for .desktop files

#define SW_FILE "/opt/MicroWorld/etc/softwares.list" // output filepath

extern int readConfiguration (const char* , char * , char *); // reads .desktop file entries 

struct sw_node // linked list node for software entries
{
	char name[256]; // app name for .desktop file
	char versionp[128]; //package version from dpkg
	char filename[256]; // .desktop filename
	char date[30];		// file creation date (stat)
//	sw_node *next; // pointer to the next node in linked list
	sw_node *next = nullptr; // pointer to the next node in linked list // to prevent dangling pointer
};

string toLower(string s) // converts string to lowercase
			 {
				 transform(s.begin(),s.end(),s.begin(),::tolower); // Apply to lower to each other
										  	return s; // Return modified string
			 }

bool isValidDesktopFile(const string &fname) // Validates if file is a .desktop file
{
	if (fname.empty()) return false; // Empty filename - invalid 
        if (fname[0] == '.') return false; // Hidden file - invalid 
	if (fname.size() < 8 ) return false; // Must be at least 8 chars (x.desktop)
	if (fname.substr(fname.size() -8) != ".desktop") return false;
	// Must end with .desktop
	
	if (fname.find(".dpkg")!= string::npos) return false;// Excluse .dpkg backup files

       return true; // valid .desktop file	
}

//------------  linked list operations --------------//
void addNode(sw_node **head, const sw_node &n) // Add node to linked list
{
	// check for duplicated by filename
	for(sw_node *t= *head; t; t= t->next)
	{
		if(strcmp(t->filename, n.filename)==0) // If filename already exists
		 return; // Don't add duplicate
	}

	sw_node *temp = new sw_node; // Allocate new node
	*temp = n; 	// copy data to new node
	temp -> next = *head;// Insert at beginning of list
	*head = temp; // Update head pointer
}

void removeNode(sw_node **head, const string &fname) // Remove node by filename
{
sw_node *curr = *head, *prev = NULL; // current and previous pointers
		while(curr) // traverse list
	{
		if(toLower(curr->filename) == toLower(fname))
			// case-insensitive match 
		{
			if(prev == NULL) //if removing head node
				*head = curr->next; //Update head
				else
					prev->next= curr->next; // Bypass current node

				delete curr; // Free memory 
					return ;
		}

		prev= curr; // Move pointers forward
		curr = curr->next;
	}
}


bool getPackageDetails(const string &fname, sw_node &n) // Get app name and version
{
	char filepath[512];
	sprintf(filepath,"%s%s", WATCH_DIR, fname.c_str()); // build full file path

	char Appname[256]= "";
	char Version[128]="";
	char buffer[512];

	// Read application name from .desktop file 
	if(configuration(filepath,(char*)"Desktop Entry:Name",AppName) <0)
		return false; // Failed to read name

	//Find which package owns this file
	char cmd[512];
	sprintf(cmd,"dpkg-query -S \"%s\" 2>/dev/null"); //command to find package
	FILE *fp = popen(cmd,"r"); // execute command and read output
	if(!fp) return false;

	string output;
	if(fgets(buffer, sizeof(buffer),fp)) // read package name
	{
		output= buffer;
	}
	pclose(fp); // close pipe

	size_t pos = output.find(":");// Find colon separator

	if(pos == string::npos)
		return false; // INvalid output format

	string pkg = output.substr(0,pos); // Extract package name

	// Get package version
	sprintf(cmd, "dpkg -s %s | grep Version | awk '{print $2}'  ",pkg.c_str());

	fp = popen(cmd, "r"); // execute version command
	if (!fp) return false;

	if(fgets(buffer,sizeof(buffer),fp)) // Read version
		strcpy (Version, buffer);

	pclose(fp);

	Version[strcspn(Version,"\n")] = 0; // Remove newline character

	// Get file creation time
	struct stat stats;
	char date[30]="";

	if(stat(filepath,&stats)==0) // Get file stats
	{
		struct tm *timeinfo = localtime(&stats.st_ctime); // convert to local time
		strftime(date,sizeof(date),"[%d/%b/%Y] [%d/%m/%Y]",timeinfo); // Format date
									      // populate node structure 
		strcpy(n.name,Appname);
		strcpy(n.version,Version);
		strcpy(n.filename,fname.c_str());
		strcpy(n.date,date);
		return true; // Sucess
	}



}
void initialLoad(sw_node **head) // Load all existing .desktop files
{
	DIR * dir; // Directory pointer
	struct dirent *ent; // Directory entry
	dir = opendir(WARCH_DIR); // Open applications directory
	if(!dir) // check if opened sucessfully
	{
		XPRINT(1,"[ERROR] FAILED TO OPEN DIRECTORY\n\n");
		return;
	}
	// Read all entries in directory
	while((ent = readdir(dir)) != NULL)
	{
		string fname = ent->d_name; // Get filename
		if(!isValidDesktopFile(fname)) // skip non- .desktop files
		{continue;}

		sw_node n;
		bool sucess = false;

		// Retry up to 50 times(1 sec delay between attempts)
		for(int i=0; i<5;i++)
		{
			if(getPackageDeatils(fname,n)) 
			{
				sucess = true;
				break;
			}
			sleep(1); // wait 1 sec before retry
		}

		if (!sucess) // skip if still failing
		{
			XPRINT(1,"[INIT-DEBUG] skip: %s\n\n".fname_c_str());
			continue;
		}
		addNode(head ,n); // Add to linked list
	}

	closedir(dir); // Close directory

	XPRINT(1,"[INIT] Inital software list loaded\n\n");


}
void monitorSoftwares() // main monitoring loop
{
	int fd = inotify_init(); // initialize inotify instance
				 // Add watch for create,delete and move events
	int wd = inotify_add_watch(fd,WATCH_DIR,IN_CREATE|IN_DELETE|IN_MOVED_TO|IN_MOVED_FROM);
	char buffer[4096]; // Buffer for events 
	sw_node *head = NULL; // Initialize empty list

	// Initial sync - lead existing files

	initialLoad(&head);

	XPRINT(1,"Monitoring started...\n\n");
	// Main event loop 
	while(1)
	{
		int length = read(fd,buffer,sizeof(buffer)); // Read events
		if (length <= 0) continue; // No events, continue
		int i =0; //Index into buffer
			  // Process all events in buffer
		while(i<length)
		{
			struct inotify_event *event = (struct inotify_event * )&buffer[i]; // type Cast to event structure
			if(event->len >0) // If event has a filename
			{
				string fname = event->name; // Get filename
				if(!isValidDesktopFile(fname)) // Skip ono- . desktop files
				{
					i+= sizeof(struct inotify_event) + event->len;
					continue;

				}
				sw_node n;
				// ------ INSTALL EVENT -----//
				if(event->mask & (IN_CREATE | IN_MOVED_TO))
				{
					bool sucess = false;
					// Retry up to 50 times (1 sec between attempts)
					for(int attempt = 1; attempt <= 50; attempt++)
					{
						if(getPackageDetails(fname, n)) // Try to get details
						{
							sucess = true;
							break;
						}
						XPRINT(1,"[debug] ATTEMPT %d: dpkg not ready for :%s\n",attempt,fname.c_str());
						sleep(1);
						//wait 1 sec before retry
					}
					if(sucess) // If package details retrieved
					{
						XPRINT(1,"[+] Installed : [%s] %s.\n\n",fname.c_str(),n.name);

				addNode(&head,n); // Add to list
				head = sortList(head); // sort list
				writeFile(head); // Write to file
					}
					else
					{
						XPRINT(1,"[ERROR] DPKG FAILED FOR:%s\n\n",fname.c_str());
					}
				}


				//----------Uninstall EVENT---------
			if(event->mask & (IN_DELETE | IN_MOVED_FROM))
			{
				XPRINT(1,"[-] Removed: [%s]\n\n",fname.c_str());
				removeNode(&head,fname); // Remove from list
				head = sortList(head); //Sort list
				writeFile(head); // Write to file
			}
			}

			//Move to next event in buffer
		i += sizeof(struct inotify_event)+ eent->len;

		}

	}
}


int main()	// program entry point 
{
	monitorSoftwares(); // start monitoring (never returns
	return 0; // Never reached (infinite loop)
}
