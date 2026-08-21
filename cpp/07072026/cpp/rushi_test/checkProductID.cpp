
#include <iostream>
#include <fstream>
#include <string>

std::string getProductID()
{
    const char* filename = "/opt/MicroWorld/etc/escan-ver";
    std::ifstream file(filename);
    
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return "";
    }
    
    std::string line;
    int lineCount = 0;
    std::string productID = "";
    
    while (std::getline(file, line))
    {
        ++lineCount;
        if (lineCount == 2)
        {
            // Extract only the product ID (e.g., "5CL" from "5CL 1.0A")
            size_t pos = line.find(' ');
            if (pos != std::string::npos)
            {
                productID = line.substr(0, pos);
            }
            else
            {
                productID = line;
            }
            break;
        }
    }
    
    return productID;
}

int main()
{
    std::string productID = getProductID();
    if (!productID.empty())
    {
        std::cout << "Product ID: " << productID << std::endl;
    }
    else
    {
        std::cout << "Product ID not found" << std::endl;
    }
    return 0;
}
