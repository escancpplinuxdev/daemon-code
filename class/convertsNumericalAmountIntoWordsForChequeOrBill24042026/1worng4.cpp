#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// ---------- Word conversion functions (exact same as before) ----------
string oneToNineteen(int n) 
{
    const char* words[] = {"", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN",
                           "EIGHT", "NINE", "TEN", "ELEVEN", "TWELVE", "THIRTEEN",
                           "FOURTEEN", "FIFTEEN", "SIXTEEN", "SEVENTEEN", "EIGHTEEN", "NINETEEN"};
    if (n >= 1 && n <= 19) return words[n];
    return "";
}

string tens(int n) 
{
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

string belowThousand(int n) 
{
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

string aboveCrores(long long n) 
{
    if (n < 10000000) return convertRupees(n);
    long long crorePart = n / 10000000;
    long long remainder = n % 10000000;
    string result = aboveCrores(crorePart) + " CRORE";
    if (remainder > 0) result += " " + convertRupees(remainder);
    return result;
}

string convertRupees(long long n) 
{
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

string convertPaise(int p) 
{
    if (p == 0) return "";
    if (p >= 1 && p <= 19) return oneToNineteen(p);
    else return tens(p);
}

// ---------- Main function: exact string parsing with rounding ----------
int main() 
{
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
        string rupees_str, frac_str;
        if (dot == string::npos) {
            rupees_str = input;
            frac_str = "";
        } else {
            rupees_str = input.substr(0, dot);
            frac_str = input.substr(dot + 1);
        }

        // Handle negative sign
        bool negative = false;
        if (!rupees_str.empty() && rupees_str[0] == '-') {
            negative = true;
            rupees_str = rupees_str.substr(1);
        }
        // Remove leading zeros
        size_t nonZero = rupees_str.find_first_not_of('0');
        if (nonZero != string::npos) rupees_str = rupees_str.substr(nonZero);
        else rupees_str = "0";

        long long rupees = stoll(rupees_str);

        // ----- Process fractional part with rounding -----
        int paise = 0;
        int rounding_extra = 0;
        if (!frac_str.empty()) {
            // Extract first three digits (if exist) – third digit decides rounding
            int first_two = 0, third_digit = 5; // default round up if third digit absent? Actually no.
            if (frac_str.size() >= 2) {
                first_two = stoi(frac_str.substr(0, 2));
                if (frac_str.size() >= 3) {
                    third_digit = frac_str[2] - '0';
                } else {
                    third_digit = 0; // no third digit → no rounding
                }
            } else if (frac_str.size() == 1) {
                first_two = stoi(frac_str.substr(0, 1)) * 10;
                third_digit = 0;
            } else {
                first_two = 0;
                third_digit = 0;
            }
            paise = first_two;
            if (third_digit >= 5) {
                paise++;   // round up paise
            }
        }

        // Handle paise overflow (e.g., 99 + 1 = 100)
        if (paise >= 100) {
            rupees++;
            paise -= 100;
        }

        // Build words
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
