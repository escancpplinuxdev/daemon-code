#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Arrays to store words for numbers
string units[] = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", 
                   "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" };

string tens[] = { "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };

// Helper function to convert numbers less than 100 to words
string convertLessThanHundred(int n) {
    if (n < 20) {
        return units[n];
    }
    return tens[n / 10] + (n % 10 != 0 ? " " + units[n % 10] : "");
}

// Function to convert a number to Indian Currency Words
string numberToWords(long long n) {
    if (n == 0) return "Zero";

    string res = "";

    // Handle Crores (1,00,00,000)
    if (n >= 10000000) {
        res += numberToWords(n / 10000000) + " Crore ";
        n %= 10000000;
    }

    // Handle Lakhs (1,00,000)
    if (n >= 100000) {
        res += convertLessThanHundred(n / 100000) + " Lakh ";
        n %= 100000;
    }

    // Handle Thousands (1,000)
    if (n >= 1000) {
        res += convertLessThanHundred(n / 1000) + " Thousand ";
        n %= 1000;
    }

    // Handle Hundreds (100)
    if (n >= 100) {
        res += units[n / 100] + " Hundred ";
        n %= 100;
    }

    // Handle remaining tens and units
    if (n > 0) {
        if (res != "") res += "and ";
        res += convertLessThanHundred(n);
    }

    return res;
}

int main() {
    long long num;
    cout << "Enter a number (up to 100 Crores): ";
    if (!(cin >> num)) {
        cout << "Invalid input!" << endl;
        return 1;
    }

    if (num < 0 || num > 1000000000) {
        cout << "Please enter a positive number up to 1,00,00,00,000." << endl;
    } else {
        cout << "Output: " << numberToWords(num) << endl;
    }

    return 0;
}
