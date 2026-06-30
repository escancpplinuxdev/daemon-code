#include <iostream>
#include <string>
#include <cctype>

// Trim leading/trailing spaces and collapse multiple spaces into one
std::string trim(const std::string& s)
{
    // Step 1: Trim leading/trailing spaces
    size_t start = s.find_first_not_of(" \t");
    if (start == std::string::npos)
    {
        return "";
    }
    size_t end = s.find_last_not_of(" \t");
    std::string trimmed = s.substr(start, end - start + 1);

    // Step 2: Collapse multiple spaces into one
    std::string result;
    bool inSpace = false;
    for (size_t i = 0; i < trimmed.length(); ++i)
    {
        if (trimmed[i] == ' ')
        {
            if (!inSpace)
            {
                result += ' ';
                inSpace = true;
            }
            // else skip extra spaces
        }
        else
        {
            result += trimmed[i];
            inSpace = false;
        }
    }

    return result;
}

// Convert a string to lowercase (C++98 compatible)
std::string toLower(const std::string& s)
{
    std::string result = s;
    for (size_t i = 0; i < result.length(); ++i)
    {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

int main()
{
    std::string s1 = "  Hello     WorLd  ";
    std::string s2 = "hello world";

    if (toLower(trim(s1)) == toLower(trim(s2)))
    {
        std::cout << "Strings are equal (case-insensitive, trimmed, spaces collapsed)." << std::endl;
    }
    else
    {
        std::cout << "Strings are NOT equal." << std::endl;
    }

    return 0;
}
