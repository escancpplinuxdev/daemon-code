#include<iostream>
#include <cstring>
#include<time.h>
#include "log1.h"
//#include "readConfiguration.h"
void log1 (int currentLogLevel,const char *logLine)
{
        time_t sinceepoch = time (NULL);
        struct tm *rpt_time = localtime (&sinceepoch);
        char rtime[24];
        memset (rtime, 0, 24);

        strftime (rtime, 23, "[%d-%b-%Y@%H:%M:%S] ", rpt_time);

        {
                int debugLevel = 4;
                char buff[50]="";
/*                if(readConfiguration("/opt/MicroWorld/etc/GAblocker.conf","GAblocker:LogLevel",buff) == 0)
                {
                        debugLevel = atoi(buff);
                }
*/
                if(currentLogLevel <= debugLevel)
                {


                        FILE *fp = fopen ("/var/MicroWorld/var/log/Rushi.log", "a");
                        if (fp == NULL)
                        {
                                system ("mkdir -p /var/MicroWorld/var/log/");
                                FILE *fp = fopen ("/var/MicroWorld/var/log/Rushi.log", "a");
                                if (fp == NULL)
                                {
                                        return;
                                }
                        }
                        fprintf (fp, "%s ", rtime);
                        //fprintf(fp, "%s\n",logLine.c_str() ? logLine.c_str() :"NULL");
                        fprintf (fp, "%s\n", logLine ? logLine : "NULL");
                        fclose (fp);
                }
        }
}

