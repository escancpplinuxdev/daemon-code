#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

// Convert a string to lowercase (C++98 compatible)
std::string toLower(const std::string& s)
{
    std::string result = s;
    // Loop through each character and convert to lowercase
    for (size_t i = 0; i < result.length(); ++i)
    {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

int main()
{
    std::string s1 = "Hello World";
    std::string s2 = "hello world";

    // Compare lowercased versions
    if (toLower(s1) == toLower(s2))
    {
        std::cout << "Strings are equal (case-insensitive)." << std::endl;
    }
    else
    {
        std::cout << "Strings are NOT equal." << std::endl;
    }

    // Alternative: read from user input
    /*
    std::cout << "Enter first string: ";
    std::getline(std::cin, s1);
    std::cout << "Enter second string: ";
    std::getline(std::cin, s2);
    if (toLower(s1) == toLower(s2)) { ... }
    */

    return 0;
}
