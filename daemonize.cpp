#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

//#include "readConfiguration.h"

extern int user_dbg_level = 1;

using namespace std;


void NFLogLine(short int reqDebugLevel, string logLine )
{
     if(reqDebugLevel <= user_dbg_level)
     {
          FILE *fp = fopen("/var/MicroWorld/var/log/nfdaemon.log", "a");
          if(fp == NULL)
          {
               system("mkdir -p /var/MicroWorld/var/log/");
               FILE *fp = fopen("/var/MicroWorld/var/log/nfDaemon.log", "a");
               if(fp == NULL)
               {
                    return;
               }
          }

          {
               time_t sinceepoch = time(NULL);
               struct tm *rpt_time = localtime(&sinceepoch);
               char rtime[24];
               memset(rtime, 0, 24);
               strftime(rtime, 23, "[%d-%b-%Y@%H:%M:%S] ", rpt_time);
               fprintf(fp,"%s ", rtime );
               fprintf(fp, "%s\n",logLine.c_str() ? logLine.c_str() :"NULL");
               fclose(fp);
          }
     }
}

int daemonize(void)
{
	pid_t pid;
	int fds[3],i;

	fds[0] = open("/dev/null", O_RDONLY);
	fds[1] = open("/dev/null", O_WRONLY);
	fds[2] = open("/dev/null", O_WRONLY);
	if(fds[0] == -1 || fds[1] == -1 || fds[2] == -1) {
		fputs("Can't open /dev/null\n", stderr);
		for( i = 0; i <= 2; i++)
			if(fds[i] != -1)
				close(fds[i]);
		return -1;
	}
	for(i = 0; i <= 2; i++) {
		if(dup2(fds[i], i) == -1) {
			return -1;
		}
	}
	for(i = 0; i <= 2; i++)
		if(fds[i] > 2)
			close(fds[i]);

	pid = fork();
	if(pid == -1)
		return -1;
	if(pid)
		exit(0);
	setsid();
	return 0;
}

int main()
{
	if(daemonize() < 0)
	{
		NFLogLine(1,string("process not daemonize"));
//		return 0;
	}
	else
	{
		NFLogLine(1,string("process daemonize"));
		
	}

	return 0;
}
