#include <iostream>
#include <string>
using namespace std;

void stringToArray(string aa, int a[], int n)
{
    // Initialize all elements to 0 first
    for(int i = 0; i < n; i++)
    {
        a[i] = 0;
    }
    
    // Place digits from rightmost position (units place)
    int len = aa.length();
    for(int i = 0; i < len; i++)
    {
        a[n - len + i] = aa[i] - '0';
    }
}

void printArray(int s[], int n)
{
    // Skip leading zeros
    int start = 0;
    while(start < n && s[start] == 0)
    {
        start++;
    }
    
    // If all zeros, print 0
    if(start == n)
    {
        cout << "0";
    }
    else
    {
        for(int i = start; i < n; i++)
        {
            cout << s[i];
        }
    }
    cout << endl;
}

int main()
{
    int n, sa, sb;
    string aa, bb;
    
    cout << "Enter number a: ";
    cin >> aa;
    
    cout << "Enter number b: ";
    cin >> bb;
    
    sa = aa.length();
    sb = bb.length();
    
    // Determine maximum length
    if(sa > sb)
        n = sa;
    else
        n = sb;
    
    // Add one extra position for possible carry at the end
    int a[n+1], b[n+1], c[n+1], s[n+1];
    
    // Initialize carry array to 0
    for(int i = 0; i <= n; i++)
    {
        c[i] = 0;
    }
    
    // Convert strings to arrays (right-aligned)
    stringToArray(aa, a, n);
    stringToArray(bb, b, n);
    
    // Perform addition from right to left
    for(int i = n - 1; i >= 0; i--)
    {
        s[i] = a[i] + b[i] + c[i];
        
        if(s[i] >= 10)
        {
            s[i] = s[i] - 10;
            c[i - 1] = 1;
        }
    }
    
    // Handle final carry at position -1 (store at beginning)
    s[-1] = c[-1];  // This is still problematic - needs proper handling
    
    cout << "Sum: ";
    
    // Print result (handle carry at the beginning)
    if(c[-1] == 1)
        cout << "1";
    
    printArray(s, n);
    
    return 0;
}
