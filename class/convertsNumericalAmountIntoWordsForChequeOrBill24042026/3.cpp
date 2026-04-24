#include <iostream>
#include <string>
#include <cmath>
#include <limits>   // for numeric_limits
using namespace std;

// ---------- Conversion functions (same as before) ----------
/*
string oneToNineteen(int n) {
    const char* words[] = {"", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
                           "EIGHT", "NINE", "TEN", "ELEVEN", "TWELVE", "THIRTEEN",
                           "FOURTEEN", "FIFTEEN", "SIXTEEN", "SEVENTEEN", "EIGHTEEN", "NINETEEN"};
    if (n >= 1 && n <= 19) return words[n];
    return "";
}

string tens(int n) {
    const char* tensWords[] = {"", "", "TWENTY", "THIRTY", "FORTY", "FIFTY",
                               "SIXTY", "SEVENTY", "EIGHTY", "NINETY"};
    if (n >= 20 && n <= 99) {
        int t = n / 10;
        int r = n % 10;
        if (r == 0) return tensWords[t];
        else return string(tensWords[t]) + " " + oneToNineteen(r);
    }
    return "";
}
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Word mappings for units and tens
string units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", 
                   "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
string tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

// Helper to handle numbers under 100
string convertToWords(long n) 
{
    if (n < 20) return units[n];
    return tens[n / 10] + (n % 10 != 0 ? " " + units[n % 10] : "");
}

string belowThousand(int n) {
    if (n == 0) return "";
    string result;
    if (n >= 100) {
        result += convertToWords(n / 100) + " HUNDRED";
        n %= 100;
        if (n > 0) result += " ";
    }
    if (n >= 20) result += tens[n/10];
    else if (n > 0) result += convertToWords(n);
    return result;
}

string convertRupees(long long n) {
    if (n == 0) return "ZERO";
    string result;
    if (n >= 10000000) {
        long long crorePart = n / 10000000;
        result += belowThousand(crorePart) + " CRORE";
        n %= 10000000;
        if (n > 0) result += " ";
    }
    if (n >= 100000) {
        long long lakhPart = n / 100000;
        result += belowThousand(lakhPart) + " LAKH";
        n %= 100000;
        if (n > 0) result += " ";
    }
    if (n >= 1000) {
        long long thousandPart = n / 1000;
        result += belowThousand(thousandPart) + " THOUSAND";
        n %= 1000;
        if (n > 0) result += " ";
    }
    if (n > 0) result += belowThousand(n);
    return result;
}

string convertPaise(int p) {
    if (p == 0) return "";
    if (p >= 1 && p <= 19) return convertToWords(p);
    else return tens[p/10];
}

string amountToWords(double amount) {
    if (amount < 0) return "MINUS " + amountToWords(-amount);
    if (amount == 0) return "ZERO RUPEE ONLY";

    long long rupees = (long long)amount;
    int paise = round((amount - rupees) * 100);
    if (paise >= 100) {
        rupees += paise / 100;
        paise = paise % 100;
    }

    string result = convertRupees(rupees) + " RUPEE";
    if (rupees > 1) result += "S";
    if (paise > 0) result += " AND " + convertPaise(paise) + " PAISE";
    result += " ONLY";
    return result;
}

// ---------- Main loop with input validation ----------
int main() {
    double amount;
    cout << "Enter 0 (Zero) to end the loop.\n";
    while (true) {
        cout << "Enter amount (e.g., 1234.56): ";
        if (!(cin >> amount)) {
            // Invalid input (letters, multiple signs, etc.)
            cout << "Invalid input! Please enter a numeric value.\n";
            cin.clear();  // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            continue;
        }
        if (amount == 0) {
		cout<<"ZERO RUPEES ONLY\n";
            cout << "Exiting loop. Goodbye!\n";
            break;
        }
        cout << amountToWords(amount) << "\n";
    }
    return 0;
}
