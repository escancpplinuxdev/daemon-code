#include <iostream>
#include <string>
using namespace std;

// ─── Word Tables ────────────────────────────────────────────────────────────

string ones[] = {
    "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
    "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen",
    "Sixteen", "Seventeen", "Eighteen", "Nineteen"
};

string tens[] = {
    "", "", "Twenty", "Thirty", "Forty", "Fifty",
    "Sixty", "Seventy", "Eighty", "Ninety"
};

// ─── Convert numbers below 1000 to words ────────────────────────────────────

string belowThousand(int n) {
    if (n == 0) return "";

    string result = "";

    if (n >= 100) {
        result += ones[n / 100] + " Hundred";
        n %= 100;
        if (n > 0) result += " ";
    }

    if (n >= 20) {
        result += tens[n / 10];
        if (n % 10 != 0)
            result += " " + ones[n % 10];
    } else if (n > 0) {
        result += ones[n];
    }

    return result;
}

// ─── Main Converter (Indian System) ─────────────────────────────────────────
// Supports: up to 100,00,00,000  (100 Crore)

string numberToWords(long long n) {

    // ── Boundary check ───────────────────────────────────────────────────────
    if (n < 0)
        return "Negative numbers are not supported";
    if (n > 10000000000LL)           // > 100 Crore
        return "Number exceeds 100 Crore limit";
    if (n == 0)
        return "Zero";

    // ── Special exact case ───────────────────────────────────────────────────
    if (n == 10000000000LL)
        return "One Hundred Crore";

    string result = "";

    // ── Crore  (10,000,000) ──────────────────────────────────────────────────
    if (n >= 10000000) {
        long long crore = n / 10000000;
        result += belowThousand((int)crore) + " Crore";
        n %= 10000000;
        if (n > 0) result += " ";
    }

    // ── Lakh  (100,000) ──────────────────────────────────────────────────────
    if (n >= 100000) {
        int lakh = (int)(n / 100000);
        result += belowThousand(lakh) + " Lakh";
        n %= 100000;
        if (n > 0) result += " ";
    }

    // ── Thousand  (1,000) ────────────────────────────────────────────────────
    if (n >= 1000) {
        int thousand = (int)(n / 1000);
        result += belowThousand(thousand) + " Thousand";
        n %= 1000;
        if (n > 0) result += " ";
    }

    // ── Remainder < 1000 ────────────────────────────────────────────────────
    if (n > 0) {
        result += belowThousand((int)n);
    }

    return result;
}

// ─── Entry Point ─────────────────────────────────────────────────────────────

int main() {
    long long num;

    cout << "╔══════════════════════════════════════════╗" << endl;
    cout << "║    Number to Words  (upto 100 Crore)     ║" << endl;
    cout << "╚══════════════════════════════════════════╝" << endl;

    while (true) {
        cout << "\nEnter a number (0 to exit): ";
        cin >> num;

        if (num == 0) {
            cout << "Exiting... Goodbye!" << endl;
            break;
        }

        cout << "In Words : " << numberToWords(num) << endl;
    }

    return 0;
}
