
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <cctype>
using namespace std;

struct Term
{
    int coefficient = 1;
    map<string, int> vars;
    string group;
    int    groupPower = 0;
};

// -------------------------------------------------------------------
// Parse one term like "2a^3b", "c(a+b)", "ap"
// -------------------------------------------------------------------
Term parseTerm(const string &raw)
{
    Term t;
    string s;
    for (char c : raw) if (!isspace(c)) s += c;

    // coefficient at the start
    size_t i = 0;
    string num;
    while (i < s.size() && isdigit(s[i])) num += s[i++];
    if (!num.empty()) t.coefficient = stoi(num);

    // group like (a+b)
    size_t open = s.find('(');
    if (open != string::npos)
    {
        size_t close = s.find(')', open);
        if (close != string::npos)
        {
            t.group = s.substr(open, close - open + 1);
            t.groupPower = 1;
            s = s.substr(0, open);
        }
    }

    // variables
    for (size_t k = 0; k < s.size(); k++)
    {
        if (!isalpha(s[k])) continue;
        string var(1, s[k]);
        int pow = 1;
        if (k + 1 < s.size() && s[k + 1] == '^')
        {
            size_t j = k + 2;
            string p;
            while (j < s.size() && isdigit(s[j])) p += s[j++];
            if (!p.empty()) pow = stoi(p);
            k = j - 1;
        }
        t.vars[var] = pow;
    }

    return t;
}

// -------------------------------------------------------------------
// Split "c(a+b) + d(a+b) + e(a+b)" into terms, respecting parentheses
// -------------------------------------------------------------------
vector<Term> parseExpression(const string &line)
{
    vector<Term> terms;

    string s;
    for (char c : line) if (!isspace(c)) s += c;

    size_t start = 0;
    int sign = 1;
    int depth = 0;

    if (!s.empty() && s[0] == '-') { sign = -1; start = 1; }

    for (size_t i = start; i <= s.size(); i++)
    {
        if (i < s.size())
        {
            if (s[i] == '(') depth++;
            else if (s[i] == ')') depth--;
        }

        bool atEnd = (i == s.size());
        bool atTopLevelSign = (!atEnd && depth == 0 && (s[i] == '+' || s[i] == '-'));

        if (atEnd || atTopLevelSign)
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
// Format a term
// -------------------------------------------------------------------
string formatTerm(const Term &t, bool first)
{
    string s;
    int coef = t.coefficient;
    bool hasVars = !t.vars.empty() || t.groupPower > 0;

    if (first)
    {
        if (coef < 0) s += "-";
    }
    else
    {
        s += (coef < 0) ? " - " : " + ";
    }
    coef = abs(coef);

    if (coef != 1 || !hasVars) s += to_string(coef);

    for (auto &kv : t.vars)
    {
        if (kv.second <= 0) continue;
        s += kv.first;
        if (kv.second > 1) s += "^" + to_string(kv.second);
    }

    if (t.groupPower > 0)
    {
        s += t.group;
        if (t.groupPower > 1) s += "^" + to_string(t.groupPower);
    }
    return s;
}

// -------------------------------------------------------------------
// The factorizer
// -------------------------------------------------------------------
string factorExpression(const vector<Term> &terms)
{
    if (terms.empty()) return "";

    int g = abs(terms[0].coefficient);
    for (size_t i = 1; i < terms.size(); i++)
    {
        g = gcd(g, abs(terms[i].coefficient));
    }
    if (g == 0) g = 1;

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

    string out;
    bool wroteCommon = false;

    if (g > 1) { out += to_string(g); wroteCommon = true; }

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

    if (!wroteCommon)
    {
        string s;
        for (size_t i = 0; i < terms.size(); i++)
        {
            s += formatTerm(terms[i], i == 0);
        }
        return s;
    }

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
// Main
// -------------------------------------------------------------------
int main()
{
    cout << "Enter expressions (one per line). Type 'quit' ya 'exit' to stop.\n\n";

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
