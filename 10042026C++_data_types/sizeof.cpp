#include <iostream>
using namespace std;

struct A 
{
    char c;
    int i;
};

struct B 
{
    int i;
    char c;
};

struct C 
{
    char c1;
    char c2;
    int i;
};

int main() 
{
    cout << sizeof(A) << endl;
    cout << sizeof(B) << endl;
    cout << sizeof(C) << endl;
    return 0;
}
