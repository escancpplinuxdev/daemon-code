#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

void initilizeArrayToZero(int a[], int n)
{
    for (int i = 0; i <= n; i++)
    {
        a[i] = 0;
    }
}

void printArray(int s[], int n)
{
    // Find first non-zero digit
    int start = 0;
    while (start <= n && s[start] == 0)
    {
        start++;
    }
    
    if (start > n)
    {
        cout << "0";
    }
    else
    {
        for (int i = start; i <= n; i++)
        {
            cout << s[i];
        }
    }
    cout << endl;
}

int main()
{
    int count;
    vector<string> numbers;
    
    cout << "Enter total number of addition elements : ";
    cin >> count;
    cout << "Enter number for element \n";
    
    for (int i = 0; i < count; i++)
    {
        string num;
        cout << "num[" << i + 1 << "] : ";
        cin >> num;
        numbers.push_back(num);
    }

    // Start with sum = "0"
    string sum = "0";
    
    // Add each number one by one
    for (int j = 0; j < count; j++)
    {
        string currentNum = numbers[j];
        string a = sum;
        string b = currentNum;
        
        // Make both strings same length by padding with leading zeros
        int maxLen = max(a.length(), b.length());
        while (a.length() < maxLen) a = "0" + a;
        while (b.length() < maxLen) b = "0" + b;
        
        string result = "";
        int carry = 0;
        
        cout << "\n========== Adding: " << currentNum << " ==========" << endl;
        cout << "Current sum: " << sum << endl;
        
        // Add digit by digit from right to left
        for (int i = a.length() - 1; i >= 0; i--)
        {
            int digitA = a[i] - '0';
            int digitB = b[i] - '0';
            int total = digitA + digitB + carry;
            
            // Convert total to string
            string totalStr = to_string(total);
            
            // Get last digit
            result = totalStr.back() + result;
            
            // Get carry (all digits except last)
            if (totalStr.length() > 1)
            {
                carry = stoi(totalStr.substr(0, totalStr.length() - 1));
            }
            else
            {
                carry = 0;
            }
            
            cout << "  Column " << (a.length() - i) << ": " << digitA << " + " << digitB;
            cout << " + carry " << (i == a.length()-1 ? 0 : carry) << " = " << total;
            cout << " → write " << totalStr.back();
            if (carry > 0) cout << ", carry " << carry;
            cout << endl;
        }
        
        // Add final carry if any
        if (carry > 0)
        {
            result = to_string(carry) + result;
            cout << "  Final carry: " << carry << endl;
        }
        
        sum = result;
        cout << "Result after adding " << currentNum << ": " << sum << endl;
    }
    
    // Print final result
    cout << "\n========================================" << endl;
    cout << "FINAL SUM: " << sum << endl;
    cout << "========================================" << endl;
    
    return 0;
}
