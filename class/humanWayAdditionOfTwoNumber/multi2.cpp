#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
    int count;
    vector<string> numbers;

    cout << "Enter total number of Multiplication elements : ";
    cin >> count;
    cout << "Enter number for element \n";

    for (int i = 0; i < count; i++)
    {
        string num;
        cout << "num[" << i + 1 << "] = ";
        cin >> num;
        numbers.push_back(num);
    }

    // Start with product = "1"
    string product = "1";

    // Multiply each number one by one
    for (int j = 0; j < count; j++)
    {
        cout << "\n========== Multiplying by: " << numbers[j] << " ==========\n";
        cout << "Current product = " << product << endl;

        // Convert current number to integer (multiplier)
        int multiplier = stoi(numbers[j]);

        // Multiplication using repeated addition (works for 0, 1, and any other number)
        string newProduct = "0";
        for (int k = 0; k < multiplier; k++)
        {
            // Add product to newProduct (repeated addition)
            string a = newProduct;
            string b = product;

            // Make both strings same length by padding with leading zeros
            int maxLen = (a.length() > b.length()) ? a.length() : b.length();
            while (a.length() < maxLen) a = "0" + a;
            while (b.length() < maxLen) b = "0" + b;

            string result = "";
            int carry = 0;

            // Addition digit by digit
            for (int i = a.length() - 1; i >= 0; i--)
            {
                int digitA = a[i] - '0';
                int digitB = b[i] - '0';
                int total = digitA + digitB + carry;

                string totalStr = to_string(total);
                result = totalStr.back() + result;

                if (totalStr.length() > 1)
                    carry = stoi(totalStr.substr(0, totalStr.length() - 1));
                else
                    carry = 0;
            }
            if (carry > 0)
                result = to_string(carry) + result;

            newProduct = result;
        }

        product = newProduct;
        cout << "Product after multiplying by " << numbers[j] << " = " << product << endl;
    }

    cout << "\n=========================\n";
    cout << "Final Product = " << product << endl;
    cout << "=========================\n";

    return 0;
}
