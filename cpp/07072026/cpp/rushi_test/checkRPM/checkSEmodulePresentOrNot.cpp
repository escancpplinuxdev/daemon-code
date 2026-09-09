
#include<iostream>
#include<cstring>


bool isSemoduleLoaded()
{
    // Execute: semodule -l | grep -i microworld_ssh
    int ret = system("semodule -l 2>/dev/null | grep -qi microworld_ssh");
    return (ret == 0);  // ret=0 means found
}

int main()
{
        if(isSemoduleLoaded())
        {

                std::cout<<"Semodule Loaded. \n";
        }
        else
        {
                std::cout<<"No Semodule. \n";
        }

        return 0;
}

