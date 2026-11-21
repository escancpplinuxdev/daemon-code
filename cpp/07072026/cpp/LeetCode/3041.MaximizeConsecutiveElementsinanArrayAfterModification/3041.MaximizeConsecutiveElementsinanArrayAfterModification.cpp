/*
3041. Maximize Consecutive Elements in an Array After Modification
Hard
Topics
premium lock iconCompanies
Hint

You are given a 0-indexed array nums consisting of positive integers.

Initially, you can increase the value of any element in the array by at most 1.

After that, you need to select one or more elements from the final array such that those elements are consecutive when sorted in increasing order. For example, the elements [3, 4, 5] are consecutive while [3, 4, 6] and [1, 1, 2, 3] are not.

Return the maximum number of elements that you can select.

 

Example 1:

Input: nums = [2,1,5,1,1]
Output: 3
Explanation: We can increase the elements at indices 0 and 3. The resulting array is nums = [3,1,5,2,1].
We select the elements [3,1,5,2,1] and we sort them to obtain [1,2,3], which are consecutive.
It can be shown that we cannot select more than 3 consecutive elements.

Example 2:

Input: nums = [1,4,7,10]
Output: 1
Explanation: The maximum consecutive elements that we can select is 1.

class Solution {
public:
    int maxSelectedElements(vector<int>& nums) {
        
    }
};

and also show to interviewer how time and space complexity calculate


3041. Maximize Consecutive Elements in an Array After Modification
Explanation

We can increase each element by at most 1. We want to select the maximum number of elements that can form a consecutive sequence (like [3,4,5] or [1,2,3]) after sorting.
Key Insight — DP with Two Choices

After sorting nums, we process each element and decide:

    Keep it as is (value x)

    Increase it by 1 (value x+1)

We want the longest consecutive sequence we can form.
DP State

Let dp[v] = the maximum length of a consecutive sequence ending at value v using processed elements.

For each element x in sorted order, we have two options:

    Keep x as x: It can extend a sequence ending at x-1, or start a new sequence of length 1.

        dp[x] = max(dp[x], dp[x-1] + 1)

    Increase x to x+1: It can extend a sequence ending at x, or start a new sequence of length 1.

        dp[x+1] = max(dp[x+1], dp[x] + 1)

Critical: We must process option 2 before option 1 for each element. Why? Because if we process x as x first, then x+1 might be "blocked" by the current element using up the value x in a different way. Actually, let me think more carefully.

The standard trick: for each x:

    First, update dp[x+1] using the old dp[x] (before this element updates dp[x]). This represents using x+1 as the new value.

    Then, update dp[x] using dp[x-1].

Why this order? Because if we updated dp[x] first, then dp[x+1] would be based on the new dp[x] which includes the current element — that would count the same element twice.

Let me verify with an example.
Example 1: nums = [2,1,5,1,1]

Sort: [1, 1, 1, 2, 5].

DP initialization: dp[v] = 0 for all v.

Process x = 1 (first):

    Update dp[2] = max(dp[2], dp[1] + 1) = max(0, 0+1) = 1.

    Update dp[1] = max(dp[1], dp[0] + 1) = max(0, 0+1) = 1.

Process x = 1 (second):

    Update dp[2] = max(dp[2], dp[1] + 1) = max(1, 1+1) = 2.

    Update dp[1] = max(dp[1], dp[0] + 1) = max(1, 0+1) = 1.

Process x = 1 (third):

    Update dp[2] = max(dp[2], dp[1] + 1) = max(2, 1+1) = 2.

    Update dp[1] = max(dp[1], dp[0] + 1) = max(1, 0+1) = 1.

Process x = 2:

    Update dp[3] = max(dp[3], dp[2] + 1) = max(0, 2+1) = 3.

    Update dp[2] = max(dp[2], dp[1] + 1) = max(2, 1+1) = 2.

Process x = 5:

    Update dp[6] = max(dp[6], dp[5] + 1) = max(0, 0+1) = 1.

    Update dp[5] = max(dp[5], dp[4] + 1) = max(0, 0+1) = 1.

Max dp[v]: dp[3] = 3. ✅

Result: 3 ✅
Example 2: nums = [1,4,7,10]

Sort: [1, 4, 7, 10].

Process x=1:

    dp[2] = dp[1]+1 = 1

    dp[1] = dp[0]+1 = 1

Process x=4:

    dp[5] = dp[4]+1 = 1

    dp[4] = dp[3]+1 = 1

Process x=7:

    dp[8] = dp[7]+1 = 1

    dp[7] = dp[6]+1 = 1

Process x=10:

    dp[11] = dp[10]+1 = 1

    dp[10] = dp[9]+1 = 1

Max dp[v]: 1. ✅

Result: 1 ✅
Why Process dp[x+1] Before dp[x]?

Suppose we have nums = [1, 1]. Sort: [1, 1].

Correct order (x+1 first):

    x=1: dp[2] = dp[1]+1 = 1, then dp[1] = dp[0]+1 = 1.

    x=1: dp[2] = max(1, dp[1]+1) = max(1, 1+1) = 2, then dp[1] = max(1, 0+1) = 1.

    Max = 2. ✅ (We can make [1, 2] from [1, 1].)

Wrong order (x first):

    x=1: dp[1] = dp[0]+1 = 1, then dp[2] = dp[1]+1 = 2.

    x=1: dp[1] = max(1, dp[0]+1) = 1, then dp[2] = max(2, dp[1]+1) = 2.

    Max = 2. Hmm, same result here.

Let me find a case where it matters. Consider nums = [1, 2].

Correct order:

    x=1: dp[2] = dp[1]+1 = 1, dp[1] = dp[0]+1 = 1.

    x=2: dp[3] = dp[2]+1 = 2, dp[2] = max(1, dp[1]+1) = max(1, 2) = 2.

    Max = 2. ✅

Wrong order:

    x=1: dp[1] = 1, then dp[2] = dp[1]+1 = 2.

    x=2: dp[2] = max(2, dp[1]+1) = 2, then dp[3] = dp[2]+1 = 3.

    Max = 3. WRONG! We can't have 3 consecutive elements from [1, 2] because we only have 2 elements.

The bug: in the wrong order, when processing x=2, we first update dp[2] using dp[1], but dp[2] was already 2 from the previous step (using 1→2). Then dp[3] = dp[2]+1 = 3 uses the same element twice (once as the 1→2 and once as the 2→3). This double-counts.

The correct order avoids this: when processing x=2, we update dp[3] using the old dp[2] (which is 1 from the first element being used as 1→2 — wait no, let me redo).

Actually, let me redo the correct order carefully for [1, 2]:

    Initialize dp[0..] = 0.

    x=1:

        dp[2] = max(dp[2], dp[1] + 1) = max(0, 0+1) = 1. (Use 1 as 2.)

        dp[1] = max(dp[1], dp[0] + 1) = max(0, 0+1) = 1. (Use 1 as 1.)

    x=2:

        dp[3] = max(dp[3], dp[2] + 1) = max(0, 1+1) = 2. (Use 2 as 3, extending the sequence ending at 2 which has length 1.)

        dp[2] = max(dp[2], dp[1] + 1) = max(1, 1+1) = 2. (Use 2 as 2, extending the sequence ending at 1 which has length 1.)

    Max = 2. ✅

So the correct answer is 2. The wrong order gave 3, which is impossible.

Rule: Always process dp[x+1] before dp[x] for each element, because dp[x+1] should be based on the state before the current element is used as x.
Final Code
cpp
*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int maxSelectedElements(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        
        // dp[v] = max length of consecutive sequence ending at value v
        // Use a map or array. Since values can be up to ~10^9, use unordered_map.
        // Actually, we can use a map or just an array if we coordinate-compress.
        // For simplicity, use unordered_map.
        unordered_map<int, int> dp;
        int ans = 0;
        
        for (int x : nums) {
            // Option 1: use x+1 (process BEFORE x)
            dp[x + 1] = max(dp[x + 1], dp[x] + 1);
            ans = max(ans, dp[x + 1]);
            
            // Option 2: use x as x (process AFTER x+1)
            dp[x] = max(dp[x], dp[x - 1] + 1);
            ans = max(ans, dp[x]);
        }
        
        return ans;
    }
};

int main() {
    Solution sol;

    // Test 1
    vector<int> nums1 = {2, 1, 5, 1, 1};
    cout << "Test 1: " << sol.maxSelectedElements(nums1) << " (expected 3)" << endl;

    // Test 2
    vector<int> nums2 = {1, 4, 7, 10};
    cout << "Test 2: " << sol.maxSelectedElements(nums2) << " (expected 1)" << endl;

    // Test 3
    vector<int> nums3 = {1, 1};
    cout << "Test 3: " << sol.maxSelectedElements(nums3) << " (expected 2)" << endl;

    // Test 4
    vector<int> nums4 = {1, 2};
    cout << "Test 4: " << sol.maxSelectedElements(nums4) << " (expected 2)" << endl;

    // Test 5
    vector<int> nums5 = {3, 3, 3, 3};
    cout << "Test 5: " << sol.maxSelectedElements(nums5) << " (expected 4)" << endl;
    // [3,3,3,3] → can become [3,4,3,4] → sorted [3,3,4,4] → consecutive? No, [3,4] is length 2? 
    // Wait, [3,3,3,3]: we can make [3,4,3,4] → sorted [3,3,4,4]. Consecutive subsequence is [3,4] length 2.
    // Or [3,3,4,5]? But 4th element is 3, can only be 3 or 4. So max is [3,4,5]? We have three 3s and one 4? Let's see:
    // 3,3,3,3 → 3,4,3,4 or 4,4,4,4 or 3,3,3,4 etc.
    // Best: 3,4,5? We need a 3, a 4, a 5. We have four 3s. Change two to 4 and 5? But we can only add at most 1. So 3 can become 3 or 4. No 5 possible.
    // So best consecutive is [3,4] length 2? But we have 4 elements.
    // Wait, we can have 3,3,4,4 → sorted → 3,3,4,4. Consecutive subsequence: 3,4 length 2.
    // Or 3,4,4,4 → sorted → 3,4,4,4. Subsequence 3,4 length 2.
    // Actually, can we get 3,4,5? We need a 5. 3+1=4 max. So no 5. Max consecutive is [3,4] length 2? But we have 4 elements, so 2 seems too low.
    // Hmm, let me reconsider. The problem says "select one or more elements from the final array such that those elements are consecutive when sorted". We don't need to use all elements. We just need to select a subset that forms a consecutive sequence.
    // So for [3,3,3,3] → we can make it [3,4,3,4] → sorted [3,3,4,4]. We can select [3,4] (one 3 and one 4) → length 2. Or select [3,4] from [3,4,4,4]? Still length 2.
    // Wait, can we make [3,4,5,6]? No, 3 can only become 3 or 4.
    // So max is 2? Let me check with the DP.
    // Actually, I realize we can make [3,4,4,4] → sorted → [3,4,4,4]. The longest consecutive subsequence is [3,4] (length 2).
    // But wait, can we make [3,4,5,?]? No 5 possible.
    // So answer is 2? But that seems low. Let me re-read the problem.
    // "select one or more elements from the final array such that those elements are consecutive when sorted in increasing order"
    // So we select a subset. For [3,3,3,3], we can make [3,3,3,4] (change one 3 to 4). Then select [3,4] → length 2.
    // Or [3,3,4,4] → select [3,4] → length 2.
    // Can we get length 3? We'd need three consecutive values, e.g., [3,4,5]. We have no 5. So max is 2.
    // Hmm, but we have 4 elements. Can we make [3,4,4,4] and select [3,4,4]? No, [3,4,4] is not consecutive (duplicates). Consecutive means each element is exactly 1 more than the previous. So [3,4,4] is not consecutive.
    // So answer is 2.
    // Let me verify with DP:
    // x=3: dp[4] = dp[3]+1 = 1, dp[3] = dp[2]+1 = 1.
    // x=3: dp[4] = max(1, dp[3]+1) = max(1, 2) = 2, dp[3] = max(1, dp[2]+1) = 1.
    // x=3: dp[4] = max(2, dp[3]+1) = max(2, 2) = 2, dp[3] = max(1, dp[2]+1) = 1.
    // x=3: dp[4] = max(2, dp[3]+1) = 2, dp[3] = 1.
    // Max = 2. ✅
    // So Test 5 expected is 2, not 4.

    return 0;
}

/*

Wait — I need to fix the expected value for Test 5. Let me recompute.
Test 5: nums = [3, 3, 3, 3]

Sort: [3, 3, 3, 3].

DP:

    x=3: dp[4] = dp[3]+1 = 1, dp[3] = dp[2]+1 = 1.

    x=3: dp[4] = max(1, dp[3]+1) = max(1, 2) = 2, dp[3] = max(1, dp[2]+1) = 1.

    x=3: dp[4] = max(2, dp[3]+1) = max(2, 2) = 2, dp[3] = 1.

    x=3: dp[4] = 2, dp[3] = 1.

Max = 2. ✅

So Test 5 expected is 2, not 4. Let me correct.

Actually, wait — can we get 3? Let me manually think. We have four 3s. We can change at most 1 each. So values can be 3 or 4. We want a consecutive sequence. The best we can do is [3, 4] (one 3, one 4) → length 2. Or [4, 5]? No 5. So max is 2.

Yes, Test 5 expected is 2.

Let me also reconsider Test 1: [2,1,5,1,1]. We found 3. Let me verify manually: sorted [1,1,1,2,5]. Change to [2,1,2,3,5]? Wait, 1→2, 1→1, 1→1, 2→3, 5→5 → [2,1,1,3,5]. Select [1,2,3] → length 3. ✅

OK, let me fix the main.
Corrected Main
cpp

int main() {
    Solution sol;

    vector<int> nums1 = {2, 1, 5, 1, 1};
    cout << "Test 1: " << sol.maxSelectedElements(nums1) << " (expected 3)" << endl;

    vector<int> nums2 = {1, 4, 7, 10};
    cout << "Test 2: " << sol.maxSelectedElements(nums2) << " (expected 1)" << endl;

    vector<int> nums3 = {1, 1};
    cout << "Test 3: " << sol.maxSelectedElements(nums3) << " (expected 2)" << endl;

    vector<int> nums4 = {1, 2};
    cout << "Test 4: " << sol.maxSelectedElements(nums4) << " (expected 2)" << endl;

    vector<int> nums5 = {3, 3, 3, 3};
    cout << "Test 5: " << sol.maxSelectedElements(nums5) << " (expected 2)" << endl;

    return 0;
}

Dry Run of Final Code
Dry Run 1: nums = [2, 1, 5, 1, 1]

Sort: [1, 1, 1, 2, 5].
Step	x	dp[x+1] update	dp[x] update	dp state (relevant)	ans
Init	—	—	—	all 0	0
1	1	dp[2] = max(0, dp[1]+1) = 1	dp[1] = max(0, dp[0]+1) = 1	dp[1]=1, dp[2]=1	1
2	1	dp[2] = max(1, dp[1]+1) = 2	dp[1] = max(1, dp[0]+1) = 1	dp[1]=1, dp[2]=2	2
3	1	dp[2] = max(2, dp[1]+1) = 2	dp[1] = 1	dp[1]=1, dp[2]=2	2
4	2	dp[3] = max(0, dp[2]+1) = 3	dp[2] = max(2, dp[1]+1) = 2	dp[2]=2, dp[3]=3	3
5	5	dp[6] = max(0, dp[5]+1) = 1	dp[5] = max(0, dp[4]+1) = 1	dp[5]=1, dp[6]=1	3

Result: 3 ✅
Dry Run 2: nums = [1, 4, 7, 10]
Step	x	dp[x+1]	dp[x]	ans
1	1	dp[2]=1	dp[1]=1	1
2	4	dp[5]=1	dp[4]=1	1
3	7	dp[8]=1	dp[7]=1	1
4	10	dp[11]=1	dp[10]=1	1

Result: 1 ✅
Dry Run 3: nums = [1, 1]
Step	x	dp[x+1]	dp[x]	ans
1	1	dp[2]=1	dp[1]=1	1
2	1	dp[2]=max(1, 2)=2	dp[1]=max(1, 1)=1	2

Result: 2 ✅ (change to [1, 2])
Dry Run 4: nums = [1, 2]
Step	x	dp[x+1]	dp[x]	ans
1	1	dp[2]=1	dp[1]=1	1
2	2	dp[3]=max(0, dp[2]+1)=2	dp[2]=max(1, dp[1]+1)=2	2

Result: 2 ✅
Dry Run 5: nums = [3, 3, 3, 3]
Step	x	dp[x+1]	dp[x]	ans
1	3	dp[4]=1	dp[3]=1	1
2	3	dp[4]=max(1, dp[3]+1)=2	dp[3]=max(1, 1)=1	2
3	3	dp[4]=max(2, dp[3]+1)=max(2, 2)=2	dp[3]=1	2
4	3	dp[4]=2	dp[3]=1	2

Result: 2 ✅
Complexity
Time Complexity
Step	Cost
Sort	O(n log n)
DP loop (n iterations)	O(n) (each iteration does O(1) hash map operations)
Total	O(n log n)

The hash map operations are O(1) average. In the worst case (hash collisions), they could be O(n) each, but with good hash functions this is rare. Using unordered_map is fine here.

Alternative: If we coordinate-compress, we can use an array of size 2n+2, giving O(n) DP time with O(n) space. But sorting dominates anyway.
Space Complexity
Item	Cost
DP hash map	O(n) (at most n distinct keys)
Sort recursion stack	O(log n)
Total	O(n)
How to Explain This to an Interviewer

    Problem restatement: "We can add 0 or 1 to each element. We want to select the maximum subset that forms a consecutive sequence."

    Key insight: "After sorting, for each element x, we have two choices: use it as x or as x+1. We want the longest chain where each next value is exactly 1 more than the previous."

    DP state: "dp[v] = the longest consecutive sequence ending at value v. For each element x, we can extend a sequence ending at x-1 by using x as x, or extend a sequence ending at x by using x as x+1."

    Critical ordering: "We must process the x+1 option before the x option. Otherwise, the same element could be used twice — once to extend a sequence to x, and then again to extend further to x+1, which is illegal."

    Time: "Sorting is O(n log n), and the DP is O(n) with hash map operations. Total is O(n log n)."

    Space: "The DP hash map uses O(n) space, and sort uses O(log n) stack. Total is O(n)."

    Why not O(n)? "Sorting is necessary because consecutive elements must be processed in order. We could use a counting-sort-like approach if values were bounded, but they can be up to 10^9, so comparison sorting is needed."

Summary
Aspect	Detail
Problem	Max consecutive elements after adding 0 or 1 to each
Approach	Sort + DP with dp[v] = longest sequence ending at v
Critical detail	Process dp[x+1] before dp[x] to avoid double-counting
Time	O(n log n)
Space	O(n) for DP map, O(log n) sort stack
The "Aha" Moment

The element x can be used in two ways — as x or as x+1 — but not both. The ordering of updates ensures we don't accidentally use it twice. Specifically, updating dp[x+1] first uses the old dp[x] (before x contributes to it), so the current element is counted exactly once. If we updated dp[x] first, then dp[x+1] would include the current element's contribution to dp[x], effectively counting it twice.
Final Code (Copy-Paste)
cpp

class Solution {
public:
    int maxSelectedElements(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        unordered_map<int, int> dp;
        int ans = 0;
        for (int x : nums) {
            dp[x + 1] = max(dp[x + 1], dp[x] + 1);
            ans = max(ans, dp[x + 1]);
            dp[x] = max(dp[x], dp[x - 1] + 1);
            ans = max(ans, dp[x]);
        }
        return ans;
    }
};

🎯 Time: O(n log n) | Space: O(n)

*/

