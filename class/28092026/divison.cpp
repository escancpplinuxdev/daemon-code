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

    string dvd = to_string(dividend);
    string dvs = to_string(divisor);

    struct Step
    {
        int       q;        // quotient digit chosen at this step
        long long value;    // number brought down at this step
        long long product;  // divisor * q
        long long rem;      // value - product
        int       col;      // index of the rightmost dividend digit used
    };

    vector<Step> steps;

    long long cur = 0;
    for (int i = 0; i < (int)dvd.size(); i++)
    {
        cur = cur * 10 + (dvd[i] - '0');

        int q = 0;
        while ((q + 1) * divisor <= cur)
        {
            q++;
        }

        steps.push_back({ q, cur, (long long)q * divisor, cur - (long long)q * divisor, i });

        cur -= (long long)q * divisor;
    }

    string quotient;
    for (auto &s : steps)
    {
        quotient += to_string(s.q);
    }
    if (quotient.empty())
    {
        quotient = "0";
    }

    int lhsWidth  = max((int)dvs.size(), (int)quotient.size()) + 1;
    int startCol  = lhsWidth + 2;
    int fullWidth = startCol + (int)dvd.size();

    auto blank = [&]()
    {
        return string(fullWidth, ' ');
    };

    // 1) Quotient line, right-aligned over the dividend
    {
        string line = blank();
        int end   = startCol + (int)dvd.size();
        int start = end - (int)quotient.size();
        for (int i = 0; i < (int)quotient.size(); i++)
        {
            line[start + i] = quotient[i];
        }
        cout << line << "\n";
    }

    // 2) Divisor ) Dividend
    {
        string line = blank();
        for (int i = 0; i < (int)dvs.size(); i++)
        {
            line[lhsWidth - (int)dvs.size() + i] = dvs[i];
        }
        line[lhsWidth] = ')';
        for (int i = 0; i < (int)dvd.size(); i++)
        {
            line[startCol + i] = dvd[i];
        }
        cout << line << "\n";
    }

    // 3) Long-division bar
    {
        string line = blank();
        line[lhsWidth - 1] = '|';
        for (int i = startCol; i < startCol + (int)dvd.size(); i++)
        {
            line[i] = '_';
        }
        cout << line << "\n";
    }

    // 4) One block per step
    for (size_t s = 0; s < steps.size(); s++)
    {
        const Step &st = steps[s];
        int col = startCol + st.col;

        auto putNum = [&](long long num)
        {
            string line = blank();
            string ns = to_string(num);
            int c = col - (int)ns.size() + 1;
            if (c < 0)
            {
                c = 0;
            }
            for (int i = 0; i < (int)ns.size(); i++)
            {
                line[c + i] = ns[i];
            }
            cout << line << "\n";
        };

        // value being divided (no minus in margin)
        putNum(st.value);

        // product being subtracted (one '-' in the margin)
        {
            string line = blank();
            string ps = to_string(st.product);
            int c = col - (int)ps.size() + 1;
            if (c < 0)
            {
                c = 0;
            }
            for (int i = 0; i < (int)ps.size(); i++)
            {
                line[c + i] = ps[i];
            }
            line[lhsWidth - 1] = '-';
            cout << line << "\n";
        }

        // separator
        {
            string line = blank();
            string ps = to_string(st.product);
            int c = col - (int)ps.size() + 1;
            if (c < 0)
            {
                c = 0;
            }
            for (int i = c; i <= col; i++)
            {
                line[i] = '-';
            }
            cout << line << "\n";
        }

        // remainder
        putNum(st.rem);
    }

    cout << "\n";
}

int main()
{
    printLongDivision(156, 4);
    printLongDivision(17, 5);
    printLongDivision(1234, 12);
    printLongDivision(100, 25);
    return 0;
}
