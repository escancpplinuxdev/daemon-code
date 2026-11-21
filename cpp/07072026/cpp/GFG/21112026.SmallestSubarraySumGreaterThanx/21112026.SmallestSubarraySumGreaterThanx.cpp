/*
Smallest Subarray Sum Greater Than x
Difficulty: EasyAccuracy: 37.07%Submissions: 175K+Points: 2Average Time: 20m

Given an integer x and an array of integers, arr[]. Find the length of the smallest subarray whose sum is strictly greater than x.

If no such subarray exists, return 0.

Examples:

Input: x = 51, arr[] = [1, 4, 45, 6, 0, 19]
Output: 3
Explanation: The smallest subarray with a sum greater than 51 is [4, 45, 6], whose sum is 55 and length is 3.

Input: x = 100, arr[] = [1, 10, 5, 2, 7]
Output: 0
Explanation: No subarray has a sum greater than 100. Hence, the answer is 0.

class Solution {
  public:
    int smallestSubWithSum(vector<int>& arr, int x) {
        // code here
        
    }
};

also explain to interviewer how time and space complexity calculate

*/
/*
Smallest Subarray Sum Greater Than X
Explanation

We need the minimum length of a contiguous subarray whose sum is strictly greater than x. If none exists, return 0.
Key Insight — Sliding Window (Two Pointers)

Since all elements are non-negative (the problem statement implies integers, but to make sliding window work, we assume non-negative; actually let me check — the array can have zeros, but if negatives are allowed, sliding window fails. Given the problem's nature and constraints, it's typically non-negative. I'll assume non-negative as is standard for this problem).

With non-negative numbers, the sliding window technique works perfectly:

    Maintain a window [l, r].

    Expand r to increase the sum.

    When sum > x, try to shrink from the left (l++) to find a smaller window.

    Track the minimum window length found.

Algorithm

    l = 0, sum = 0, minLen = INF.

    For r = 0 to n-1:

        Add arr[r] to sum.

        While sum > x:

            Update minLen = min(minLen, r - l + 1).

            Subtract arr[l] from sum, increment l.

    Return minLen if found, else 0.

Why Sliding Window Works

    Monotonic property: Adding more elements only increases the sum (non-negative values). Removing elements only decreases it.

    When sum > x, shrinking from the left is safe: we might find a smaller valid window. We continue shrinking until sum ≤ x again, then expand r.

    This ensures we check all candidate windows that could be minimal.

Example 1: x = 51, arr = [1, 4, 45, 6, 0, 19]
r	arr[r]	sum after add	Window [l, r]	sum > 51?	Action	minLen
0	1	1	[0,0]	❌	—	INF
1	4	5	[0,1]	❌	—	INF
2	45	50	[0,2]	❌	—	INF
3	6	56	[0,3]	✅	len 4, shrink: sum=55, l=1	4
		55	[1,3]	✅	len 3, shrink: sum=51, l=2	3
		51	[2,3]	❌	stop	3
4	0	51	[2,4]	❌	—	3
5	19	70	[2,5]	✅	len 4, shrink: sum=25, l=3	3
		25	[3,5]	❌	stop	3

Result: 3 ✅ (subarray [4, 45, 6] from indices 1-3, sum 55)
Example 2: x = 100, arr = [1, 10, 5, 2, 7]

Total sum = 25 ≤ 100. No subarray exceeds 100. → 0 ✅
Final Code
cpp
*/
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Solution {
public:
    int smallestSubWithSum(vector<int>& arr, int x) {
        int n = (int)arr.size();
        int l = 0;
        long long sum = 0;
        int minLen = INT_MAX;

        for (int r = 0; r < n; r++) {
            sum += arr[r];

            // Shrink window while sum > x
            while (sum > x) {
                minLen = min(minLen, r - l + 1);
                sum -= arr[l];
                l++;
            }
        }

        return (minLen == INT_MAX) ? 0 : minLen;
    }
};

int main() {
    Solution sol;

    // Test 1
    vector<int> arr1 = {1, 4, 45, 6, 0, 19};
    cout << "Test 1: " << sol.smallestSubWithSum(arr1, 51)
         << " (expected 3)" << endl;

    // Test 2
    vector<int> arr2 = {1, 10, 5, 2, 7};
    cout << "Test 2: " << sol.smallestSubWithSum(arr2, 100)
         << " (expected 0)" << endl;

    // Test 3: single element greater than x
    vector<int> arr3 = {5};
    cout << "Test 3: " << sol.smallestSubWithSum(arr3, 3)
         << " (expected 1)" << endl;

    // Test 4: single element equal to x (must be strictly greater)
    vector<int> arr4 = {5};
    cout << "Test 4: " << sol.smallestSubWithSum(arr4, 5)
         << " (expected 0)" << endl;

    // Test 5: all positive, exact answer
    vector<int> arr5 = {2, 3, 1, 2, 4, 3};
    cout << "Test 5: " << sol.smallestSubWithSum(arr5, 7)
         << " (expected 2)" << endl;
    // [4,3] sum 7 not > 7; [2,3,1,2]=8 len 4; [3,1,2,4]=10 len 4; [1,2,4]=7 not >7; [2,4]=6 no;
    // Actually [4,3]=7 not >7. [2,3,1,2]=8 len 4. Hmm, let me find len 2: [4,3]=7 no. len 3: [3,1,2]=6, [1,2,4]=7 no, [2,4,3]=9 yes! len 3.
    // So expected is 3? Let me recheck. arr = [2,3,1,2,4,3], x=7.
    // Subarrays with sum > 7: [2,3,1,2]=8 (len 4), [3,1,2,4]=10 (len 4), [1,2,4,3]=10 (len 4),
    // [2,3,1,2,4]=12 (len 5), [3,1,2,4,3]=13 (len 5), [2,3,1,2,4,3]=15 (len 6),
    // [2,4,3]=9 (len 3). So min len is 3. Let me correct expected to 3.

    return 0;
}

/*
Let me fix Test 5's expected value to 3.
Dry Run of Final Code
Dry Run 1: arr = [1, 4, 45, 6, 0, 19], x = 51

Initial: l=0, sum=0, minLen=INF.
r	arr[r]	sum	Window	sum>51?	Shrink actions	minLen
0	1	1	[0,0]	❌	—	INF
1	4	5	[0,1]	❌	—	INF
2	45	50	[0,2]	❌	—	INF
3	6	56	[0,3]	✅	len=4, minLen=4; sum=55, l=1	4
		55	[1,3]	✅	len=3, minLen=3; sum=51, l=2	3
		51	[2,3]	❌	stop	3
4	0	51	[2,4]	❌	—	3
5	19	70	[2,5]	✅	len=4, minLen=3; sum=25, l=3	3
		25	[3,5]	❌	stop	3

Result: 3 ✅
Dry Run 2: arr = [1, 10, 5, 2, 7], x = 100
r	arr[r]	sum	sum>100?
0	1	1	❌
1	10	11	❌
2	5	16	❌
3	2	18	❌
4	7	25	❌

minLen stays INF → return 0 ✅
Dry Run 3: arr = [5], x = 3
r	arr[r]	sum	sum>3?	Action	minLen
0	5	5	✅	len=1, minLen=1; sum=0, l=1	1

Result: 1 ✅
Dry Run 4: arr = [5], x = 5
r	arr[r]	sum	sum>5?
0	5	5	❌ (5 is not > 5)

minLen = INF → return 0 ✅
Dry Run 5: arr = [2, 3, 1, 2, 4, 3], x = 7
r	arr[r]	sum	sum>7?	Shrink	minLen
0	2	2	❌	—	INF
1	3	5	❌	—	INF
2	1	6	❌	—	INF
3	2	8	✅	len=4, minLen=4; sum=6, l=1	4
4	4	10	✅	len=4, minLen=4; sum=8, l=2	4
		8	✅	len=3, minLen=3; sum=7, l=3	3
		7	❌	stop	3
5	3	10	✅	len=3, minLen=3; sum=8, l=4	3
		8	✅	len=2, minLen=2; sum=4, l=5	2
		4	❌	stop	2

Wait — at r=5, after adding 3, sum=10 (window [3,5] = [2,4,3]). Shrink: len=3, minLen=3, sum=8 (window [4,5] = [4,3]), l=4. Sum=8>7, len=2, minLen=2, sum=4 (window [5,5] = [3]), l=5. Sum=4≤7, stop.

So [4, 3] has sum 7 — not > 7. But wait, sum=8 after removing arr[3]=2? Let me recheck.

At r=5, before adding 3: sum=7 (window [3,4] = [2,4], sum=6+? wait).

Let me redo carefully:

After r=4: sum was 7 (window [3,4] = [2,4], sum=6? No, 2+4=6). Hmm, I had sum=7 at end of r=4. Let me redo.

Actually at r=4:

    sum += 4 → sum was 6 (after r=3, sum=6 with window [1,3]=[3,1,2]). Wait, after r=3: sum=6 (window [1,3]=[3,1,2]). l=1.

    r=4: sum += 4 → sum=10. Window [1,4]=[3,1,2,4].

    sum=10>7: len=4, minLen=4; sum=10-3=7, l=2. Window [2,4]=[1,2,4].

    sum=7 not >7. Stop.

After r=4: l=2, sum=7, minLen=4.

r=5: sum += 3 → sum=10. Window [2,5]=[1,2,4,3].

    sum=10>7: len=4, minLen=4; sum=10-1=9, l=3. Window [3,5]=[2,4,3].

    sum=9>7: len=3, minLen=3; sum=9-2=7, l=4. Window [4,5]=[4,3].

    sum=7 not >7. Stop.

Result: 3 ✅ (minLen=3, from window [3,5]=[2,4,3] sum=9 or [1,2,4,3] sum=10).

Wait, but sum=9 for [2,4,3] is >7, len=3. And [4,3] sum=7 not >7. So minLen=3.

Let me correct Test 5 expected to 3. I'll fix the main.

Actually, let me re-examine: is there a length-2 window with sum > 7? Pairs: (2,3)=5, (3,1)=4, (1,2)=3, (2,4)=6, (4,3)=7. None >7. Length-3: (2,3,1)=6, (3,1,2)=6, (1,2,4)=7, (2,4,3)=9 ✅. So min is 3. Correct.
Corrected Main
cpp

int main() {
    Solution sol;

    vector<int> arr1 = {1, 4, 45, 6, 0, 19};
    cout << "Test 1: " << sol.smallestSubWithSum(arr1, 51) << " (expected 3)" << endl;

    vector<int> arr2 = {1, 10, 5, 2, 7};
    cout << "Test 2: " << sol.smallestSubWithSum(arr2, 100) << " (expected 0)" << endl;

    vector<int> arr3 = {5};
    cout << "Test 3: " << sol.smallestSubWithSum(arr3, 3) << " (expected 1)" << endl;

    vector<int> arr4 = {5};
    cout << "Test 4: " << sol.smallestSubWithSum(arr4, 5) << " (expected 0)" << endl;

    vector<int> arr5 = {2, 3, 1, 2, 4, 3};
    cout << "Test 5: " << sol.smallestSubWithSum(arr5, 7) << " (expected 3)" << endl;

    return 0;
}


Complexity
Time Complexity
Step	Cost
Outer loop over r (n iterations)	O(n)
Inner while loop	Total across all iterations: each element is added once and removed at most once → O(n) total
Total time	O(n)

The key is that even though there's a nested loop, the inner loop's total work across all outer iterations is bounded by n (each element is subtracted at most once). This is the amortized O(1) per element argument.
Space Complexity
Item	Cost
l, r, sum, minLen	O(1)
No extra arrays or data structures	—
Total auxiliary space	O(1)
How to Explain to an Interviewer

    Problem restatement: "Find the minimum length of a contiguous subarray whose sum is strictly greater than x."

    Key insight: "Since all values are non-negative, adding elements only increases the sum and removing them only decreases it. This monotonicity lets us use a sliding window."

    Algorithm: "I expand the right pointer, adding elements to the sum. When sum > x, I record the window length and shrink from the left (removing arr[l], advancing l) while sum remains > x. This explores all minimal windows ending at each position."

    Correctness: "For each right endpoint r, once we've shrunk to the point where sum ≤ x, any further shrinking would only make it smaller — so we've found the minimal valid window ending at r. By considering all r, we find the global minimum."

    Time complexity: "The outer loop runs n times. The inner while loop subtracts each element at most once across the entire execution — because l only moves forward. So the total inner loop work is O(n). Total time is O(n)."

    Space complexity: "Only four integer variables — l, r, sum, minLen. No arrays or extra structures. So auxiliary space is O(1)."

    Edge cases: "If no subarray sums to > x, minLen remains INF and we return 0. If a single element > x, we catch it immediately. If the total sum ≤ x, we return 0."

    Why not prefix sums? "We could use prefix sums + binary search to find, for each l, the smallest r such that prefix[r] - prefix[l] > x — that would be O(n log n). Sliding window is better: O(n)."

Summary
Aspect	Detail
Problem	Min length subarray with sum > x
Approach	Sliding window (two pointers)
Key insight	Non-negative values → monotonic sum → sliding window works
Time	O(n) — amortized O(1) per element
Space	O(1) auxiliary
Edge cases	No valid subarray → 0; single element > x → 1
The "Aha" Moment

The nested while loop looks like it might make the time O(n²), but it's actually O(n). The key is that the l pointer only moves forward — each element is subtracted from sum at most once. So the total work of the inner loop across all outer iterations is bounded by n. This is the classic amortized analysis argument for sliding window: even though each element might be "touched" by the inner loop, the total touches is O(n).
Final Code (Copy-Paste)
cpp

class Solution {
public:
    int smallestSubWithSum(vector<int>& arr, int x) {
        int n = arr.size();
        int l = 0, minLen = INT_MAX;
        long long sum = 0;
        for (int r = 0; r < n; r++) {
            sum += arr[r];
            while (sum > x) {
                minLen = min(minLen, r - l + 1);
                sum -= arr[l];
                l++;
            }
        }
        return minLen == INT_MAX ? 0 : minLen;
    }
};

🎯 Time: O(n) | Space: O(1) auxiliary

*/
