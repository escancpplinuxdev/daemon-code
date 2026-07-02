
#include <iostream> 	// std::cout, std::cin, std::endl
#include <stdio.h>	// FILE, popen(), pclose(), fgets(), sprintf()
#include <stdlib.h>	// NULL
#include <unistd.h>	// popen() addition defination
#include <string.h>	// strlen()

bool getDispNumByCmd(const char * Name, char * DispNum)
{
	char cmd[1024] = "";
	FILE *fp  = NULL;
	int written = snprintf(cmd,sizeof(cmd),"ps e -u %s | sed -rn 's/.* DISPLAY=(:[0-9]*).*/\\1/p'",Name);
	if (written < 0 || written >= (int)std::size(cmd))
	{
		std::cerr<<"cmd get trucnted\n";	
		return false;
	}

	fp = popen(cmd,"r");
	if(fp == NULL)
	{
		std::cerr<<"popen failed\n";
		return false;
	}
	else
	{
		if (fgets(DispNum,50,fp) == NULL)
		{
			DispNum[0] = '\0';
			std::cerr<<"fgets failed\n";
		}
		else
		{
			size_t len = strlen(DispNum);
			if (len > 0 && DispNum[len - 1] == '\n')
			{
				DispNum[len - 1 ] = '\0';
			}
		}
	}
	pclose (fp);
	return true;

}
int main()
{
	const char  Name[1024] = "omkar";
	char DispNum[24] = "";

	if (getDispNumByCmd(Name, DispNum))
	{
		std::cout<<"DispNum = '"<<DispNum<<"'\n";
	}
	else
	{
		std::cout<<"Failed DispNum = '"<<DispNum<<"'\n";
	}

	return 0;
}
