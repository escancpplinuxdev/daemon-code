/*

   2243. Calculate Digit Sum of a String
   Easy
   Topics
   premium lock iconCompanies
   Hint

   You are given a string s consisting of digits and an integer k.

   A round can be completed if the length of s is greater than k. In one round, do
   the following:

   Divide s into consecutive groups of size k such that the first k characters
   are in the first group, the next k characters are in the second group, and so
   on. Note that the size of the last group can be smaller than k. Replace each
   group of s with a string representing the sum of all its digits. For example,
   "346" is replaced with "13" because 3 + 4 + 6 = 13. Merge consecutive groups
   together to form a new string. If the length of the string is greater than k,
   repeat from step 1.

   Return s after all rounds have been completed.



   Example 1:

Input: s = "11111222223", k = 3
Output: "135"
Explanation:
- For the first round, we divide s into groups of size 3: "111", "112", "222",
and "23". Then we calculate the digit sum of each group: 1 + 1 + 1 = 3, 1 + 1 +
2 = 4, 2 + 2 + 2 = 6, and 2 + 3 = 5. So, s becomes "3" + "4" + "6" + "5" =
"3465" after the first round.
- For the second round, we divide s into "346" and "5".
Then we calculate the digit sum of each group: 3 + 4 + 6 = 13, 5 = 5.
So, s becomes "13" + "5" = "135" after second round.
Now, s.length <= k, so we return "135" as the answer.

Example 2:

Input: s = "00000000", k = 3
Output: "000"
Explanation:
We divide s into "000", "000", and "00".
Then we calculate the digit sum of each group: 0 + 0 + 0 = 0, 0 + 0 + 0 = 0, and
0 + 0 = 0. s becomes "0" + "0" + "0" = "000", whose length is equal to k, so we
return "000".

class Solution {
public:
string digitSum(string s, int k) {

}
};


Dry Run of the In-Place digitSum Program

Let me walk through the program step by step, explaining the logic and then dry-running each test case.
📜 Program Logic Breakdown
The Core Idea

Instead of creating a new string each round, this version overwrites the original string s in place. It uses a writeIdx pointer that advances as it writes the new characters, and an n variable that tracks the current "active length."
Key Variables
Variable	Purpose
n	Current length of the active string
writeIdx	Where to write the next output character (compacting in-place)
i	Start index of the current group
limit	End index (exclusive) of the current group
sum	Sum of digits in the current group
Round Structure

Each round:

    Iterate groups with i from 0 to n-1, stepping by k.

    For each group [i, limit), compute sum of digits.

    Write the decimal digits of sum into s starting at writeIdx.

        If sum == 0, write a single '0'.

        Otherwise, write digits in reverse (using % 10) then reverse them in place.

    After all groups, set n = writeIdx (the new active length).

    Repeat while n > k.

Critical Safety Note

The writeIdx is always ≤ the current i at the start of each group. Why? Because each group of k characters is replaced by a sum with at most log10(9k) digits — which is much smaller than k for k ≥ 2. So we never overwrite characters we haven't read yet.
🧪 Dry Run 1: s = "11111222223", k = 3

Initial: s = "11111222223", n = 11.
Round 1 (n = 11 > 3)

s indices: 0:1 1:1 2:1 3:1 4:1 5:2 6:2 7:2 8:2 9:2 10:3
Group	i	limit	Digits summed	sum	Write to s	writeIdx after	s after
1	0	3	s[0],s[1],s[2] = 1+1+1	3	s[0]='3'	1	"31111222223"
2	3	6	s[3],s[4],s[5] = 1+1+2	4	s[1]='4'	2	"34111222223"
3	6	9	s[6],s[7],s[8] = 2+2+2	6	s[2]='6'	3	"34611222223"
4	9	11	s[9],s[10] = 2+3	5	s[3]='5'	4	"34651222223"

After Round 1: n = writeIdx = 4. Active string is s[0..3] = "3465".
Round 2 (n = 4 > 3)
Group	i	limit	Digits summed	sum	Write to s	writeIdx after	s after
1	0	3	s[0],s[1],s[2] = 3+4+6	13	write "13": s[0]='3', s[1]='1', then reverse → s[0]='1', s[1]='3'	2	"1365..."
2	3	4	s[3] = 5	5	s[2]='5'	3	"1355..."

After Round 2: n = 3. Active string is s[0..2] = "135".
Round 3

n = 3 ≤ k = 3 → loop ends.

s.resize(3) → "135".

Result: "135" ✅
🧪 Dry Run 2: s = "00000000", k = 3

Initial: s = "00000000", n = 8.
Round 1 (n = 8 > 3)
Group	i	limit	Digits summed	sum	Write	writeIdx	s after
1	0	3	0+0+0	0	s[0]='0'	1	"00000000" (first char still 0)
2	3	6	0+0+0	0	s[1]='0'	2	"00000000"
3	6	8	0+0	0	s[2]='0'	3	"00000000"

After Round 1: n = 3. s[0..2] = "000".
Round 2

n = 3 ≤ 3 → loop ends.

s.resize(3) → "000".

Result: "000" ✅
🧪 Dry Run 3: s = "5", k = 1

Initial: s = "5", n = 1.

n = 1 > k = 1? No, 1 > 1 is false. Loop doesn't run.

s.resize(1) → "5".

Result: "5" ✅
🧪 Dry Run 4: s = "123", k = 5

Initial: s = "123", n = 3.

n = 3 > 5? No. Loop doesn't run.

s.resize(3) → "123".

Result: "123" ✅
🧪 Dry Run 5: s = "999999", k = 2

Initial: s = "999999", n = 6.
Round 1 (n = 6 > 2)
Group	i	limit	Digits summed	sum	Write	writeIdx	s after
1	0	2	9+9	18	write "18": s[0]='8', s[1]='1', reverse → s[0]='1', s[1]='8'	2	"189999"
2	2	4	9+9	18	s[2]='8', s[3]='1', reverse → s[2]='1', s[3]='8'	4	"181899"
3	4	6	9+9	18	s[4]='8', s[5]='1', reverse → s[4]='1', s[5]='8'	6	"181818"

After Round 1: n = 6. s = "181818".
Round 2 (n = 6 > 2)
Group	i	limit	Digits summed	sum	Write	writeIdx	s after
1	0	2	1+8	9	s[0]='9'	1	"981818"
2	2	4	1+8	9	s[1]='9'	2	"991818"
3	4	6	1+8	9	s[2]='9'	3	"999818"

After Round 2: n = 3. s[0..2] = "999".
Round 3 (n = 3 > 2)
Group	i	limit	Digits summed	sum	Write	writeIdx	s after
1	0	2	9+9	18	write "18": s[0]='8', s[1]='1', reverse → s[0]='1', s[1]='8'	2	"189818"
2	2	3	9	9	s[2]='9'	3	"189818" (s[2] was already 9)

After Round 3: n = 3. s[0..2] = "189".
Round 4 (n = 3 > 2)
Group	i	limit	Digits summed	sum	Write	writeIdx	s after
1	0	2	1+8	9	s[0]='9'	1	"989818"
2	2	3	9	9	s[1]='9'	2	"999818"

After Round 4: n = 2. s[0..1] = "99".
Round 5

n = 2 ≤ 2 → loop ends.

s.resize(2) → "99".

Result: "99" ✅
🔍 Why the In-Place Writing Is Safe

At the start of group i, we've read i characters so far and written writeIdx characters. We need writeIdx ≤ i to avoid overwriting unread data.

    Initially, writeIdx = 0 = i (before group 0).

    After processing group i, we've consumed k characters but written at most ⌈log10(9k)⌉ characters (the digit count of the maximum possible sum).

    For k ≥ 2, ⌈log10(9k)⌉ ≤ k always. So writeIdx grows by at most k while i grows by k — safe.

    The only exception is k = 1, where 9 × 1 = 9 → 1 digit, so writeIdx grows by 1 while i grows by 1 — still safe.

For k = 1, the loop never runs anyway (since n > 1 triggers, but each group is 1 char, sum = that digit, so string doesn't shrink — this would be an infinite loop! Let me check.)

Wait — for k = 1, each group is a single character. Its digit sum is the character itself. So s doesn't change, n = writeIdx = n, and n > k remains true forever → infinite loop!

But the test case only has k = 1 with n = 1 (Test 3), where the loop doesn't run. If k = 1 with n > 1, this code hangs. The problem probably guarantees k ≥ 2, or the test cases don't include k = 1 with n > 1. Worth noting as a caveat.
🧪 Dry Run 6 (Bonus): s = "1111", k = 1

    n = 4 > 1 → Round 1:

        Group 0: s[0]='1' → sum 1 → write '1' → writeIdx=1

        Group 1: s[1]='1' → sum 1 → write '1' → writeIdx=2

        Group 2: s[2]='1' → sum 1 → write '1' → writeIdx=3

        Group 3: s[3]='1' → sum 1 → write '1' → writeIdx=4

    n = 4 → still > 1 → infinite loop. ⚠️

This is a bug in the code for k = 1 with n ≥ 2. But LeetCode's problem constraints likely ensure k ≥ 2 (or the test suite doesn't include this case). Since the provided test cases don't hit it, the code passes.
📋 Summary of Dry Runs
Test	Input	Result	Expected	Match?
1	"11111222223", k=3	"135"	"135"	✅
2	"00000000", k=3	"000"	"000"	✅
3	"5", k=1	"5"	"5"	✅
4	"123", k=5	"123"	"123"	✅
5	"999999", k=2	"99"	"99"	✅
🧠 Key Takeaways

    In-place compaction saves memory but is trickier to reason about — the writeIdx ≤ i invariant makes it safe for k ≥ 2.

    Manual integer-to-string conversion avoids std::to_string overhead but adds ~10 lines of code. For this problem's small scale, the optimization is marginal.

    n tracking is crucial — s.size() isn't reliable mid-round because we're overwriting.

    Caveat: k = 1 with n ≥ 2 causes an infinite loop. The problem likely guarantees k ≥ 2 or avoids this case. If you want to be safe, add a guard:
    cpp

    if (k == 1) return s;  // nothing changes

    Since k = 1 means each group is 1 char, the sum equals the char itself, so the string never changes. Actually, for k = 1, the string never changes but the loop keeps running — so return s immediately is correct.

give above c++ code  how you calculate time and space complexity explain to interviewer
Time & Space Complexity Analysis — digitSum (2243)

Here's how I'd explain the complexity analysis to an interviewer, with the reasoning step by step.
📜 The Code (recap)
cpp

string digitSum(string s, int k) {
    int n = s.size();
    while (n > k) {
        int writeIdx = 0;
        for (int i = 0; i < n; i += k) {
            int sum = 0;
            int limit = min(i + k, n);
            for (int j = i; j < limit; ++j)
                sum += s[j] - '0';
            // write digits of sum to s[writeIdx...]
            if (sum == 0) s[writeIdx++] = '0';
            else {
                int start = writeIdx;
                while (sum > 0) { s[writeIdx++] = (sum % 10) + '0'; sum /= 10; }
                // reverse
                int end = writeIdx - 1;
                while (start < end) swap(s[start++], s[end--]);
            }
        }
        n = writeIdx;
    }
    s.resize(n);
    return s;
}

⏱ Time Complexity
Step 1 — Cost of one round

Let n be the current active length at the start of the round.

Inner loop over characters: We visit each of the n characters exactly once to compute group sums. That's O(n).

Writing digits: For each group of size k, the sum is at most 9k, so it has at most D = ⌈log₁₀(9k)⌉ + 1 digits. There are ⌈n/k⌉ groups, so total digit writes ≈ ⌈n/k⌉ × D.

    D is constant with respect to n (it depends only on k).

    For k ≥ 2, D ≤ 3 for typical values (e.g., k=2000 gives D=5).

    The reversal step does at most D/2 swaps per group → also O(⌈n/k⌉ × D).

So one round costs O(n) — the per-character scan dominates, and the digit-writing work is asymptotically smaller.
Step 2 — How many rounds?

After one round, the new length is at most:
text

n' = ⌈n/k⌉ × D

where D = ⌈log₁₀(9k)⌉ + 1 is a constant (independent of n).

So each round shrinks the length by roughly a factor of k/D.

    For k ≥ 2, k/D ≥ 2/log₁₀(18) ≈ 2/1.26 ≈ 1.58 > 1 — the length strictly shrinks each round.

    The number of rounds is therefore O(log_{k/D} n) = O(log n) when k ≥ 2.

    For large k (say k = n), we do 0 rounds. For k = 2, the shrink factor is about 2/2 = 1 … let me check D for k = 2: 9×2 = 18, D = 2. Shrink factor k/D = 1. Hmm, that means no shrink?

Let me re-examine. For k = 2, each pair sums to at most 18 → 2 digits. So n' = ⌈n/2⌉ × 2 = n roughly. No shrink!

Wait — but the sum of two digits is at most 9 + 9 = 18, which is 2 digits. So each group of 2 chars becomes at most 2 chars. In the worst case (all 9s), the string length stays exactly the same. Infinite loop!

But we saw in Test 5 ("999999", k=2) that it did terminate. Why? Because after round 1 ("181818"), the digits are 1 and 8, summing to 9 — 1 digit. So the length shrinks in subsequent rounds because the digits become smaller.

More generally: after the first round, all digits are at most D = 2 (for k=2), so each pair sums to at most 4 — still D=1 digit → shrink factor 2.

So the actual number of rounds is:

    Round 1: O(n)

    Round 2 onward: length shrinks by factor ≥ 2 per round (since sums fit in fewer digits)

    Total rounds: O(log n) after the first round, so O(log n) overall.

Step 3 — Total time

Each round is O(current length). The lengths form a geometric series:
text

n, n/2, n/4, n/8, ...

Sum = n × (1 + 1/2 + 1/4 + ...) = 2n = O(n).

Total Time = O(n) ✅
Edge case k = 1

For k = 1, each group is 1 char, sum = that char, so the string never changes → infinite loop. The problem implicitly excludes this case (or the code would need a guard). If we assume k ≥ 2, the analysis holds.
Edge case k ≥ n

0 rounds → O(1).
Interview-ready one-liner

    "Each round is O(n) because we scan every character once. The string length shrinks geometrically (at least by a factor of 2 per round for k ≥ 2), so the total work is a geometric series: n + n/2 + n/4 + … = O(n). Overall time is O(n)."

💾 Space Complexity
Variables used

    n, writeIdx, i, j, limit, sum, start, end, temp — all O(1) integers.

    The input string s — modified in place, no new string allocated for intermediate results.

    The final s.resize(n) — trims in place, no reallocation (capacity is already ≥ n).

What we don't allocate

    No string next buffer (unlike the naive solution, which allocates a new string each round → O(n) per round × O(log n) rounds = O(n) total auxiliary, but with extra allocation overhead).

    No vector<char> for groups.

    No to_string temporaries.

Auxiliary space

O(1) — only a fixed number of scalar variables.
Total space (including input)

O(n) — dominated by the input string s itself, which the problem counts as input, not auxiliary.
Interview-ready one-liner

    "Space is O(1) auxiliary — we modify the string in place, using only a handful of integer pointers. The only O(n) memory is the input string itself, which we're allowed to reuse."

📊 Comparison with the Naive Version
Version	Time	Auxiliary Space	Notes
Naive (new string each round)	O(n)	O(n)	Allocates a new string of size n/k × D per round; total auxiliary O(n)
In-place (this code)	O(n)	O(1)	Overwrites s in place; no extra buffer
Manual int→string	O(n)	O(1)	Avoids to_string temporary allocations

The in-place version is strictly better on space with the same asymptotic time.
🎯 How to Present This to an Interviewer

    "Let me analyze it. Per round, I scan all n characters once to compute group sums — that's O(n). Writing the sum digits is O(number of groups × digits per sum) = O((n/k) × log(9k)), which is asymptotically smaller than O(n) since log(9k) is a constant. So each round is O(n).

    Number of rounds: Each round, a group of k characters (each ≤ 9) produces a sum ≤ 9k, which fits in at most log₁₀(9k) + 1 digits — a constant independent of n. So the length shrinks by roughly a factor of k / log(9k) per round. For k ≥ 2, this factor is > 1, and after the first round the digits are small enough that the shrink is at least 2×. So we need O(log n) rounds.

    Total time: The lengths form a geometric series n, n/2, n/4, …, summing to O(n).

    Space: I'm modifying the string in place with a single write pointer. No new string, no vectors, no to_string temporaries. So auxiliary space is O(1) — just a handful of integer variables. Total space is O(n) for the input string itself.

    One caveat: for k = 1 the string never shrinks, so I'd add an early if (k == 1) return s; guard. The problem constraints effectively guarantee k ≥ 2."

📋 Summary Table
Aspect	Complexity	Reasoning
Time per round	O(n)	Scan all characters once; digit-writing is asymptotically smaller
Number of rounds	O(log n)	Length shrinks geometrically (≥ 2× per round after the first)
Total time	O(n)	Geometric series n + n/2 + n/4 + … = 2n
Auxiliary space	O(1)	In-place overwrite, only scalar variables
Total space	O(n)	Input string s (counted as input)
k = 1 edge case	Infinite loop	Needs guard if (k == 1) return s;
k ≥ n edge case	O(1)	Loop doesn't run
The "Aha" Moment for the Interviewer

    "The key insight is that the string length shrinks geometrically because k digits of size ≤ 9 sum to a number with only O(log k) digits — a constant number independent of n. So despite the outer while loop, the total work is a geometric series, not O(n log n). And by writing in place, we get O(1) auxiliary space — strictly better than the naive O(n) buffer approach."

 */

#include <iostream>
#include <string>
using namespace std;

class Solution
{
	public:
		string digitSum(string s, int k)
		{
			int n = s.size();
			while (n > k)
			{
				int writeIdx = 0; // Pointer to overwrite 's' in-place

				for (int i = 0; i < n; i += k)
				{
					int sum = 0;
					int limit = (i + k < n) ? (i + k) : n;

					for (int j = i; j < limit; ++j)
					{
						sum += s[j] - '0';
					}

					// Fast manual integer-to-char conversion (avoids std::to_string
					// overhead)
					if (sum == 0)
					{
						s[writeIdx++] = '0';
					}
					else
					{
						int start = writeIdx;
						while (sum > 0)
						{
							s[writeIdx++] = (sum % 10) + '0';
							sum /= 10;
						}
						// Reverse the digits we just wrote
						int end = writeIdx - 1;
						while (start < end)
						{
							char temp = s[start];
							s[start++] = s[end];
							s[end--] = temp;
						}
					}
				}
				n = writeIdx; // Update the active length of the string
			}

			s.resize(n); // Trim the leftover characters at the end
			return s;
		}
};

class Solution1
{
	public:
		string digitSum(string s, int k)
		{
			while ((int)s.size() > k)
			{
				string next = "";
				int n = (int)s.size();

				for (int i = 0; i < n; i += k)
				{
					int sum = 0;
					// Sum digits in the chunk [i, min(i+k-1, n-1)]
					for (int j = i; j < min(i + k, n); j++)
					{
						sum += (s[j] - '0');
					}
					next += to_string(sum);
				}

				s = next;
			}

			return s;
		}
};

int main()
{
	Solution sol;

	// Test 1
	string s1 = "11111222223";
	int k1 = 3;
	cout << "Test 1: \"" << s1 << "\", k=" << k1 << " -> \""
		<< sol.digitSum(s1, k1) << "\" (expected \"135\")" << endl;

	// Test 2
	string s2 = "00000000";
	int k2 = 3;
	cout << "Test 2: \"" << s2 << "\", k=" << k2 << " -> \""
		<< sol.digitSum(s2, k2) << "\" (expected \"000\")" << endl;

	// Test 3: single digit, k = 1
	string s3 = "5";
	int k3 = 1;
	cout << "Test 3: \"" << s3 << "\", k=" << k3 << " -> \""
		<< sol.digitSum(s3, k3) << "\" (expected \"5\")" << endl;

	// Test 4: already short enough
	string s4 = "123";
	int k4 = 5;
	cout << "Test 4: \"" << s4 << "\", k=" << k4 << " -> \""
		<< sol.digitSum(s4, k4) << "\" (expected \"123\")" << endl;

	// Test 5: k = 2
	string s5 = "999999";
	int k5 = 2;
	cout << "Test 5: \"" << s5 << "\", k=" << k5 << " -> \""
		<< sol.digitSum(s5, k5) << "\"" << endl;
	// Round 1: 99,99,99 → 18,18,18 → "181818"
	// Round 2: 18,18,18 → 9,9,9 → "999" (len 3 > 2)
	// Round 3: 99, 9 → 18, 9 → "189" (len 3 > 2)
	// Round 4: 18, 9 → 9, 9 → "99" (len 2 ≤ 2)

	return 0;
}

/*

   The test code you provided is a driver harness that creates an instance of the
   Solution class and validates the digitSum method against 5 different edge cases
   and standard inputs. It prints the input, the actual output from your function,
   and the expected output to verify correctness.

   Explanation of the Test Cases

   • Test 1 (Standard Case): Tests a string ("11111222223", length 11) that does
   not divide evenly by k = 3. It verifies that the final partial group ("23") is
   handled properly without going out of bounds. • Test 2 (All Zeros): Tests
   "00000000" with k = 3. It ensures that when a group's sum equals 0, the code
   properly writes the character '0' instead of skipping it or producing an empty
   string. • Test 3 (Single Character Boundary): Tests "5" with k = 1. Since n (1)
   is not greater than k (1), the while (n > k) loop is bypassed completely and
   returns "5" immediately. • Test 4 (String Shorter Than k): Tests "123" with k
   = 5. Since n (3) is already smaller than k (5), no grouping occurs and "123" is
   returned unchanged. • Test 5 (Multi-Round Reduction & Carry Over): Tests
   "999999" with k = 2. This triggers multiple rounds where two-digit sums (18)
   temporarily keep the string length high before shrinking down to length 2.

   Step-by-Step Dry Run (In-Place Optimized Code)

   Here is how the optimized in-place code executes Test 1 and Test 5, showing how
   writeIdx overwrites s safely without needing extra strings.

   Dry Run 1: s = "11111222223", k = 3

   • Initial State: s = "11111222223", n = 11. Since 11 > 3, enter Round 1.
   Round 1 (n = 11, writeIdx = 0):
   1. i = 0 to 2 ("111"): sum = 3. Writes '3' at s[0]. writeIdx becomes 1.
   2. i = 3 to 5 ("112"): sum = 4. Writes '4' at s[1]. writeIdx becomes 2.
   3. i = 6 to 8 ("222"): sum = 6. Writes '6' at s[2]. writeIdx becomes 3.
   4. i = 9 to 10 ("23"): sum = 5. Writes '5' at s[3]. writeIdx becomes 4.
   • End of Round 1: n becomes 4. The active prefix of s is now "3465" (leftover
   characters after index 3 are ignored). Round 2 (n = 4, writeIdx = 0): • Since 4
   > 3, enter Round 2.
   1. i = 0 to 2 ("346"): sum = 13.
   • Modulo loop writes '3' at s[0], then '1' at s[1].
   • Reversal loop swaps s[0] and s[1] to make "13". writeIdx becomes 2.
   2. i = 3 to 3 ("5"): sum = 5. Writes '5' at s[2]. writeIdx becomes 3.
   • End of Round 2: n becomes 3. Active prefix of s is "135".
Termination:
• Loop condition n > k (3 > 3) is false.
• s.resize(3) trims the unused tail, returning "135".

Dry Run 2: s = "999999", k = 2

• Initial State: s = "999999", n = 6.

Round	Active String (s[0..n-1])	Groups (k = 2)	Group Sums
Overwritten Prefix	New n (writeIdx)	Continue (n > 2)? 1 "999999"
"99", "99", "99"	18, 18, 18	"181818"	6	Yes (6 > 2) 2
"181818"	"18", "18", "18"	9, 9, 9	"999"	3	Yes (3 > 2) 3
"999"	"99", "9"	18, 9	"189"	3	Yes (3 > 2) 4	"189"	"18",
"9"	9, 9	"99"	2	No (2 > 2 is false)

• Termination: s.resize(2) trims the string to "99" and returns it.
If you'd like to explore this further, I can:
• Trace the memory layout of s during the in-place digit reversal step.
• Help you write unit tests with assert() so your test suite automatically fails
if an output is wrong.

To find the Time Complexity and Space Complexity of the optimized digitSum code,
we analyze how the input size \(N\) (the length of string s) shrinks in each
round and how much memory is used.

1. Time Complexity: \(\mathcal{O}(N)\)

Even though there are nested loops (while \(\rightarrow \) for \(\rightarrow \)
for), the total time complexity is linear, or \(\mathcal{O}(N)\). Here is how we
calculate it step by step:

Step A: Work Done in a Single Round

In one iteration of the while (n > k) loop:
• The outer for loop jumps by k steps (i += k), running roughly \(\frac{n}{k}\)
	times. • The inner for loop processes k characters per group. • Together, the
	two for loops visit every character from 0 to n - 1 once. That takes
	\(\mathcal{O}(n)\) operations. • Converting sum to characters and reversing them
takes at most \(\log_{10}(9k)\) steps per group. Since there are \(\frac{n}{k}\)
	groups, this also takes less than \(\mathcal{O}(n)\) operations. • Total work
	for one round: \(\mathcal{O}(n)\).

	Step B: How Much Does n Shrink Each Round?

	Let \(N\) be the initial length of the string.
	• Each group of \(k\) digits has a maximum possible sum of \(9k\) (when every
			digit is '9'). • The number of digits in \(9k\) is \(\lfloor \log_{10}(9k)
	\rfloor + 1\). • Because \(k \ge 2\), the number of digits in the sum is always
	strictly smaller than \(k\) (for example, when \(k = 2\), the max sum 18 has 2
			digits, and in the very next round "18" becomes "9", shrinking by half). •
	Specifically, every 1 to 2 rounds, the string length \(n\) shrinks by at least a
	constant factor (e.g., at least by half, \(n \rightarrow \frac{n}{2}\)).

	Step C: Summing the Work Across All Rounds

	If we add up the work done in every round as \(n\) shrinks, we get a geometric
	series:
\(\text{Total\ Work}\approx N+\frac{N}{2}+\frac{N}{4}+\frac{N}{8}+\dots +k\)
	Using the geometric series sum formula (\(1 + \frac{1}{2} + \frac{1}{4} + \dots
			= 2\)):
		\(\text{Total\ Work}\le 2N=\mathcal{O}(N)\)
	Note: Even if \(k = 2\) and the string is all '9's (like "999999"), a round
	might keep the same length once ("999999" \(\rightarrow \) "181818"), which at
	most doubles the work to \(4N\). Constants are dropped in Big-O notation,
	leaving \(\mathcal{O}(N)\).

	2. Space Complexity: \(\mathcal{O}(1)\) Auxiliary Space

	Space complexity measures the extra memory allocated by the algorithm beyond the
	input itself. • No Extra Strings or Arrays: Unlike your original code (which
			created a new string next of size up to \(N\)), this version overwrites the
	input string s directly using writeIdx. • Fixed Primitive Variables: We only
	declare a fixed number of integer and character variables (n, writeIdx, i, sum,
			limit, j, start, end, temp). • In-Place Resizing: s.resize(n) shrinks the
	existing buffer without allocating new heap memory. Because the number of
	variables never grows regardless of whether s has 10 characters or 10 million
	characters, the auxiliary space complexity is \(\mathcal{O}(1)\) (constant
			space).

	Side-by-Side Comparison


	Metric	Your Original Code	Optimized In-Place Code
Time Complexity	\(\mathcal{O}(N)\)	\(\mathcal{O}(N)\)
Auxiliary Space Complexity	\(\mathcal{O}(N)\) (allocates next string)
	\(\mathcal{O}(1)\) (modifies s in-place) Heap Allocations
	\(\mathcal{O}(N)\) (from next += to_string())	0 allocations

	Would you like to test your understanding with a quick time-complexity practice
	problem on nested loops, or would you like to see how to analyze amortized
	complexity for std::string operations?

	yes

	Here is a quick practice problem on nested loops—similar to digitSum where the
	inner loop variables depend on the outer loop—followed by a breakdown of how
	std::string amortized complexity works.

	Part 1: Practice Problem (Nested Loops)

	Look at the following C++ function. Try to figure out its Time Complexity before
	reading the solution below! cpp void processArray(int n) { for (int i = n; i >
			0; i /= 2) { for (int j = 0; j < i; j++) {
		// O(1) constant work
		cout << j << " ";
	}
	}
	}
Use code with caution.

How to Solve It

• Step 1 (Trace the Outer Loop): i starts at n and gets divided by 2 each step
				 (n, n/2, n/4, ..., 1). • Step 2 (Count Inner Loop Work): For each value of i,
				 the inner loop runs i times. • Step 3 (Sum the Operations): Add the work from
											     every iteration of the outer loop:
															       \(\text{Total\ Operations}=n+\frac{n}{2}+\frac{n}{4}+\frac{n}{8}+\dots +1\)
	• Answer: Because this forms a geometric series that sums to 2n, the time
	complexity is \(\mathcal{O}(n)\) (not \(\mathcal{O}(n \log n)\)).

	Part 2: Amortized Complexity of std::string Operations

	In your original code, you used next += to_string(sum). Understanding how C++
	handles += explains why your original code was slower in practice even though
	both versions are \(\mathcal{O}(N)\). • How std::string Grows: A std::string
	stores characters in a contiguous block of heap memory with a fixed capacity.
	When you append characters with += and exceed that capacity, C++ must allocate a
	new memory block (usually 1.5× or 2× larger), copy all existing characters over,
	and free the old memory. • Why += is Amortized \(\mathcal{O}(1)\): Because the
									   capacity doubles each time, expensive reallocations happen very rarely (at sizes
											   \(1, 2, 4, 8, 16, \dots\)). Spreading that rare copy cost across all the cheap
									   appends gives an average—or amortized—cost of \(\mathcal{O}(1)\) per character.
									   • Why In-Place Won: Even though += is amortized \(\mathcal{O}(1)\), your
									   original code created a brand-new string next in every while round. That forced
									   C++ to repeatedly allocate, grow, and destroy heap buffers from scratch, whereas
									   the in-place version reused the exact same buffer with zero reallocations. If
	you want to keep sharpening your complexity analysis skills, I can give you: • A
tricky nested-loop quiz (with 3–4 problems ranging from \(\mathcal{O}(\log N)\)
		to \(\mathcal{O}(N \log N)\)) • An explanation of space complexity in recursive
functions (call stack memory)

	*/
