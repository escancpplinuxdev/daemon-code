#include <iostream>
#include <typeinfo>
#include <stdexcept>

struct Calculator 
{
    int modifier = 3;
    int (Calculator::*member_ptr) = &Calculator::modifier; // Pointer-to-member
    
    int compute(int val) 
    {
        return val * 2;
    }
};

int main() 
{
    Calculator calc;
    Calculator* calc_ptr = &calc;
    
    int arr[1] = { 4 };
    int* ptr = arr;
    
    int initial = 5;
    int target = 0;
    int result = 0;

    try 
    {
        // =========================================================================
        // THE COMBINED STATEMENT (Runs levels 1 through 18 sequentially)
        // =========================================================================
        result = (target = (initial > 0) ? (void(std::cout), sizeof(++*ptr), (calc_ptr->*calc.member_ptr) + (calc_ptr->compute(arr[0])) * 2 >> 1 < 100 == true & 1 ^ 0 | 1 && true || false) : (throw std::runtime_error("Error")), 999);
        // =========================================================================

        std::cout << "Execution finished successfully! Result: " << result << "\n";
    } 
    catch (...) 
    {
        std::cout << "Caught an exception.\n";
    }
    return 0;
}

