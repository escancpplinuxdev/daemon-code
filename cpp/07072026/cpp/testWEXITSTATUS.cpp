
#include <iostream>

int main()
{
	int ret = system( " which brave >/dev/null 2>&1");
	int ret1 = system("which brave-browser >/dev/null 2>&1"); //redirect stdoutput to /dev/null

	if ( WEXITSTATUS (ret) == 0 || WEXITSTATUS (ret1) == 0 )
	{
		std::cout<<"brave found\n";
	}

	char entry[1000] = "";
	int  scheduleMinute  = 5, scheduleHour  = 4 ,monthDay = 3;
	char user[20] = "rushi";
snprintf(entry, 999, "%u %u %u * * %s /opt/MicroWorld/usr/bin/updatenow_avs >> /var/MicroWorld/var/log/autoupdate_avs.log 2>&1; /opt/MicroWorld/usr/bin/updatenow >> /var/MicroWorld/var/log/autoupdate.log 2>&1\n", scheduleMinute, scheduleHour, monthDay, user);
	
	std::cout<<"[ENTRY] "<<entry<<"\n";

	return 0;
}

//(date +'[%d-%b-%Y@%H:%M:%S]'; /opt/MicroWorld/usr/bin/updatenow_avs) >> /var/MicroWorld/var/log/autoupdate_avs.log 2>&1
