#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// ---------- Conversion functions ----------
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

string belowThousand(int n) {
    if (n == 0) return "";
    string result;
    if (n >= 100) {
        result += oneToNineteen(n / 100) + " HUNDRED";
        n %= 100;
        if (n > 0) result += " ";
    }
    if (n >= 20) result += tens(n);
    else if (n > 0) result += oneToNineteen(n);
    return result;
}

string convertRupees(long long n); // forward declaration

string aboveCrores(long long n) {
    if (n < 10000000) return convertRupees(n);
    long long crorePart = n / 10000000;
    long long remainder = n % 10000000;
    string result = aboveCrores(crorePart) + " CRORE";
    if (remainder > 0) result += " " + convertRupees(remainder);
    return result;
}

string convertRupees(long long n) {
    if (n == 0) return "ZERO";
    string result;
    if (n >= 10000000) {
        long long crorePart = n / 10000000;
        result += aboveCrores(crorePart) + " CRORE";
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
    if (p >= 1 && p <= 19) return oneToNineteen(p);
    else return tens(p);
}

// ---------- Main function with string splitting ----------
int main() {
    string input;
    cout << "Enter 0 (Zero) to end the loop\n";
    while (true) {
        cout << "Enter amount (e.g., 1234.56): ";
        cin >> input;
        if (input == "0") {
            cout << "ZERO RUPEE ONLY\nExiting loop. Goodbye!\n";
            break;
        }

        // Split at decimal point
        size_t dot = input.find('.');
        string rupeesStr, paiseStr;

        if (dot == string::npos) {
            rupeesStr = input;
            paiseStr = "0";
        } else {
            rupeesStr = input.substr(0, dot);
            paiseStr = input.substr(dot + 1);
            // Keep exactly two digits for paise
            if (paiseStr.length() > 2) paiseStr = paiseStr.substr(0, 2);
            else if (paiseStr.length() == 1) paiseStr += "0";
            else if (paiseStr.empty()) paiseStr = "0";
        }

        // Handle negative sign
        bool negative = false;
        if (!rupeesStr.empty() && rupeesStr[0] == '-') {
            negative = true;
            rupeesStr = rupeesStr.substr(1);
        }
        // Remove leading zeros from rupees
        size_t nonZero = rupeesStr.find_first_not_of('0');
        if (nonZero != string::npos) rupeesStr = rupeesStr.substr(nonZero);
        else rupeesStr = "0";

        long long rupees = stoll(rupeesStr);
        int paise = stoi(paiseStr);

        // ----- Floating point precision fix (kept for compatibility, though not needed) -----
        if (paise >= 100) {
            rupees += paise / 100;
            paise = paise % 100;
        }
	
                cout<<"\nrupees -> "<<rupees<<endl;
                cout<<"paise-> "<<paise<<endl;


        // Build the result
        string result;
        if (negative) result = "MINUS ";

        if (rupees == 0 && paise == 0) {
            result += "ZERO RUPEE ONLY";
        } else {
            if (rupees > 0) {
                result += convertRupees(rupees) + " RUPEE";
                if (rupees > 1) result += "S";
            } else {
                result += "ZERO RUPEE";
            }
            if (paise > 0) {
                if (rupees > 0) result += " AND ";
                result += convertPaise(paise) + " PAISE";
            }
            result += " ONLY";
        }

        cout << input << "  :  " << result << "\n\n";
    }
    return 0;
}
