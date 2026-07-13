
//g++     ifdef_else_endif_in_code.cpp   -o ifdef_else_endif_in_code -D__WMCBUILD__
#include <iostream>
#include <cstring>
#include "print.h"
#include "ifdef_else_endif_in_code.h"

//int printFormMain(int *len1, int *len2)
std::ostream& printFormMain(int *len1, int *len2)
{
	std::cout<<"printFormMain \n";
	return *len1 > *len2 ? std::cout<<"len1 = '"<<*len1<<"'\n" : std::cout<<"len2 = '"<<*len2<<"'\n";
}

int main(int argc, char **argv)
{
	if (argc !=3 )
	{
		std::cout<<"argument issue, less argument\n";
		printf("Usage: passprompt <serial> <device_name>\n");
		return 0;
	}
		
	int len1 = strlen(argv[1]);
	int len2 = strlen(argv[2]);

	std::cout<<"len1 = '"<<len1<<"' len2 = '"<<len2<<"'\n";
	
	if(len1 > 128 || len1 < 2)
	{
		std::cout<<"len1 > 128 || len1 < 2\n";
	}
	else
	{
		std::cout<<"not len1 > 128 || len1 < 2\n";
	}
	

	if(len2 > 256 || len2 < 2)
	{
		std::cout<<"len2 > 256 || len2 < 2\n";
	}
	else
	{
		std::cout<<"not len2 > 256 || len2 < 2\n";
	}

#ifdef __WMCBUILD__
	std::cout<<"__WMCBUILD__\n";
#else
	std::cout<<"__SOHO__\n";
#endif


#ifndef __WMCBUILD__
	std::cout<<"ifnotdef ____WMCBUILD__\n";
#else
	std::cout<<"#ifdef __WMCBUILD__\n";
#endif

	print(NULL);
	printFormMain(&len1,&len2);	

	return 0;
}
