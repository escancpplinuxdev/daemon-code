#include <iostream>

// Mark a function as obsolete/deprecated with a helpful message
[[deprecated("Use modern_sum() instead, as this function is unsafe.")]]  // comment which show warning for obsolete complier code message
int old_sum(int a, int b) 
{
    return a + b;
}

int main() 
{
    // This will compile, but the compiler will throw a prominent warning
    std::cout << old_sum(5, 10)<<"\n"; 
    return 0;
}
