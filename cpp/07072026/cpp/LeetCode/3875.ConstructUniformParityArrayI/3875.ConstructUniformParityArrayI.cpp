/*
3875. Construct Uniform Parity Array I
Easy
Topics
premium lock iconCompanies
Hint

You are given an array nums1 of n distinct integers.

You want to construct another array nums2 of length n such that the elements in nums2 are either all odd or all even.

For each index i, you must choose exactly one of the following (in any order):

    nums2[i] = nums1[i]
    nums2[i] = nums1[i] - nums1[j], for an index j != i

Return true if it is possible to construct such an array, otherwise, return false.

 

Example 1:

Input: nums1 = [2,3]

Output: true

Explanation:

    Choose nums2[0] = nums1[0] - nums1[1] = 2 - 3 = -1.
    Choose nums2[1] = nums1[1] = 3.
    nums2 = [-1, 3], and both elements are odd. Thus, the answer is true​​​​​​​.

Example 2:

Input: nums1 = [4,6]

Output: true

Explanation:​​​​​​​

    Choose nums2[0] = nums1[0] = 4.
    Choose nums2[1] = nums1[1] = 6.
    nums2 = [4, 6], and all elements are even. Thus, the answer is true.

class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        
    }
};
give this with int main()
*/

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        int n = nums1.size();
        if (n == 0) return true;
        if (n == 1) return true;  // Single element can be either odd or even
        
        // Count even and odd numbers
        int evenCount = 0, oddCount = 0;
        for (int num : nums1) {
            if (num % 2 == 0) evenCount++;
            else oddCount++;
        }
        
        // Case 1: All elements are already same parity
        if (evenCount == n || oddCount == n) return true;
        
        // Case 2: We can make all elements odd
        // We need to convert even elements to odd using nums2[i] = nums1[i] - nums1[j]
        // For even - even = even, even - odd = odd, odd - odd = even
        // So an even element can become odd if there is at least one odd element to subtract
        if (oddCount >= 1) {
            // All even elements can become odd by subtracting an odd number
            // Odd elements can remain odd (nums2[i] = nums1[i])
            return true;
        }
        
        // Case 3: We can make all elements even
        // Odd elements can become even by subtracting another odd number
        // But if there's only one odd number, it cannot become even
        if (oddCount >= 2) {
            // Odd elements can become even by subtracting another odd number
            // Even elements can remain even (nums2[i] = nums1[i])
            return true;
        }
        
        // If there's exactly 1 odd and rest even, we can't make all even
        // (because the single odd can't become even)
        // And we already checked all odd case
        return false;
    }
};

int main() {
    Solution solution;
    
    // Example 1
    vector<int> nums1 = {2, 3};
    cout << "Example 1: [2,3] -> " << (solution.uniformArray(nums1) ? "true" : "false") << endl;
    // Expected: true
    
    // Example 2
    vector<int> nums2 = {4, 6};
    cout << "Example 2: [4,6] -> " << (solution.uniformArray(nums2) ? "true" : "false") << endl;
    // Expected: true
    
    // Additional test cases
    vector<int> nums3 = {1, 3, 5};
    cout << "Test 3: [1,3,5] -> " << (solution.uniformArray(nums3) ? "true" : "false") << endl;
    // Expected: true (all odd already)
    
    vector<int> nums4 = {2, 4, 6};
    cout << "Test 4: [2,4,6] -> " << (solution.uniformArray(nums4) ? "true" : "false") << endl;
    // Expected: true (all even already)
    
    vector<int> nums5 = {1, 2};
    cout << "Test 5: [1,2] -> " << (solution.uniformArray(nums5) ? "true" : "false") << endl;
    // Expected: true (1 odd can stay odd, 2 even can become odd: 2-1=1)
    
    vector<int> nums6 = {1, 2, 3};
    cout << "Test 6: [1,2,3] -> " << (solution.uniformArray(nums6) ? "true" : "false") << endl;
    // Expected: true
    
    vector<int> nums7 = {1, 4, 6};
    cout << "Test 7: [1,4,6] -> " << (solution.uniformArray(nums7) ? "true" : "false") << endl;
    // Expected: false (only one odd, rest even can't make all even or all odd)
    // Wait, let me re-evaluate...
    
    // nums1 = [1, 4, 6]
    // evenCount = 2, oddCount = 1
    // oddCount >= 1 -> can make all odd: 4-1=3 (odd), 6-1=5 (odd), 1 stays odd
    // So actually true!
    
    vector<int> nums8 = {2, 4, 6, 8};
    cout << "Test 8: [2,4,6,8] -> " << (solution.uniformArray(nums8) ? "true" : "false") << endl;
    // Expected: true (all even already)
    
    vector<int> nums9 = {1, 3, 5, 7};
    cout << "Test 9: [1,3,5,7] -> " << (solution.uniformArray(nums9) ? "true" : "false") << endl;
    // Expected: true (all odd already)
    
    vector<int> nums10 = {2, 4, 6, 9};
    cout << "Test 10: [2,4,6,9] -> " << (solution.uniformArray(nums10) ? "true" : "false") << endl;
    // Expected: true (oddCount=1, can make all odd: 2-9=-7 odd, 4-9=-5 odd, 6-9=-3 odd, 9 stays odd)
    
    return 0;
}

/*
🧠 Explanation
Key Insight

We need to determine if we can make all elements of nums2 either all odd or all even.

For each index i, we can either:

    nums2[i] = nums1[i] (keep the element as is)

    nums2[i] = nums1[i] - nums1[j] where j != i

Parity Rules
Operation	Result Parity
even - even	even
even - odd	odd
odd - even	odd
odd - odd	even
Analysis
Case 1: All elements already same parity

    If all elements are even → nums2 = nums1 (all even) → true

    If all elements are odd → nums2 = nums1 (all odd) → true

Case 2: Make all elements odd

    We need to make all even elements become odd

    An even element can become odd by subtracting an odd number: even - odd = odd

    So we need at least one odd number in the array

    If oddCount >= 1, all even elements can become odd, and odd elements can stay odd

Case 3: Make all elements even

    We need to make all odd elements become even

    An odd element can become even by subtracting another odd number: odd - odd = even

    So we need at least two odd numbers in the array

    If oddCount >= 2, all odd elements can become even, and even elements can stay even

Summary
oddCount	Can make all odd?	Can make all even?	Result
0	❌ No (no odd to subtract)	✅ Yes (all even)	true
1	✅ Yes (subtract the odd)	❌ No (need 2 odds)	true
>= 2	✅ Yes (subtract one odd)	✅ Yes (subtract another odd)	true

Wait, this means it's ALWAYS true except when...

Let me reconsider... Actually, it seems like it's always true for n >= 2?

Let me check the edge cases:
Let's Test All Cases
cpp

// Test with 1 odd and rest even
nums1 = [1, 2, 4, 6]
oddCount = 1
// Make all odd: 2-1=1, 4-1=3, 6-1=5, 1 stays odd → TRUE

// Test with 2 odds and rest even
nums1 = [1, 3, 2, 4]
oddCount = 2
// Make all even: 1-3=-2, 3-1=2, 2 stays even, 4 stays even → TRUE

// Test with all even
nums1 = [2, 4, 6]
oddCount = 0
// All even already → TRUE

// Test with all odd
nums1 = [1, 3, 5]
oddCount = n
// All odd already → TRUE

So it seems like it's ALWAYS true for n >= 2!

But wait, let me check if there's any case where it fails...

Actually, let me reconsider. The problem says "The elements in nums2 are either all odd or all even."

If n = 1:

    Single element can be either odd or even (choose nums2[0] = nums1[0]) → true

If n >= 2, it seems always true, but let me verify if there's any hidden constraint I'm missing.

Actually, the problem says "You are given an array nums1 of n distinct integers." but I don't see any specific case where it would fail for n >= 2.

Wait, I think I found it! The issue is when we have exactly one odd and we want to make all elements even. We can't because:

    The single odd element: odd - odd is not possible (no other odd)

    odd - even = odd (stays odd)

    So the single odd element can never become even

But we don't NEED to make it even if we can make everything odd instead (which we can, since there's at least one odd to subtract).

So the answer is ALWAYS true for n >= 1?

Let me think again...

Actually, let me trace with a specific example: nums1 = [2, 4, 6] (all even)

    We can keep all as even (nums2 = nums1) → true

nums1 = [1, 3, 5] (all odd)

    We can keep all as odd (nums2 = nums1) → true

nums1 = [1, 2] (one odd, one even)

    Make all odd: 2-1=1 (odd), 1 stays odd → true

nums1 = [1, 2, 3] (two odds, one even)

    Make all even: 1-3=-2 (even), 3-1=2 (even), 2 stays even → true

So indeed, it's ALWAYS true for n >= 1!

Let me write the simplified solution:
cpp

class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        // It's always possible!
        // If n == 0: return true (empty array)
        // If n == 1: single element can be odd or even
        // If n >= 2: always possible
        return true;
    }
};

Wait, let me double-check if there's any edge case...

What if n = 2 and both numbers are even? e.g., [2, 4]

    All even already → true

What if n = 2 and both numbers are odd? e.g., [1, 3]

    All odd already → true

What if n = 2 and one odd one even? e.g., [1, 2]

    Make all odd: 2-1=1 (odd), 1 stays odd → true

    Or make all even: 1-? cannot make 1 even because 1-2=-1 (odd), 2 stays even

So yes, always true for n >= 1!

But wait, let me re-read the problem statement carefully:

"nums2[i] = nums1[i] - nums1[j], for an index j != i"

This means we can subtract ANY other element, not just one fixed element.

So for [2, 4] (both even):

    To make all odd: 2 can subtract 4: 2-4=-2 (even) → not odd

    Actually, even - even = even, so we can't make even numbers odd if all numbers are even!

Aha! This is the edge case!

If all numbers are even, we can't make them odd because even - even = even.
If all numbers are odd, we can't make them even because odd - odd = even (wait, odd - odd = even, so we CAN make all even!)

Wait, if all numbers are odd, we CAN make them all even by subtracting pairs of odds.

But if all numbers are even, we CAN'T make them all odd because even - even = even.

So the ONLY failure case is when all numbers are even? No...

Actually, let's reconsider:

    If all even: we can make all even (keep as is) → true

    If all odd: we can make all odd (keep as is) → true

    If mixed: we can make all odd (subtract an odd from evens) OR all even (subtract odds from each other, keep evens as is)

So it's ALWAYS true!

Wait, what about [2, 4, 6] (all even)?

    All even already → true

What about [1, 3, 5] (all odd)?

    All odd already → true

What about [1, 3, 2, 4] (mixed)?

    All odd: 2-1=1, 4-1=3, 1 stays odd, 3 stays odd → true

    All even: 1-3=-2, 3-1=2, 2 stays even, 4 stays even → true

So indeed, it's ALWAYS TRUE for n >= 1!

Let me verify the problem constraints more carefully... The problem says "n distinct integers" but doesn't mention any other constraints.

Actually, wait. Let me re-read the problem statement:

"nums2[i] = nums1[i] - nums1[j], for an index j != i"

This means for each index i, you can either keep nums1[i] or replace it with nums1[i] - nums1[j] for some j != i.

The key insight is: You can choose a DIFFERENT j for each i!

So for an even number x, to make it odd, you need to subtract an odd number y from it: x - y = odd.
Since you can choose any j != i, if there's at least one odd number in the array, all even numbers can become odd.

For an odd number x, to make it even, you need to subtract another odd number y: x - y = even.
So if there are at least 2 odd numbers, all odd numbers can become even.

But we don't NEED to make all numbers even if we can make all numbers odd (which requires at least 1 odd).

So the only case where it might fail is if there are NO odd numbers (all even), but in that case, we can just keep all numbers as even.

Therefore, the answer is always true!

Actually wait... I need to reconsider if there's any case where it's false...

What if n = 0? Empty array - should be true (vacuously).

What if n = 1? Single element - can be odd or even → true.

So yes, ALWAYS TRUE!

Let me provide the final answer:
✅ Final Solution (Simplified)
cpp

class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        // It's always possible to make all elements odd or even
        // because:
        // 1. If all elements are already same parity → true
        // 2. If there's at least one odd element → make all odd by subtracting that odd from evens
        // 3. If there are at least two odd elements → make all even by subtracting odds from each other
        // 4. If all elements are even → keep as even
        return true;
    }
};

Wait, I just realized I might be missing something. Let me re-examine [2, 4, 6] more carefully:

To make all odd, we need to change 2, 4, 6 to odd:

    2 - ? = odd → need to subtract an odd number

    4 - ? = odd → need to subtract an odd number

    6 - ? = odd → need to subtract an odd number

But there is NO odd number in the array to subtract! So we CAN'T make them all odd.
But we CAN make them all even (by keeping them as is).

So the answer is true.

What about [2, 4, 6, 8, 10]? All even → true.

What about [2, 4, 6, 1]? Mixed → true.

So yes, ALWAYS TRUE!
🎯 Final Code
cpp

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        // It's always possible to construct uniform parity array
        // - If all even: keep as even
        // - If at least one odd: make all odd by subtracting odd from evens
        // - If at least two odds: also can make all even
        // - If only one odd: keep odds as odd, make evens odd
        return true;
    }
};

int main() {
    Solution solution;
    
    vector<int> test1 = {2, 3};
    cout << "[2,3] -> " << (solution.uniformArray(test1) ? "true" : "false") << endl;
    
    vector<int> test2 = {4, 6};
    cout << "[4,6] -> " << (solution.uniformArray(test2) ? "true" : "false") << endl;
    
    vector<int> test3 = {1, 3, 5};
    cout << "[1,3,5] -> " << (solution.uniformArray(test3) ? "true" : "false") << endl;
    
    vector<int> test4 = {2, 4, 6};
    cout << "[2,4,6] -> " << (solution.uniformArray(test4) ? "true" : "false") << endl;
    
    vector<int> test5 = {1, 2, 4, 6};
    cout << "[1,2,4,6] -> " << (solution.uniformArray(test5) ? "true" : "false") << endl;
    
    vector<int> test6 = {1, 2};
    cout << "[1,2] -> " << (solution.uniformArray(test6) ? "true" : "false") << endl;
    
    return 0;
}

Output:
text

[2,3] -> true
[4,6] -> true
[1,3,5] -> true
[2,4,6] -> true
[1,2,4,6] -> true
[1,2] -> true

🧠 Conclusion

The answer is ALWAYS true for all valid inputs!

The problem might be trickier than it appears, but the solution is actually simple: it's always possible to construct a uniform parity array because you can always either keep the elements as they are (if they're already uniform) or use the difference operation to change parity (if there's at least one odd element).




*/
