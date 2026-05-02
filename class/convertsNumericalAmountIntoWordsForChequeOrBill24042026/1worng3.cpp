#include <iostream>
#include <string>
#include <cmath>      // for powl, but we avoid floating point; use integer math
#include <algorithm>
using namespace std;

// ---------- Word conversion functions (same as before) ----------
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

string convertRupees(long long n); // forward decl

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

// ---------- Exact conversion using integer arithmetic on fractional part ----------
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
        string rupees_str, fractional_str;
        if (dot == string::npos) {
            rupees_str = input;
            fractional_str = "";
        } else {
            rupees_str = input.substr(0, dot);
            fractional_str = input.substr(dot + 1);
        }

        // Remove leading zeros from rupees
        bool negative = false;
        if (!rupees_str.empty() && rupees_str[0] == '-') {
            negative = true;
            rupees_str = rupees_str.substr(1);
        }
        size_t nonZero = rupees_str.find_first_not_of('0');
        if (nonZero != string::npos)
            rupees_str = rupees_str.substr(nonZero);
        else
            rupees_str = "0";

        // Convert rupees to integer
        long long rupees = stoll(rupees_str);

        // --- Handle fractional part exactly ---
        int paise = 0;
        if (!fractional_str.empty()) {
            // Remove trailing zeros? Actually we need all digits for exact fraction.
            // Convert fractional string to a big integer (using unsigned long long; assume fits).
            // Let D = number of digits.
            unsigned long long frac_val = stoull(fractional_str);  // might overflow if > 2^64-1 digits? unlikely.
            size_t d = fractional_str.length();
            // Compute 10^d as unsigned long long; if d > 19, overflow – but input is limited by the user.
            unsigned long long pow10 = 1;
            for (size_t i = 0; i < d; ++i) pow10 *= 10;
            // paise = floor( (frac_val * 100) / pow10 )
            unsigned long long paise_ull = (frac_val * 100) / pow10;
            paise = (int)(paise_ull % 100);   // ensure within 0-99
            // No carry to rupees because frac_val < pow10, so paise_ull < 100.
        }
	cout <<"\nrupees -> "<<rupees<<endl;
	cout <<"paise -> "<<paise<<endl;
        // Additional safety: if paise >= 100 (should not happen, but just in case)
        if (paise >= 100) {
            rupees += paise / 100;
            paise = paise % 100;
        }

        // Build the word representation
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
