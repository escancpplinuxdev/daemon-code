#include <iostream>
#include <string>
#include <cctype>

std::string normalize(const std::string& s)
{
    std::string result;

    // Step 1: Convert to lowercase and collect non‑space characters
    std::string lower;
    for (size_t i = 0; i < s.length(); ++i)
    {
        lower += std::tolower(static_cast<unsigned char>(s[i]));
    }

    // Step 2: Trim leading spaces
    size_t start = lower.find_first_not_of(' ');
    if (start == std::string::npos)
    {
        return "";  // empty string
    }
    size_t end = lower.find_last_not_of(' ');
    lower = lower.substr(start, end - start + 1);

    // Step 3: Collapse multiple spaces into one
    bool inSpace = false;
    for (size_t i = 0; i < lower.length(); ++i)
    {
        if (lower[i] == ' ')
        {
            if (!inSpace)
            {
                result += ' ';
                inSpace = true;
            }
            // else skip this space
        }
        else
        {
            result += lower[i];
            inSpace = false;
        }
    }

    return result;
}

int main()
{
    std::string s1 = "  Google   Chrome  ";
    std::string s2 = "google chrome";

    if (normalize(s1) == normalize(s2))
    {
        std::cout << "Strings are equal (case‑insensitive, trimmed, spaces collapsed)." << std::endl;
    }
    else
    {
        std::cout << "Strings are NOT equal." << std::endl;
    }

    return 0;
}
