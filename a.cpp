#include "readConfiguration.h"

///Sends event when software, hardware insformation changes.
/**
 * \sa sendStatusEvent()
 * \sa eventsender() 
 * \sa getPackageStatus()
 * \sa deletePackageList()
 * \sa MW_HW_INFO()
 * \sa checknupdateHWevent() 
 */
void* sendhwevent(void *)
{
	/**
	 * All other availabe feature events will be snet before sending this event.
	 */
	sendStatusEvent(NULL);

	/**
	 * If installed software list is not sent to the server before, it creates a list of all installed software and sent these information in events. Number of 10800 events will be sent, each for one software installed.

	 XXPRINT(1, "[%s:%d %s] .\n\n", __FILE__, __LINE__, __FUNCTION__);
	 */
	if ( access( SOFTWARELIST_FILE, F_OK ) != 0 )
	{
		struct node *header = NULL;
		if( ( header = getPackageStatus( ) ) == NULL )
		{
			XXPRINT(1,"Failed to fetch present software list.\n");
		}

		else
		{
			struct node *iter, *temp;
			string description;

			for( iter = header; iter != NULL;  iter = iter->next)
			{
				if( strlen( iter->date) != 0 && iter->name != NULL )
				{
					description = "Software Already Present ";
					description += iter->date;
					description += " {Ver:";
					description += iter->version;
					description += "}";
					if((strstr(iter->name, "eScan administrator") != NULL && strstr(iter->name, "eScan administrator (root)") == NULL) || strstr(iter->name, "eScan command-line"))
					{
						//Skip multiple escan packages, allow ony "eScan administrator (root)"
						continue;
					}
					//eventsender( GENERAL_ESCAN_NEWLYINSTSOFTWARES, INFO_SEVERITY,(char *)description.c_str(), (char *)"Software Install/Uninstall Status", (char *)iter->name, (char *)"winclient" );
					eventsender( GENERAL_ESCAN_PRESENTSOFTWARES, INFO_SEVERITY,(char *)description.c_str(), (char *)"Software Install/Uninstall Status", (char *)iter->name, (char *)"winclient" );
					sleep(1);
				}
			}

			deletePackageList();
		}
	}
	//       It keeps track of all installed softwares and list of software names sent to the server.

	//Here monitor software changes



	int fd = inotify_init();
	int wd = inotify_add_watch(fd, WATCH_DIR, IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM);
	/// ------   write install app c++ code here ----- ///
}


int findInstalledSoftwares()
{
	DIR *directory;
	struct dirent *ent;
	struct stat stats;
	struct tm *timeinfo;
	char buffer[BUFFER_SIZE]="", filePath[BUFFER_SIZE]="", date[30]="";//, *cPtr = NULL;
	FILE *inputFile, *outputFile;

	/**
	 * All software details will be stored in a temporary file which is not sorted.
	 */

	outputFile = fopen(UNSORTED_FILE, "w+");
	if(outputFile == NULL)
	{
		return -1;
	}
	/*File names ends with '.desktop' in '/usr/share/applications' directory will be used to get the software details.*/

	directory = opendir(APPLICATIONS_DIR);
	if(directory == NULL)
	{
		fclose(outputFile);
		return -1;
	}
	while ((ent = readdir (directory)) != NULL)
	{
		char *cPtr = NULL;
		char NFile[1024]="";
		strcpy(NFile,ent->d_name);

		cPtr = strcasestr(NFile, ".desktop");
		/**
		 * Files which does not ends with '.desktop' in their names, will be ignored.
		 */
		if(cPtr == NULL)
		{
			continue;
		}
		sprintf(filePath, "%s/%s", APPLICATIONS_DIR, ent->d_name);
		/**
		 * If file's statastics could not obtained, or open for reading, those file will be ignored.
		 */
		if(stat(filePath, &stats) != 0)
		{
			continue;
		}


		if(readConfiguration(filePath,(char *) "Desktop Entry:Name", cPtr) < 0)
		{
			continue;
		}

		char PkgName[1024]="",Version[1024]="";

		if(GetPkgNameIfAppName(PkgName,Version,filePath))
		{
			fprintf(outputFile,"%s|%s\n" , cPtr,Version);
		}
		else
		{
			if(strcasestr(cPtr,"escan") == NULL)
			{
				if(fputs(cPtr, outputFile) < 0)
				{}
				if(fputs("\n", outputFile) < 0)
				{}
			}

		}




		if(isNotifiedBefore(cPtr,Version) == 1)
		{
			//This function check that whether given software is notified before or not.
			//It returns 1 for notified software continue go to next software, 0 otherwise go for next step

			continue;
		}
		/**
		 * Software details those are not yet sent to the server will be added into the list. The installation date will be taken as (.desktop) file creation date. The date format would be '[DD/MM/YYYY] [DD/MMM/YYYY]'. Failed to add any of the software into the list, will be treated as an error and -1 will be returned.
		 */
		timeinfo = localtime(&(stats.st_ctime));
		strftime(date, sizeof(date), "[%d/%b/%Y] [%d/%m/%Y]", timeinfo);
		if(addToNotificationList(cPtr, 0, date,Version) != 0)
		{

			closedir(directory);
		}
	}

	closedir(directory);
	fclose(outputFile);
	/**
	 * The temporary file will be sorted and renamed. Failure case of sorting and renaming the file on disk will be ignored.
	 */
	if(access(UNSORTED_FILE, F_OK) == 0)
	{
		sprintf(buffer, "sort -u -o %s %s && rm -f %s", SORTED_FILE, UNSORTED_FILE, UNSORTED_FILE);// Rushi 060326 test original  make coment for testing



		//        sprintf(buffer, "sort -u -o %s %s ", SORTED_FILE, UNSORTED_FILE); // Rushi 060326 test change original
		sprintf(buffer, "sort -u -o %s %s && mv -f %s %s ", SORTED_FILE, UNSORTED_FILE, UNSORTED_FILE,"/opt/MicroWorld/etc/softwares.list.tmp_1_rushi");// Rushi 060326 test original  make coment for testing
		if(system(buffer) != 0)
		{}
	}
	return 0;
}

#define BUFFER_SIZE      1024
#define APPLICATIONS_DIR "/usr/share/applications"
#define SORTED_FILE       "/opt/MicroWorld/etc/softwares.list.tmp"
#define UNSORTED_FILE     "/opt/MicroWorld/etc/softwares.list.tmp_1"
#define WATCH_DIR "/usr/share/applications/"
#define SW_FILE   "/opt/MicroWorld/etc/softwares.list"


char buffer2[256];

use inotify watch on /usr/share/applications/ directory where
1] in App install filename.desktop file created on filepath = /usr/share/applications/filename.desktop

AppName =  if(readConfiguration(filePath,(char *) "Desktop Entry:Name", AppName) < 0)
{
	//   ABLogLine(4,string("Failed to get Desktop Entry:Name in ")+string(DesktopFile));
	continue;
}

pkgname	= dpkg --search \"%s\"  2>/dev/null",filepath 
version = dpkg -s %s |grep Version |awk -F' ' '{print $2}'",PkgName

date    =    timeinfo = localtime(&(stats.st_ctime));
strftime(date, sizeof(date), "[%d/%b/%Y] [%d/%m/%Y]", timeinfo);
if(addToNotificationList(cPtr, 0, date,Version) != 0)


	so if i install vlc by using 
	apt install vlc then add  vlc-appname|version in softwares.list file in sorted order with pervious data example

	Before apt install vlc

	root@rushi-VB-220:~# cat /opt/MicroWorld/etc/softwares.list | grep -iE "vlc|gnu|gedit|chrome|calculator|evince"
	Calculator|1:48.0.2-1ubuntu3
	Evince|48.1-3ubuntu2
	GNU Image Manipulation Program|3.0.4-6.1
	Google Chrome|146.0.7680.153-1

	After apt install vlc

	root@rushi-VB-220:~# cat /opt/MicroWorld/etc/softwares.list | grep -iE "vlc|gnu|gedit|chrome|calculator|evince"
	Calculator|1:48.0.2-1ubuntu3
	Evince|48.1-3ubuntu2
	GNU Image Manipulation Program|3.0.4-6.1
	Google Chrome|146.0.7680.153-1
	VLC media player|3.0.21-11

	here it is sorted order.
	last output is /opt/MicroWorld/etc/softwares.list with previos app-name|version  appended new install app-name|version in sorted 

