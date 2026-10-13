#include <iostream>
#include <string>
#include <vector>
using namespace std;

void printLongDivision(long long dividend, long long divisor)
{
    if (divisor == 0)
    {
        cout << "Error: division by zero\n";
        return;
    }
    cout << "dividend = " << dividend << "\n";
    cout << "divisor  = " << divisor  << "\n";
    cout << "\n\n";

    string dvd = to_string(dividend);
    string dvs = to_string(divisor);

    cout << "dvd.size() = " << (int)dvd.size() << "\n";

    // ---- PASS 1: integer part of the quotient ----
    string quotient;
    long long cur = 0;
    for (int i = 0; i < (int)dvd.size(); i++)
    {
        if (i > 0)
        {
            cout << cur << "\t+ " << (dvd[i] - '0') << "\n";
        }

        cur = cur * 10 + (dvd[i] - '0');

        cout << "current = " << cur << "\n";

        int q = 0;
        while ((q + 1) * divisor <= cur)
        {
            q++;
        }

        quotient += to_string(q);

        cout << "product  = " << (long long)q * divisor << "\n";
        cout << cur << "\n- \n" << (long long)q * divisor << "\n__\n";

        cur -= (long long)q * divisor;
    }
    if (quotient.empty())
    {
        quotient = "0";
    }

    cout << "\n\n";
    cout << "remainder = " << cur << "\n";
    cout << "quotient  = " << quotient << "\n";

    // ---- PASS 2: decimal expansion ----
    // If remainder isn't zero, keep going with a decimal point
    if (cur != 0)
    {
        cout << "\n--- decimal expansion ---\n";
        quotient += ".";

        const int MAX_DECIMAL_DIGITS = 7;
        int decimalCount = 0;

        while (cur != 0 && decimalCount < MAX_DECIMAL_DIGITS)
        {
            // Bring down a zero
            cout << "\nbring down 0\n";
            cur = cur * 10;

            cout << "current = " << cur << "\n";

            int q = 0;
            while ((q + 1) * divisor <= cur)
            {
                q++;
            }

            quotient += to_string(q);
            decimalCount++;

            cout << "product  = " << (long long)q * divisor << "\n";
            cout << cur << "\n- \n" << (long long)q * divisor << "\n__\n";

            cur -= (long long)q * divisor;

            cout << "remainder = " << cur
                 << "   (decimal digits so far = " << decimalCount << ")\n";
        }

        if (cur == 0)
        {
            cout << "\nremainder became 0 after "
                 << decimalCount << " decimal digit(s).\n";
        }
        else
        {
            cout << "\nstopped after " << MAX_DECIMAL_DIGITS
                 << " decimal digits (remainder still "
                 << cur << ").\n";
        }
    }

    cout << "\n\nFINAL quotient = " << quotient << "\n";
    cout << "FINAL remainder = " << cur << "\n";
    cout << "\n";
}

int main()
{
/*    printLongDivision(174321, 6);   // terminates exactly (no decimals)
    printLongDivision(156, 4);      // terminates exactly
    printLongDivision(1, 3);        // 0.3333333... (stops after 7)
*///  printLongDivision(22, 7);       // 3.1428571... (stops after 7)
    printLongDivision(21, 7);       // 3.1428571... (stops after 7)
    return 0;
}

/*
How the Decimal Logic Works
1. When to start the decimal part
cpp

if (cur != 0)
{
    quotient += ".";
    ...
}

After the integer division, if the remainder is not zero, there's more to divide — so we append a . to the quotient and continue.

If the remainder is zero (like 156 ÷ 4 = 39), we skip the whole decimal block.
2. The stopping condition
cpp

while (cur != 0 && decimalCount < MAX_DECIMAL_DIGITS)

Two ways to stop:

    cur == 0 → the division came out exactly (e.g., 1 ÷ 8 = 0.125)

    decimalCount == 7 → we hit our digit limit (e.g., 1 ÷ 3 = 0.3333333)

3. Bringing down zeros
cpp

cur = cur * 10;

In the integer part, we brought down digits of the dividend. Past the decimal point, the dividend is all zeros (since 156 = 156.00000...). So we just multiply by 10 each time — that's "bringing down a zero."
4. Same q-finding logic

The while ((q+1) * divisor <= cur) q++; loop is identical to the integer part. Nothing changes — long division doesn't care where the decimal point is.
Example Runs
Example 1: 174321 ÷ 6 (terminates exactly)

Remainder after integer part is 3, not zero — so we'd expect decimals... but wait, let's check:

    174321 ÷ 6 = 29053.5 exactly.

So the decimal block runs once:
text

bring down 0
current = 30
product  = 30
30
- 
30
__
remainder = 0   (decimal digits so far = 1)

remainder became 0 after 1 decimal digit(s).

FINAL quotient = 029053.5
FINAL remainder = 0

Wait — but 29053.5 is .5 exactly, so it stops after 1 decimal digit. But you also said "or after . seven number" — meaning either stop when remainder is 0 or when you've written 7 decimal digits. Both conditions are checked.
Example 2: 156 ÷ 4 (terminates in the integer part)

Remainder after integer division is 0, so the decimal block never runs:
text

FINAL quotient = 039
FINAL remainder = 0

Example 3: 1 ÷ 3 (never terminates)

Remainder stays nonzero forever. We stop after 7 digits:
text

bring down 0
current = 10
product  = 9
10
- 
9
__
remainder = 1   (decimal digits so far = 1)

bring down 0
current = 10
product  = 9
...
remainder = 1   (decimal digits so far = 2)
...
remainder = 1   (decimal digits so far = 7)

stopped after 7 decimal digits (remainder still 1).

FINAL quotient = 0.3333333
FINAL remainder = 1

Example 4: 22 ÷ 7 (famous repeating decimal)

7 digits after .:
text

FINAL quotient = 3.1428571
FINAL remainder = 3

(True value is 3.142857142857... — we cut it off at 7 digits.)
What Changed vs. Your Original
Before	After
Only integer quotient	Integer + decimal expansion
Stops when digits of dividend run out	Stops when remainder is 0 OR 7 decimals written
No . in output	Appends . before decimals
quotient = "029053"	quotient = "029053.5" (or whatever)

Everything else — the q-finding loop, the bring-down logic, the product/subtract — is exactly the same. That's the beauty of long division: the algorithm is identical for the decimal part; you just bring down zeros instead of dividend digits.
Optional Tweaks
Change the decimal limit

Just change:
cpp

const int MAX_DECIMAL_DIGITS = 7;

to whatever you want.
Strip leading zeros in the integer part

If you want 029053 to print as 29053, add this after the integer pass:
cpp

size_t firstNonZero = quotient.find_first_not_of('0');
if (firstNonZero != string::npos && firstNonZero > 0)
{
    quotient = quotient.substr(firstNonZero);
}
if (quotient.empty())
{
    quotient = "0";
}

Do this before adding the ..
Show only until repeat (advanced)

For repeating decimals like 1/3, you could stop when the same (cur, q) pair appears again. That's a nice challenge if you want to go further — say the word and I'll add it.
One-Line Summary

    After the integer part, if the remainder is nonzero, append . and keep looping: bring down a zero (cur *= 10), find q the same way, subtract. Stop when cur == 0 or 7 decimal digits have been written.

*/
