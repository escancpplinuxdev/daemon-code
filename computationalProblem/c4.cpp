#include <iostream>
#include <string>

using namespace std;

const string ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
                       "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
const string tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

// Convert numbers 1-99 to words
string convertLessThanHundred(int n) {
    if (n < 20) return ones[n];
    int ten = n / 10;
    int one = n % 10;
    string result = tens[ten];
    if (one) result += " " + ones[one];
    return result;
}

// Convert numbers 1-999 to words
string convertLessThanThousand(int n) {
    if (n == 0) return "";
    if (n < 100) return convertLessThanHundred(n);
    int hundred = n / 100;
    int remainder = n % 100;
    string result = ones[hundred] + " Hundred";
    if (remainder) result += " " + convertLessThanHundred(remainder);
    return result;
}

// Convert any number up to 100 Crores (1,000,000,000) to words
string convertNumberToWords(long long n) {
    if (n == 0) return "Zero";
    
    string result;
    
    // Crores (10 million)
    long long crorePart = n / 10000000;
    if (crorePart > 0) {
        result += convertLessThanThousand(crorePart) + " Crore";
        n %= 10000000;
        if (n > 0) result += " ";
    }
    
    // Lakhs (100 thousand)
    long long lakhPart = n / 100000;
    if (lakhPart > 0) {
        result += convertLessThanThousand(lakhPart) + " Lakh";
        n %= 100000;
        if (n > 0) result += " ";
    }
    
    // Thousands
    long long thousandPart = n / 1000;
    if (thousandPart > 0) {
        result += convertLessThanThousand(thousandPart) + " Thousand";
        n %= 1000;
        if (n > 0) result += " ";
    }
    
    // Remaining hundreds and below
    if (n > 0) {
        if (n < 100)
            result += convertLessThanHundred(n);
        else
            result += convertLessThanThousand(n);
    }
    
    return result;
}

int main() {
    long long num;
    cout << "Enter a number (up to 100 Crores): ";
    cin >> num;
    
    if (num < 0) {
        cout << "Negative ";
        num = -num;
    }
    
    cout << convertNumberToWords(num) << endl;
    
    return 0;
}
