
#include <iostream>

void printSize(int arr[])   // arr is actually int* (decayed)
{
    std::cout << "Inside function: sizeof(arr) = " << sizeof(arr) << '\n';
    // sizeof(int*) = 8 (on 64-bit)
}

int main()
{
    int arr[10] = {0};
    std::cout << "Inside main    : sizeof(arr) = " << sizeof(arr) << '\n';  // 40 (10 * 4)
    printSize(arr);  // decays to int*
    return 0;
}
