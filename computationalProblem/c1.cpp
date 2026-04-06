#include <iostream>
using namespace std;

string one[] = {"", "One ", "Two ", "Three ", "Four ", "Five ", "Six ", "Seven ", "Eight ", "Nine ",
                "Ten ", "Eleven ", "Twelve ", "Thirteen ", "Fourteen ", "Fifteen ", "Sixteen ",
                "Seventeen ", "Eighteen ", "Nineteen "};

string ten[] = {"", "", "Twenty ", "Thirty ", "Forty ", "Fifty ",
                "Sixty ", "Seventy ", "Eighty ", "Ninety "};

// Function to convert number less than 1000
string numToWords(int n)
{
    string str = "";

    if (n > 99)
    {
        str += one[n / 100] + "Hundred ";
        n %= 100;
    }

    if (n > 19)
    {
        str += ten[n / 10];
        n %= 10;
    }

    if (n > 0)
    {
        str += one[n];
    }

    return str;
}

string convertToWords(long long n)
{
    if (n == 0)
        return "Zero";

    string result = "";

    // Crores
    if (n >= 10000000)
    {
        result += numToWords(n / 10000000) + "Crore ";
        n %= 10000000;
    }

    // Lakhs
    if (n >= 100000)
    {
        result += numToWords(n / 100000) + "Lakh ";
        n %= 100000;
    }

    // Thousands
    if (n >= 1000)
    {
        result += numToWords(n / 1000) + "Thousand ";
        n %= 1000;
    }

    // Remaining (Hundreds)
    if (n > 0)
    {
        result += numToWords(n);
    }

    return result;
}

int main()
{
    long long num;
    cout << "Enter number: ";
    cin >> num;

    if (num < 0 || num > 1000000000)
    {
        cout << "Out of range (0 to 100 Crores)" << endl;
        return 0;
    }

    cout << "In words    : " << convertToWords(num) << endl;

    return 0;
}
