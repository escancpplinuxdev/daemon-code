
#include <iostream>
#include <cstring>
using namespace std;


int main()
{

	char buf[8];
	strncpy(buf,"hello Wolrd!",8);

	cout<<"buf = "<<buf<<endl;

	cout<<"strlen(buf) = "<<strlen(buf)<<endl;

	return 0;
}
