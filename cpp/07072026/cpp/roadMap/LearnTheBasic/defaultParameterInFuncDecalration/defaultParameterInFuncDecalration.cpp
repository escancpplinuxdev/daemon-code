/*
In C++, while declaring the function for the first time all the default parameter should appear_________.
Options: Pick one correct answer from below
To the rightmost side of the parameter list
To the leftmost side of the parameter list
Anywhere inside the parameter list
Middle of the parameter list
Solution description
Default parameters are defined to the rightmost side of the parameter list in a function to differentiate between the normal and default parameters; for example, if a function is defined as fun(int x = 5, int y), then if we call fun(10) then 10 should be given to x or y because one can apply both logics like x = 10 already defined and 10 passed is for y but if compiler reads it from left to right it will think it is for x and no parameter is given for y; therefore, the compiler will give an error. 
*/
#include <iostream>

// ✅ CORRECT: Default parameters are on the rightmost side
void displayVolume(int length, int width = 5, int height = 10) 
{
    std::cout << "Volume: " << (length * width * height) << std::endl;
}

// ❌ INCORRECT (Will cause a compiler error):
// void invalidFunction(int length = 5, int width, int height = 10) {
//     // Error: Default argument missing for parameter 2 ('width')
// }

int main() 
{
    // You can call it by overriding none, some, or all defaults from left to right:
    
    std::cout << "Using all defaults for width and height:" << std::endl;
    displayVolume(2); // length = 2, width = 5 (default), height = 10 (default)

    std::cout << "\nOverriding width, using default height:" << std::endl;
    displayVolume(2, 3); // length = 2, width = 3, height = 10 (default)

    std::cout << "\nOverriding all parameters:" << std::endl;
    displayVolume(2, 3, 4); // length = 2, width = 3, height = 4

    return 0;
}


/*
The correct answer is To the rightmost side of the parameter list.
Why?
In C++, once a parameter is given a default value, all subsequent parameters to its right must also have default values. Therefore, default parameters are always grouped at the end (the rightmost side) of the parameter list.

*/
