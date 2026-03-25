#include <iostream>
#include <cstdio>
#include "log1.h"

using namespace std;

int add(int a, int b)
{
    char buffer[256];
    sprintf(buffer,"Adding numbers %d and %d",a,b);

    log1(1,"Rushi expected command to go executed : %s\n",buffer);
    log1(1,"[%s:%d %s]\n",__FILE__,__LINE__,__FUNCTION__);

    return a+b;
}

int subtract(int a, int b)
{
    char buffer[256];
    sprintf(buffer,"Subtracting %d from %d",b,a);

    log1(1,"Rushi expected command to go executed : %s\n",buffer);
    log1(1,"[%s:%d %s]\n",__FILE__,__LINE__,__FUNCTION__);

    return a-b;
}

int multiply(int a, int b)
{
    char buffer[256];
    sprintf(buffer,"Multiplying %d and %d",a,b);

    log1(1,"Rushi expected command to go executed : %s\n",buffer);
    log1(1,"[%s:%d %s]\n",__FILE__,__LINE__,__FUNCTION__);

    return a*b;
}

float divide(int a, int b)
{
    char buffer[256];
    sprintf(buffer,"Dividing %d by %d",a,b);

    log1(1,"Rushi expected command to go executed : %s\n",buffer);
    log1(1,"[%s:%d %s]\n",__FILE__,__LINE__,__FUNCTION__);

    if(b==0)
    {
        cout<<"Division by zero error\n";
        return 0;
    }

    return (float)a/b;
}

int main()
{
    int a=10;
    int b=5;

    cout<<"Add: "<<add(a,b)<<endl;
    cout<<"Subtract: "<<subtract(a,b)<<endl;
    cout<<"Multiply: "<<multiply(a,b)<<endl;
    cout<<"Divide: "<<divide(a,b)<<endl;

    return 0;
}
