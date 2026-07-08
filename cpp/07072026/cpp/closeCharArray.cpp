
#include <iostream>
#include <cstring>

void print(char * PkgName, char * Version)
{
		
	std::cout<<PkgName<<"|"	<<Version<<"\n";
//	Version[0] = '\0'; // work 
//	PkgName[0] = '\0';  // work
//	strcpy(Version,"");  // work

//	memset(Version,0,20); // work on size less or eqaul to assign size 
//	memset(Version,0,30);   //if put size more than assign charArray here 20 to 30 then ->
//*** stack smashing detected ***: terminated
//Aborted (core dumped)

	bzero(Version,20); // work on size less or eqaul to assign size
//	bzero(Version,30); //if put size more than assign charArray here 20 to 30 then ->
//*** stack smashing detected ***: terminated
//Aborted (core dumped)

/*	
	std::cout<<PkgName<<"|"	<<Version<<"\n";
	char * AppName  = "VLC media Player";
	char  * temp = "";
	std::cout<<AppName<<"\n";
	if(strcmp (Version, "") == 0)
	{
		sprintf(temp,"%s|",AppName);
		strcpy(AppName,temp);
	}
	std::cout<<AppName<<"\n";
*/
	char *AppName = strdup("VLC Media Player");
	char * temp = (char*) malloc(strlen(AppName) + 2);
	if(temp)
	{
		sprintf(temp,"%s|",AppName);
		free(AppName);
		AppName = temp;
	}
	
	std::cout<<AppName<<"\n";
}

int main()
{
	char PkgName[20] = "Firefox";
	char Version[20] = "3.01.25";
	
	print(PkgName,Version);
	char *cPtr = NULL;
	cPtr = "Google Chrome"; 
	if (strcmp(cPtr,"Google Chrome") == 0)
	{
		std::cout<<cPtr<<"\n";
	}
	return 0;
}
