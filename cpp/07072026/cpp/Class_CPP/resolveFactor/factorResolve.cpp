
/*
Program in One Sentence

    Read the text → split into terms (respecting parentheses) → parse each into a Term struct → find the GCD, minimum variable powers, and shared group → divide each term by the common factor → build a string like 2a^2b(a - 2b) and print it.

That's all the program does, no matter how complicated it looks.

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <cctype>
using namespace std;

/*

 example, 2a^3b becomes:
 Field	Value	Meaning
 coefficient	2	the number in front
 vars	{"a": 3, "b": 1}	a map: variable name → power
 group	""	no group here
 groupPower	0	—

 And c(a+b) becomes:
 Field	Value
 coefficient	1
 vars	{"c": 1}
 group	"(a+b)"
 groupPower	1

 Why map<string, int>? Because we need to look up "what's the power of a?" quickly, and map does that in O(log n). 

*/


//Job 2: PARSE — Turning Text into Data
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
Term parseTerm(const string &raw) //break one term into pieces
{
	Term t;
	string s;
	for (char c : raw) //First we strip spaces. That makes all subsequent logic simpler — no need to check for spaces later.
	{
		if (!isspace(c))
		{
			s += c;
		}
	}

	// coefficient at the start //"2a^3b" → "2" → coefficient = 2. "c(a+b)" → no digits → coefficient stays 1 (the default).
	size_t i = 0;
	string num;
	while (i < s.size() && isdigit(s[i])) num += s[i++];
	if (!num.empty()) t.coefficient = stoi(num);

	

	// group like (a+b) find the group.
//Find ( and the matching ). Everything between them (inclusive) is the group. Then truncate s so the group isn't scanned again in the variable loop. "c(a+b)" becomes "c"
	size_t open = s.find('(');
	if (open != string::npos)
	{
		size_t close = s.find(')', open);
		if (close != string::npos)
		{
			t.group = s.substr(open, close - open + 1); // // "(a+b)"
			t.groupPower = 1;
			s = s.substr(0, open);   // "c"
		}
	}

	// variables  // read the variables.

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
/*
Walk through the (now truncated) string. Each letter is a variable. If it's followed by ^, read the digits after it as its power. Store in the vars map.

For "2a^3b" after stripping the coefficient → "a^3b":

    k=0: 'a' → var "a", next char is ^ → power 3, skip to k=3

    k=3: 'b' → var "b", next char is end → power 1

vars = {"a": 3, "b": 1} ✓

*/

	return t;
}

// -------------------------------------------------------------------
// Split "c(a+b) + d(a+b) + e(a+b)" into terms, respecting parentheses
// -------------------------------------------------------------------
vector<Term> parseExpression(const string &line) // split the line into terms
{
	vector<Term> terms;

	string s; //First we strip spaces. That makes all subsequent logic simpler — no need to check for spaces later.
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

/*


Trace this for c(a+b)+d(a+b):
i	s[i]	depth before	depth after	Is it a split point?
0	c	0	0	no
1	(	0	1	no
2	a	1	1	no
3	+	1	1	no (depth = 1!)
4	b	1	1	no
5	)	1	0	no
6	+	0	0	YES → split
7	d	0	0	no
...	...	...	...	...

So at i = 6, we hit a + at depth 0 → split. s.substr(0, 6) = "c(a+b)" becomes term 1. Then start = 7, sign = 1.

The depth counter is everything. Without it, + inside (a+b) would split the group in half.

At the end of each iteration we also check atEnd (i == s.size()) to grab the last term
*/
	return terms;
}



// -------------------------------------------------------------------
// Format a term
// -------------------------------------------------------------------

/*

Three rules:

1.    Sign handling. First term: - at front if negative (no space). Later terms: " + " or " - " with spaces.

2.    Coefficient. Print the number unless it's 1 and there are variables (because 1a looks wrong — should be just a). If it's the only thing (no vars, no group), always print — otherwise 1 would print as empty.

3.    Variables and group. Iterate the map, print name then ^power (only if power > 1). Same for the group.

*/


//Job 4: PRINT — Building the Output String
string formatTerm(const Term &t, bool first) // turn one Term back into text
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
//Job 3: FACTOR — Find the Common Factor
string factorExpression(const vector<Term> &terms) //factorExpression does 5 sub-steps:
{
	if (terms.empty()) return "";

//1: GCD of coefficients	
	int g = abs(terms[0].coefficient); //For [2, -4] → gcd(2, 4) = 2. For [1, 1, 1] → gcd = 1.
	for (size_t i = 1; i < terms.size(); i++)
	{
		g = gcd(g, abs(terms[i].coefficient));
	}
	if (g == 0) g = 1;


//2: Common variables

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

/*

2a^3b - 4a^2b^2:

    Variable a: appears in both, powers are 3 and 2 → min is 2. → commonVars["a"] = 2

    Variable b: appears in both, powers are 1 and 2 → min is 1. → commonVars["b"] = 1

*/

//3: Common group
//Same idea as common variables, but for the string group. If every term has the exact same group string like (a+b), then that group is common. 
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


//4: Build the common-factor string

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

/*
   2a^3b - 4a^2b^2 → "2" + "a^2" + "b" = "2a^2b"

    c(a+b) + d(a+b) + e(a+b) → "(a+b)"

The if (g > 1) matters: if g == 1, we don't write "1" because 1x is just x.

*/


//5: Reduce each term inside parentheses

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
/*
    Coefficient: original / g

    Each variable: original power - common power. If it hits 0, don't include the variable.

    Group: original power - common group power.

Trace for 2a^3b - 4a^2b^2 (g = 2, commonVars = {a:2, b:1}):

Term 1 2a^3b:

    coefficient: 2 / 2 = 1

    a: 3 - 2 = 1 → keep a^1

    b: 1 - 1 = 0 → drop

    Result: 1a → formatted as "a" (coefficient 1 is hidden when there are vars)

Term 2 -4a^2b^2:

    coefficient: -4 / 2 = -2

    a: 2 - 2 = 0 → drop

    b: 2 - 1 = 1 → keep b^1

    Result: -2b

Full output: 2a^2b(a - 2b) ✓

Trace for c(a+b) + d(a+b) + e(a+b) (g = 1, commonVars = {}, commonGroup = (a+b), power = 1):

Term 1 c(a+b):

    coefficient: 1 / 1 = 1

    vars: c: 1 - 0 = 1 → keep

    group: (a+b) power 1 - 1 = 0 → drop

    Result: "c"

Term 2 d(a+b) → "d"
Term 3 e(a+b) → "e"

Full output: (a+b)(c + d + e) ✓

*/

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

//Job 1: READ — Getting Input
		cout << "> ";
		if (!getline(cin, line)) break;  // getline reads everything until the newline — unlike cin >> word which stops at spaces. That's important because expressions contain spaces.
		if (line == "quit" || line == "exit") break;
		if (line.empty()) continue;

		vector<Term> terms = parseExpression(line);     // 	turn the text into data
		string factored = factorExpression(terms);	//	do the math

		cout << "  " << line << "  =  " << factored << "\n\n";
	}
	return 0;
}


/*

Complete Example — Trace 2a^2 + 4a^2
Parse

parseExpression("2a^2 + 4a^2"):

    Strip spaces → "2a^2+4a^2"

    Walk characters:

        i=0..3: "2a^2", depth 0

        i=4: + at depth 0 → split. Term 1 = parseTerm("2a^2").

            coefficient = 2, vars = {a: 2}, no group.

        start = 5, sign = 1

        Continue to end: Term 2 = parseTerm("4a^2").

            coefficient = 4, vars = {a: 2}, no group.

terms = [{c=2, a:2}, {c=4, a:2}]
Factor

    GCD: gcd(2, 4) = 2 → g = 2

    Common vars: from term 1, a: appears in both, min(2, 2) = 2 → commonVars = {a:2}

    Common group: term 1 has no group → none.

    Common factor string: "2" + "a^2" = "2a^2"

    Reduce:

        Term 1: coef 2/2=1, a: 2-2=0 drop → "1" (printed as "1" because no vars)

        Term 2: coef 4/2=2, a: 2-2=0 drop → "2"

Result: "2a^2(1 + 2)" ✓
The Key C++ Concepts Used
Concept	Where	Why
struct	Term	group related fields together
std::vector<Term>	terms	dynamic array of terms
std::map<string,int>	vars	fast lookup + sorted iteration
std::gcd	factoring	compute GCD in one call
getline	main	read whole line including spaces
string::substr, find	parsing	slice strings at known positions
auto &	range-for	iterate without copying
lambda?	not used here — but could be	
abs, min, max	math	small helpers
isdigit, isalpha	parsing	check character types
One-Line Summary of Each Function
Function	Purpose in one line
main	Read lines from the keyboard; for each, parse → factor → print.
parseExpression	Split a line into terms at +/-, but only at parenthesis depth 0.
parseTerm	Break one term into coefficient + vars map + optional group.
factorExpression	Find GCD, min variable powers, and common group; reduce each term.
formatTerm	Turn one Term into a printable string with the right signs.


*/
