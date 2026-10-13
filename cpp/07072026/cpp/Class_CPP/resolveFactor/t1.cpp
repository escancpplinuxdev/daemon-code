#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <cctype>
using namespace std;

// -------------------------------------------------------------------
// One term: coefficient * var^power * var^power * ... * group^power
// -------------------------------------------------------------------
struct Term
{
    int coefficient = 1;
    map<string, int> vars;   // "a" -> 3, "b" -> 1
    string group;            // "(a+b)" if present
    int    groupPower = 0;
};

// -------------------------------------------------------------------
// Parse a single term like "2a^3b" or "c(a+b)" or "ap"
// -------------------------------------------------------------------
Term parseTerm(const string &raw)
{
    Term t;
    string s;
    for (char c : raw) if (!isspace(c)) s += c;

    size_t i = 0;

    // --- coefficient ---
    string num;
    while (i < s.size() && isdigit(s[i]))
    {
        num += s[i++];
    }
    if (!num.empty()) t.coefficient = stoi(num);

    // --- group like (a+b) ---
    size_t open = s.find('(');
    if (open != string::npos)
    {
        size_t close = s.find(')', open);
        t.group = s.substr(open, close - open + 1);
        t.groupPower = 1;
        s = s.substr(0, open);   // only vars before the group
    }

    // --- variables like a^3b or ap ---
    while (i < s.size())
    {
        if (!isalpha(s[i])) { i++; continue; }
        string var(1, s[i++]);
        int pow = 1;
        if (i < s.size() && s[i] == '^')
        {
            i++;
            string p;
            while (i < s.size() && isdigit(s[i])) p += s[i++];
            if (!p.empty()) pow = stoi(p);
        }
        t.vars[var] = pow;
    }

    return t;
}

// -------------------------------------------------------------------
// Split "2a^3b - 4a^2b^2" into terms with their signs
// -------------------------------------------------------------------
vector<Term> parseExpression(const string &line)
{
    vector<Term> terms;

    string s;
    for (char c : line) if (!isspace(c)) s += c;

    // split by + and -
    size_t start = 0;
    int sign = 1;

    if (!s.empty() && s[0] == '-') { sign = -1; start = 1; }

    for (size_t i = start; i <= s.size(); i++)
    {
        if (i == s.size() || s[i] == '+' || s[i] == '-')
        {
            if (i > start)
            {
                string piece = s.substr(start, i - start);
                Term t = parseTerm(piece);
                t.coefficient *= sign;
                terms.push_back(t);
            }
            if (i < s.size())
            {
                sign = (s[i] == '+') ? 1 : -1;
                start = i + 1;
            }
        }
    }
    return terms;
}

// -------------------------------------------------------------------
// Format a single term as a string
// -------------------------------------------------------------------
string formatTerm(const Term &t, bool first)
{
    string s;

    int coef = t.coefficient;
    bool hasVars = !t.vars.empty() || t.groupPower > 0;

    // sign
    if (first)
    {
        if (coef < 0) s += "-";
    }
    else
    {
        s += (coef < 0) ? " - " : " + ";
    }
    coef = abs(coef);

    // coefficient number (skip if it's 1 and there are vars)
    if (coef != 1 || !hasVars) s += to_string(coef);

    // variables
    for (auto &kv : t.vars)
    {
        if (kv.second <= 0) continue;
        s += kv.first;
        if (kv.second > 1) s += "^" + to_string(kv.second);
    }

    // group
    if (t.groupPower > 0)
    {
        s += t.group;
        if (t.groupPower > 1) s += "^" + to_string(t.groupPower);
    }

    return s;
}

// -------------------------------------------------------------------
// THE FACTORIZER
// -------------------------------------------------------------------
string factorExpression(const vector<Term> &terms)
{
    if (terms.empty()) return "";

    // 1) GCD of all coefficients
    int g = abs(terms[0].coefficient);
    for (size_t i = 1; i < terms.size(); i++)
    {
        g = gcd(g, abs(terms[i].coefficient));
    }
    if (g == 0) g = 1;

    // 2) Common variables (in all terms, min power)
    map<string, int> commonVars;
    for (auto &kv : terms[0].vars)
    {
        string name = kv.first;
        int minPow = kv.second;
        bool inAll = true;

        for (size_t i = 1; i < terms.size(); i++)
        {
            auto it = terms[i].vars.find(name);
            if (it == terms[i].vars.end()) { inAll = false; break; }
            minPow = min(minPow, it->second);
        }
        if (inAll && minPow > 0) commonVars[name] = minPow;
    }

    // 3) Common group
    string commonGroup;
    int commonGroupPower = 0;

    if (!terms[0].group.empty() && terms[0].groupPower > 0)
    {
        commonGroup = terms[0].group;
        commonGroupPower = terms[0].groupPower;
        bool inAll = true;

        for (size_t i = 1; i < terms.size(); i++)
        {
            if (terms[i].group != commonGroup || terms[i].groupPower <= 0)
            {
                inAll = false;
                break;
            }
            commonGroupPower = min(commonGroupPower, terms[i].groupPower);
        }
        if (!inAll) { commonGroup.clear(); commonGroupPower = 0; }
    }

    // 4) Build common factor string
    string out;
    bool wroteCommon = false;

    if (g > 1)
    {
        out += to_string(g);
        wroteCommon = true;
    }

    for (auto &kv : commonVars)
    {
        out += kv.first;
        if (kv.second > 1) out += "^" + to_string(kv.second);
        wroteCommon = true;
    }

    if (commonGroupPower > 0)
    {
        out += commonGroup;
        if (commonGroupPower > 1) out += "^" + to_string(commonGroupPower);
        wroteCommon = true;
    }

    // If nothing common at all
    if (!wroteCommon)
    {
        string s;
        for (size_t i = 0; i < terms.size(); i++)
        {
            s += formatTerm(terms[i], i == 0);
        }
        return s;
    }

    // 5) Reduce each term
    out += "(";
    for (size_t i = 0; i < terms.size(); i++)
    {
        Term r;
        r.coefficient = terms[i].coefficient / g;

        for (auto &kv : terms[i].vars)
        {
            int remove = 0;
            auto it = commonVars.find(kv.first);
            if (it != commonVars.end()) remove = it->second;
            int newPow = kv.second - remove;
            if (newPow > 0) r.vars[kv.first] = newPow;
        }

        if (terms[i].group == commonGroup && commonGroupPower > 0)
        {
            r.group = terms[i].group;
            r.groupPower = terms[i].groupPower - commonGroupPower;
        }
        else
        {
            r.group = terms[i].group;
            r.groupPower = terms[i].groupPower;
        }

        out += formatTerm(r, i == 0);
    }
    out += ")";
    return out;
}

// -------------------------------------------------------------------
// MAIN: read lines from keyboard
// -------------------------------------------------------------------
int main()
{
    cout << "Enter expressions (one per line). Type 'quit' to stop.\n\n";

    string line;
    while (true)
    {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line == "quit" || line == "exit") break;
        if (line.empty()) continue;

        vector<Term> terms = parseExpression(line);
        string factored = factorExpression(terms);

        cout << "  " << line << "  =  " << factored << "\n\n";
    }
    return 0;
}
