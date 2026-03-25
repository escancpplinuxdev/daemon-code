//g++ checkValgrindinBufferOverflow.cpp -o checkValgrindinBufferOverflow
#include <iostream>
#include <stdio.h>
#include <string>

int main()
{
	char buffer[10];
	std::string s="rushi is here ="; // string object s.
//	sprintf(buffer,"%s This string is too long \n",s); // stack overflow
//	snprintf(buffer,"This string is too long \n"); //error --> invalid conversion from ‘const char*’ to ‘size_t’ {aka ‘long unsigned int’}
//	snprintf(buffer,sizeof(buffer),"%s This string is too long \n",s);
	snprintf(buffer,sizeof(buffer),"%s This string is too long \n",s.c_str()); // sizeof to buffer, type casting 

	printf("%s\n",buffer);

	return 0;
}
