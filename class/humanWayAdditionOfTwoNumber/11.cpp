#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

void initializeArrayToZero(int a[], int n)
{
	for (int i = 0; i <= n; i++)
	{
		a[i] = 0;
	}
}

void stringToArray(string num, int arr[], int n)
{
	int len = num.length();
	for (int i = len - 1, j = n; i >= 0; i--, j--)
	{
		arr[j] = num[i] - '0';
	}
}

void printArray(int s[], int n)
{
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
	// Three numbers to add
	string aa = "65";
	string bb = "55";
	string cc = "99";

	cout << "Number a : " << aa << endl;
	cout << "Number b : " << bb << endl;
	cout << "Number c : " << cc << endl;

	int sa = aa.length();  // 2
	int sb = bb.length();  // 2
	int sc = cc.length();  // 2

	// Find maximum length
	int n = sa;
	if (sb > n) n = sb;
	if (sc > n) n = sc;

	cout << "Maximum digits (n) = " << n << endl;

	// Create arrays (n+1 to accommodate carry at the end)
	int a[n + 2], b[n + 2], c[n + 2], sum[n + 2], carry[n + 2];

	// Initialize all arrays to zero
	initializeArrayToZero(a, n + 1);
	initializeArrayToZero(b, n + 1);
	initializeArrayToZero(c, n + 1);
	initializeArrayToZero(sum, n + 1);
	initializeArrayToZero(carry, n + 1);

	// Convert strings to arrays (right-aligned)
	stringToArray(aa, a, n);
	stringToArray(bb, b, n);
	stringToArray(cc, c, n);

	cout << "\nArray a (digits aligned to right): ";
	printArray(a, n);
	cout << "Array b (digits aligned to right): ";
	printArray(b, n);
	cout << "Array c (digits aligned to right): ";
	printArray(c, n);

	// Addition from rightmost to leftmost
	// Start from index n (units place) down to 0
	for (int i = n; i >= 0; i--)
	{
		cout<<"i = "<<i<<endl;
		cout<<"a[i] = "<<a[i]<<endl;
		cout<<"b[i] = "<<b[i]<<endl;
		cout<<"c[i] = "<<c[i]<<endl;
		cout<<"carry[i] = "<<carry[i]<<endl;
		// Add digits from all three numbers plus carry from previous column
		int total = a[i] + b[i] + c[i] + carry[i];

		// Store the units digit in sum array
		sum[i] = total % 10;
		cout<<"sum[i] = "<<sum[i]<<endl;

		// Store carry for the next column (to the left)
		if (i > 0)
		{
			carry[i - 1] = total / 10;
		}
		else
		{
			// For the leftmost position (i=0), if there's a carry, it becomes the new digit
			// We need to handle it separately because array index -1 doesn't exist
			if (total >= 10)
			{
				// Shift all sum digits right and add the carry at beginning
				// This is simpler: just note that we have a carry at the end
				// We'll handle it after the loop
			}
		}
		cout<<"carry[i] = "<<carry[i]<<endl;

	}

	// Check if there's a final carry after the loop
	if (carry[0] > 0)
	{
		// Shift all sum digits right by 1 and add carry at position 0
		for (int i = n; i >= 0; i--)
		{
			sum[i + 1] = sum[i];
		}
		sum[0] = carry[0];
		n++; // Increase the digit count
	}

	cout << "\nCarry array: ";
	printArray(carry, n);
	cout << "Sum array  : ";
	printArray(sum, n);

	cout << "\nFinal result: ";
	printArray(sum, n);

	cout << "\nVerification: " << aa << " + " << bb << " + " << cc 
		<< " = " << (65 + 55 + 99) << endl;

	return 0;
}
