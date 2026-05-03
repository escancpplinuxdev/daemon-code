#include <iostream>
#include <string>
using namespace std;

// Convert string to array with proper padding (most significant digits at left)
void stringToArray(string num, int arr[], int n) {
    int len = num.length();
    int offset = n - len;
    
    // Fill leading zeros
    for (int i = 0; i < offset; i++) {
        arr[i] = 0;
    }
    
    // Fill the actual digits
    for (int i = offset; i < n; i++) {
        arr[i] = num[i - offset] - '0';
    }
}

// Print array without leading zeros
void printArray(int arr[], int n) {
    int i = 0;
    // Skip leading zeros
    while (i < n && arr[i] == 0) {
        i++;
    }
    // If all zeros, print at least one zero
    if (i == n) {
        cout << "0";
    }
    for (; i < n; i++) {
        cout << arr[i];
    }
}

int main() {
    string aa, bb;
    cout << "Enter number a: ";
    cin >> aa;
    cout << "Enter number b: ";
    cin >> bb;

    int sa = aa.length();
    int sb = bb.length();
    
    cout << "Length of a: " << sa << endl;
    cout << "Length of b: " << sb << endl;

    // Size for result (max length + 1 for possible carry)
    int n = (sa > sb ? sa : sb);
    int result_n = n + 1;  // Extra space for carry at the front

    // Dynamically allocate arrays
    int* a = new int[n];
    int* b = new int[n];
    int* c = new int[result_n](); // carry array (initialised to zero)
    int* s = new int[result_n](); // sum array (initialised to zero)

    // Convert strings to arrays with padding
    stringToArray(aa, a, n);
    stringToArray(bb, b, n);

    cout << "Array a (padded): ";
    printArray(a, n);
    cout << endl;

    cout << "Array b (padded): ";
    printArray(b, n);
    cout << endl;

    // Perform addition from rightmost digit (index n-1 down to 0)
    for (int i = n - 1; i >= 0; i--) {
        s[i + 1] = a[i] + b[i] + c[i + 1];  // s has one extra slot at front
        if (s[i + 1] >= 10) {
            s[i + 1] -= 10;
            c[i] = 1;  // carry to next left digit
        }
    }

    // Result is in s array (index 0 to n)
    cout << "Sum: ";
    printArray(s, result_n);
    cout << endl;

    // Clean up dynamic memory
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] s;

    return 0;
}
