/*
Dry Run — Stack-Based removeOuterParentheses
The Code
cpp

string removeOuterParentheses(string s) {
    string result;
    stack<char> st;

    for (char c : s) {
        if (c == '(') {
            if (!st.empty()) result.push_back(c);   // keep '(' only if stack is non-empty
            st.push(c);                             // always push '('
        } else {
            st.pop();                               // always pop on ')'
            if (!st.empty()) result.push_back(c);   // keep ')' only if stack is still non-empty
        }
    }
    return result;
}

💡 The Core Idea

The stack tracks how deeply we are nested.

    When we see '(':

        If the stack is already non-empty, we're going inside an existing primitive → this '(' is inner → keep it.

        If the stack is empty, we're starting a new primitive → this '(' is outermost → skip it.

        Then push it (so we track the new depth).

    When we see ')':

        Pop first (this closes the most recent '(').

        If the stack is still non-empty after popping, we're still inside a primitive → this ')' is inner → keep it.

        If the stack becomes empty after popping, we've closed the outermost pair → skip it.

So the rule is:
Character	Stack before	Stack after	Keep?
'('	empty	non-empty	❌ skip (outermost open)
'('	non-empty	non-empty	✅ keep (inner open)
')'	non-empty	non-empty	✅ keep (inner close)
')'	non-empty	empty	❌ skip (outermost close)
🧪 Dry Run 1: s = "(()())(())"

Length 10. Let's trace it character by character.
text

Index:  0  1  2  3  4  5  6  7  8  9
Char:   (  (  )  (  )  )  (  (  )  )

Initial state

    result = ""

    st = [] (empty)

i = 0, c = '('

    st.empty() → true

    So we do NOT push to result.

    st.push('(') → st = ['(']

    result = ""

i = 1, c = '('

    st.empty() → false

    So we do push to result → result = "("

    st.push('(') → st = ['(', '(']

i = 2, c = ')'

    st.pop() → st = ['(']

    st.empty() → false (still one '(')

    So we do push to result → result = "()"

i = 3, c = '('

    st.empty() → false

    So we do push to result → result = "()("

    st.push('(') → st = ['(', '(']

i = 4, c = ')'

    st.pop() → st = ['(']

    st.empty() → false

    So we do push to result → result = "()()"

i = 5, c = ')'

    st.pop() → st = []

    st.empty() → true

    So we do NOT push to result.

    result = "()()"

    🎯 End of first primitive "(()())" → removed outermost → "()()" ✅

i = 6, c = '('

    st.empty() → true

    So we do NOT push to result.

    st.push('(') → st = ['(']

    result = "()()"

i = 7, c = '('

    st.empty() → false

    So we do push to result → result = "()()("

    st.push('(') → st = ['(', '(']

i = 8, c = ')'

    st.pop() → st = ['(']

    st.empty() → false

    So we do push to result → result = "()()()"

i = 9, c = ')'

    st.pop() → st = []

    st.empty() → true

    So we do NOT push to result.

    result = "()()()"

    🎯 End of second primitive "(())" → removed outermost → "()" ✅

Final result: "()()()" ✅
📊 Summary Table for Dry Run 1
i	c	st before	st.empty() before	Action on result	st after	result
0	(	[]	true	skip	['(']	""
1	(	['(']	false	keep (	['(','(']	"("
2	)	['(','(']	—	pop → not empty → keep )	['(']	"()"
3	(	['(']	false	keep (	['(','(']	"()("
4	)	['(','(']	—	pop → not empty → keep )	['(']	"()()"
5	)	['(']	—	pop → empty → skip	[]	"()()"
6	(	[]	true	skip	['(']	"()()"
7	(	['(']	false	keep (	['(','(']	"()()("
8	)	['(','(']	—	pop → not empty → keep )	['(']	"()()()"
9	)	['(']	—	pop → empty → skip	[]	"()()()"
🧪 Dry Run 2: s = "()()"

Length 4.
text

Index:  0  1  2  3
Char:   (  )  (  )

Initial state

    result = ""

    st = []

i = 0, c = '('

    st.empty() → true → skip

    st.push('(') → st = ['(']

    result = ""

i = 1, c = ')'

    st.pop() → st = []

    st.empty() → true → skip

    result = ""

    End of first primitive "()" → removed outermost → "" ✅

i = 2, c = '('

    st.empty() → true → skip

    st.push('(') → st = ['(']

    result = ""

i = 3, c = ')'

    st.pop() → st = []

    st.empty() → true → skip

    result = ""

    End of second primitive "()" → removed outermost → "" ✅

Final result: "" ✅
📊 Summary Table for Dry Run 2
i	c	st before	st.empty() before	Action	st after	result
0	(	[]	true	skip	['(']	""
1	)	['(']	—	pop → empty → skip	[]	""
2	(	[]	true	skip	['(']	""
3	)	['(']	—	pop → empty → skip	[]	""
🧪 Dry Run 3: s = "((()))"

Length 6.
text

Index:  0  1  2  3  4  5
Char:   (  (  (  )  )  )

Initial state

    result = ""

    st = []

i = 0, c = '('

    st.empty() → true → skip

    st.push('(') → st = ['(']

    result = ""

i = 1, c = '('

    st.empty() → false → keep (

    result = "("

    st.push('(') → st = ['(', '(']

i = 2, c = '('

    st.empty() → false → keep (

    result = "(("

    st.push('(') → st = ['(', '(', '(']

i = 3, c = ')'

    st.pop() → st = ['(', '(']

    st.empty() → false → keep )

    result = "(()"

i = 4, c = ')'

    st.pop() → st = ['(']

    st.empty() → false → keep )

    result = "(())"

i = 5, c = ')'

    st.pop() → st = []

    st.empty() → true → skip

    result = "(())"

Final result: "(())" ✅
📊 Summary Table for Dry Run 3
i	c	st before	st.empty() before	Action	st after	result
0	(	[]	true	skip	['(']	""
1	(	['(']	false	keep (	['(','(']	"("
2	(	['(','(']	false	keep (	['(','(','(']	"(("
3	)	['(','(','(']	—	pop → not empty → keep )	['(','(']	"(()"
4	)	['(','(']	—	pop → not empty → keep )	['(']	"(())"
5	)	['(']	—	pop → empty → skip	[]	"(())"
🧪 Dry Run 4: s = "(()())(())(()(()))" (Example 2 from the problem)

Length 18.
text

Index:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17
Char:   (  (  )  (  )  )  (  (  )  )  (  (  )  (  (  )  )  )

Initial state

    result = ""

    st = []

Trace
i	c	st before	Action	st after	result
0	(	[]	empty → skip	['(']	""
1	(	['(']	non-empty → keep (	['(','(']	"("
2	)	['(','(']	pop → non-empty → keep )	['(']	"()"
3	(	['(']	non-empty → keep (	['(','(']	"()("
4	)	['(','(']	pop → non-empty → keep )	['(']	"()()"
5	)	['(']	pop → empty → skip	[]	"()()"
6	(	[]	empty → skip	['(']	"()()"
7	(	['(']	non-empty → keep (	['(','(']	"()()("
8	)	['(','(']	pop → non-empty → keep )	['(']	"()()()"
9	)	['(']	pop → empty → skip	[]	"()()()"
10	(	[]	empty → skip	['(']	"()()()"
11	(	['(']	non-empty → keep (	['(','(']	"()()()("
12	)	['(','(']	pop → non-empty → keep )	['(']	"()()()()"
13	(	['(']	non-empty → keep (	['(','(']	"()()()()("
14	(	['(','(']	non-empty → keep (	['(','(','(']	"()()()()(("
15	)	['(','(','(']	pop → non-empty → keep )	['(','(']	"()()()()(()"
16	)	['(','(']	pop → non-empty → keep )	['(']	"()()()()(())"
17	)	['(']	pop → empty → skip	[]	"()()()()(())"
Final result: "()()()()(())" ✅
📋 Key Observations from All Dry Runs

    Stack is never actually "searched" — we only use empty(), push(), and pop(). The stack simply tracks the current nesting depth.

    Two skip moments per primitive group:

        The '(' that takes stack from empty → non-empty (start of primitive).

        The ')' that takes stack from non-empty → empty (end of primitive).

    All inner characters are kept, regardless of depth.

    Valid input guarantee means we never pop() an empty stack — the code would crash otherwise, but the problem guarantees valid parentheses.

    Equivalent to a depth counter, but using a stack makes the "we're inside a primitive" test explicit via st.empty().

🧠 Stack vs. Depth Counter — Side by Side

The stack version and the depth-counter version produce identical results:
Event	Depth counter version	Stack version
'(' with depth 0 before	skip (outermost open)	skip (st.empty())
'(' with depth > 0 before	keep	keep (!st.empty())
')' making depth 0 after	skip (outermost close)	skip (st.empty() after pop)
')' making depth > 0 after	keep	keep (!st.empty() after pop)

The stack version is slightly more memory-heavy (O(n) vs O(1) for the counter), but conceptually clearer for some people.
Complexity
Metric	Value	Reasoning
Time	O(n)	Single pass; push/pop/empty are O(1)
Space	O(n)	Stack can grow to depth n/2 in the worst case (e.g., "((((...))))")
Output space	O(n)	result string

Compared to the depth-counter version (O(1) auxiliary space), the stack version trades memory for a slightly more explicit "are we inside a primitive?" test. Both are correct and both run in linear time.





Here is the complete step-by-step dry run of your function for the input string s = "(()())(())".
The function accurately strips the outermost parentheses of every primitive balanced string, reducing "(()())" to "()()" and "(())" to "()", resulting in "()()()".

Step-by-Step Execution Diagram

The diagram below tracks the character being read, the state of the stack, and how the result string grows over time. Notice that the outer parentheses are skipped precisely when the stack size is \(0\) before a push or \(0\) after a pop.

Detailed Dry Run Table

Step	Char	Stack Before	Action Taken	Stack After	result Added	Total result	Note
0	(	[]	Stack is empty. Push only.	[ ( ]	None	""	Outer ( (Skipped)
1	(	[ ( ]	Stack not empty. Add to result & Push.	[ (, ( ]	(	"("	Inner (
2	)	[ (, ( ]	Pop. Stack still has [ ( ]. Add to result.	[ ( ]	)	"()"	Inner )
3	(	[ ( ]	Stack not empty. Add to result & Push.	[ (, ( ]	(	"()("	Inner (
4	)	[ (, ( ]	Pop. Stack still has [ ( ]. Add to result.	[ ( ]	)	"()()"	Inner )
5	)	[ ( ]	Pop. Stack becomes empty.	[]	None	"()()"	Outer ) (Skipped)
6	(	[]	Stack is empty. Push only.	[ ( ]	None	"()()"	Outer ( (Skipped)
7	(	[ ( ]	Stack not empty. Add to result & Push.	[ (, ( ]	(	"()()("	Inner (
8	)	[ (, ( ]	Pop. Stack still has [ ( ]. Add to result.	[ ( ]	)	"()()()"	Inner )
9	)	[ ( ]	Pop. Stack becomes empty.	[]	None	"()()()"	Outer ) (Skipped)

Final Output

The function finishes the loop and returns: "()()()"
Would you like to see how to optimize this solution to \(O(1)\) auxiliary space by replacing the stack container with a simple integer counter variable?

yes

To optimize this solution to \(O(1)\) auxiliary space, we can replace the stack<char> with a single integer counter variable.
Since a stack in this problem only tracks the nesting depth of the opening parentheses (, an integer can track this exact same depth by incrementing on ( and decrementing on ).

Optimized C++ Code

cpp
#include <iostream>
#include <string>

using namespace std;

string removeOuterParentheses(string s) {
    string result;
    int counter = 0; // Replaces stack<char> st;

    for (char c : s) {
        if (c == '(') {
            // If counter > 0, this is an inner opening parenthesis
            if (counter > 0) result.push_back(c);
            counter++; // Analogous to st.push(c)
        } else {
            counter--; // Analogous to st.pop()
            // If counter > 0 after decrementing, this is an inner closing parenthesis
            if (counter > 0) result.push_back(c);
        }
    }
    return result;
}

int main() {
    string s = "(()())(())";
    cout << "Original: " << s << endl;
    cout << "Modified: " << removeOuterParentheses(s) << endl; // Output: ()()()
    return 0;
}
Use code with caution.

Complexity Comparison

Metric	Stack Approach	Counter Approach	Improvement
Time Complexity	\(O(N)\)	\(O(N)\)	Same runtime performance.
Space Complexity	\(O(N)\)	\(O(1)\)	No dynamic memory allocation for a container.

Why This Works Interfield

• counter > 0 before incrementing: Means we are already inside at least one pair of outer parentheses, so we keep the (.
• counter > 0 after decrementing: Means there is still an unmatched outer ( left in the background, so the current ) is part of an inner structure and should be kept.



*/

#include <iostream>
#include <string>
using namespace std;

class Solution
{
public:
  string removeOuterParentheses(string s)
  {
    string result;
    int depth = 0;

    for (char c : s)
    {
      if (c == '(')
      {
        // If depth > 0 before increment, this '(' is NOT outermost → keep
        if (depth > 0)
          result.push_back(c);
        depth++;
      }
      else
      { // c == ')'
        depth--;
        // If depth > 0 after decrement, this ')' is NOT outermost → keep
        if (depth > 0)
          result.push_back(c);
      }
    }

    return result;
  }
};

int main()
{
  Solution sol;

  // Test 1
  string s1 = "(()())(())";
  cout << "Input: \"" << s1 << "\" -> Output: \""
       << sol.removeOuterParentheses(s1) << "\"" << endl;

  // Test 2
  string s2 = "(()())(())(()(()))";
  cout << "Input: \"" << s2 << "\" -> Output: \""
       << sol.removeOuterParentheses(s2) << "\"" << endl;

  // Test 3
  string s3 = "()()";
  cout << "Input: \"" << s3 << "\" -> Output: \""
       << sol.removeOuterParentheses(s3) << "\"" << endl;

  // Test 4
  string s4 = "()";
  cout << "Input: \"" << s4 << "\" -> Output: \""
       << sol.removeOuterParentheses(s4) << "\"" << endl;

  // Test 5
  string s5 = "((()))";
  cout << "Input: \"" << s5 << "\" -> Output: \""
       << sol.removeOuterParentheses(s5) << "\"" << endl;

  return 0;
}

/*

1021. Remove Outermost Parentheses
Easy
Topics
premium lock iconCompanies
Hint

A valid parentheses string is either empty "", "(" + A + ")", or A + B, where A and B are valid parentheses strings, and + represents string concatenation.

    For example, "", "()", "(())()", and "(()(()))" are all valid parentheses strings.

A valid parentheses string s is primitive if it is nonempty, and there does not exist a way to split it into s = A + B, with A and B nonempty valid parentheses strings.

Given a valid parentheses string s, consider its primitive decomposition: s = P1 + P2 + ... + Pk, where Pi are primitive valid parentheses strings.

Return s after removing the outermost parentheses of every primitive string in the primitive decomposition of s.

 

Example 1:

Input: s = "(()())(())"
Output: "()()()"
Explanation: 
The input string is "(()())(())", with primitive decomposition "(()())" + "(())".
After removing outer parentheses of each part, this is "()()" + "()" = "()()()".

Example 2:

Input: s = "(()())(())(()(()))"
Output: "()()()()(())"
Explanation: 
The input string is "(()())(())(()(()))", with primitive decomposition "(()())" + "(())" + "(()(()))".
After removing outer parentheses of each part, this is "()()" + "()" + "()(())" = "()()()()(())".

Example 3:

Input: s = "()()"
Output: ""
Explanation: 
The input string is "()()", with primitive decomposition "()" + "()".
After removing outer parentheses of each part, this is "" + "" = "".

class Solution {
public:
    string removeOuterParentheses(string s) {
        
    }
};
give this with int main() and header files

*/
