/**
 *char buffer [256];
log1(1,"Rushi expected command to go executed : %s\n",buffer);

log1(1,"[%s:%d %s]\n",__FILE__,__LINE__,__FUNCTION__);
 * **/




#include<iostream>
#include <cstring>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include "log1.h"

void log1 (int currentLogLevel,const char *format,...)
{
    time_t sinceepoch = time(NULL);
    struct tm *rpt_time = localtime(&sinceepoch);

    char rtime[24];
    memset(rtime,0,sizeof(rtime));

    strftime(rtime,23,"[%d-%b-%Y@%H:%M:%S]",rpt_time);

    int debugLevel = 4;

    if(currentLogLevel <= debugLevel)
    {
        FILE *fp = fopen("/var/MicroWorld/var/log/Rushi.log","a");

        if(fp == NULL)
        {
            system("mkdir -p /var/MicroWorld/var/log/");
            fp = fopen("/var/MicroWorld/var/log/Rushi.log","a");
            if(fp == NULL)
                return;
        }

        fprintf(fp,"%s ",rtime);

        /* Handle variable arguments */
        va_list args;
        va_start(args,format);
        vfprintf(fp,format,args);
        va_end(args);

        fclose(fp);
    }
}
