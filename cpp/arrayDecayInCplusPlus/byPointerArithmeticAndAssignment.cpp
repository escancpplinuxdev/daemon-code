
#include <iostream>

int main()
{
    int arr[5] = {1, 2, 3, 4, 5};

    // Decay to int*
    int* p = arr;
    std::cout << "arr[0] = " << *p << '\n';   // 1

    // Pointer arithmetic – decay required
    p = arr + 2;
    std::cout << "arr[2] = " << *p << '\n';   // 3

    // auto deduction – decays to int*
    auto a = arr;
    std::cout << "type of a is int*; size = " << sizeof(a) << '\n';  // 8

    return 0;
}
