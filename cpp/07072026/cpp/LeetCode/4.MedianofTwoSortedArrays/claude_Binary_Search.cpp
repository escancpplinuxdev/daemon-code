/*
5 year experience interview question on binary search, and problem on it in leetcode, GFG, strivers, coding 360, etc, and more other problem give

Binary Search — Complete Interview Question Bank (5-Year Experience Level)

Same format as the vector bank — categorized, senior-depth, Allman-style code throughout.

Category 1: Fundamentals & Complexity
Q1. Why is binary search O(log n)? Prove it mathematically.

Answer: Each iteration eliminates half the remaining search space. Starting with n elements, after k iterations the space shrinks to n / 2^k. The search terminates when the space reaches 1 element:

n / 2^k = 1
2^k = n
k = log2(n)

So the number of iterations (and comparisons) grows logarithmically with input size — doubling n adds just one extra iteration.

cpp
#include <iostream>

int binarySearch(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size() - 1;
    int iterations = 0;

    while (lo <= hi)
    {
        ++iterations;
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target)
        {
            std::cout << "Found in " << iterations << " iterations\n";
            return mid;
        }
        else if (arr[mid] < target)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }

    std::cout << "Not found, " << iterations << " iterations\n";
    return -1;
}
// n=1,000,000 -> ~20 iterations. n=1,000,000,000 -> ~30 iterations.
Q2. What are the strict preconditions for binary search to work correctly?

Answer:

The search space must be sorted (or more precisely, partitioned by a monotonic predicate — see Q16).
The container must support random access — O(1) jump to any index. This is why binary search works directly on vector/array but not on list (an O(n) walk to reach mid defeats the entire purpose).
There must be a well-defined comparison between elements and the target.
cpp
// Binary search on std::list compiles but is a performance disaster —
// std::advance() on a non-random-access iterator is O(n), so the
// "binary search" degrades to O(n log n) overall, worse than a linear scan.
std::list<int> l = {1, 2, 3, 4, 5};
// std::binary_search still WORKS (it only needs ForwardIterator), but internally
// every mid-computation costs O(n) — never do this in real code.
Q3. Iterative vs recursive binary search — what's the actual trade-off?

Answer:

	Iterative	Recursive
Space	O(1)	O(log n) — call stack frames
Speed	slightly faster (no call overhead)	slightly slower
Tail-call optimization	n/a	may be optimized to O(1) space by the compiler at -O2, but this is not guaranteed by the C++ standard — never rely on it
cpp
// Iterative — O(1) space, always
int binarySearchIter(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target) return mid;
        if (arr[mid] < target)  lo = mid + 1;
        else                    hi = mid - 1;
    }

    return -1;
}

// Recursive — O(log n) space (unless TCO kicks in, which you cannot rely on)
int binarySearchRec(const std::vector<int>& arr, int lo, int hi, int target)
{
    if (lo > hi)
    {
        return -1;
    }

    int mid = lo + (hi - lo) / 2;

    if (arr[mid] == target)
    {
        return mid;
    }
    else if (arr[mid] < target)
    {
        return binarySearchRec(arr, mid + 1, hi, target);
    }
    else
    {
        return binarySearchRec(arr, lo, mid - 1, target);
    }
}

Interview answer: default to iterative in production/interview code — no stack overflow risk on pathological inputs, no TCO gamble.

Category 2: Implementation Correctness — the Classic Bug-Hunting Questions
Q4. What's wrong with mid = (lo + hi) / 2? How do you fix it?

Answer: lo + hi can overflow int if both are large (each up to INT_MAX), since their sum can exceed INT_MAX. This is a real, historically famous bug — it existed in the binary search implementation shipped in the JDK for nearly a decade (Joshua Bloch wrote about it in 2006).

cpp
// BUGGY — can overflow when lo and hi are both large
int mid = (lo + hi) / 2;

// CORRECT — never overflows, since (hi - lo) is always <= the array size
int mid = lo + (hi - lo) / 2;

Concrete failure case: lo = 1'500'000'000, hi = 2'000'000'000 → lo + hi = 3'500'000'000, which overflows a 32-bit signed int (max ~2.147 billion) → undefined behavior, likely producing a negative or nonsensical mid.

Q5. while (lo <= hi) vs while (lo < hi) — when do you use each, and what breaks if you mix them up?

Answer: This depends on the invariant you're maintaining.

lo <= hi — used when [lo, hi] is a closed interval representing "still-possible answer positions," and the loop must check the case where lo == hi (exactly one candidate left).

lo < hi — used when [lo, hi] is maintained as a range where you're converging lo and hi to the same index (used heavily in lower_bound/upper_bound style problems and "find peak/minimum" problems). The loop stops the instant lo == hi, and that final value is the answer — no separate post-loop check needed.

cpp
// Style A: lo <= hi, explicit found-check inside the loop
int style_A(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target)  lo = mid + 1;
        else                    hi = mid - 1;
    }
    return -1;
}

// Style B: lo < hi, converges lo==hi, answer read AFTER the loop
int style_B_findFirstTrue(int lo, int hi, std::function<bool(int)> pred)
{
    // finds smallest index where pred(index) is true, assuming pred is monotonic false...false,true...true
    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (pred(mid))
        {
            hi = mid;        // mid could be the answer — keep it in range
        }
        else
        {
            lo = mid + 1;    // mid is definitely not the answer
        }
    }
    return lo;   // lo == hi here — this IS the answer
}

The bug people make: mixing the two styles — e.g. using lo < hi but updating with hi = mid - 1 (borrowed from Style A) instead of hi = mid. This can skip past the correct answer entirely, because mid itself might be the answer and mid - 1 throws it away.

Q6. Walk through 3 concrete off-by-one bugs in binary search.

Answer:

cpp
// BUG 1: hi initialized to arr.size() instead of arr.size() - 1
int lo = 0, hi = arr.size();          // WRONG — hi should be size()-1
while (lo <= hi)
{
    int mid = lo + (hi - lo) / 2;
    // arr[mid] can access arr[arr.size()] on the first iteration if
    // lo=0 and hi=size() and (hi-lo)/2 rounds up to size() — OUT OF BOUNDS READ
}

// BUG 2: forgetting +1 / -1 when narrowing — causes INFINITE LOOP
int lo = 0, hi = (int)arr.size() - 1;
while (lo <= hi)
{
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] < target)
    {
        lo = mid;        // WRONG — should be mid + 1
        // if lo becomes hi eventually and arr[mid] is still < target,
        // mid = lo again forever -> lo NEVER advances -> infinite loop
    }
    else
    {
        hi = mid - 1;
    }
}

// BUG 3: using lo < hi loop condition but forgetting the answer is lo (not mid) after the loop
int lo = 0, hi = (int)arr.size() - 1;
while (lo < hi)
{
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] < target) lo = mid + 1;
    else                   hi = mid;
}
// WRONG: return mid;   <- 'mid' is a variable local to the loop body,
//                          this doesn't even compile outside the loop in well-written code,
//                          but people sometimes hoist 'mid' outside and return
//                          a STALE value from the second-to-last iteration
// CORRECT: return lo;  (or hi — they're equal at this point)
Q7. Show a concrete infinite-loop example and explain exactly why it hangs.

Answer:

cpp
// This hangs FOREVER for certain inputs — find it before running it
int findFirstBad(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;   // NOTE: this rounds DOWN (toward lo)

        if (arr[mid] < target)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid;         // BUG: when lo and hi differ by exactly 1,
                              // mid == lo, and if arr[mid] >= target,
                              // hi is set to mid == lo — no progress!
                              // Next iteration: lo==hi triggers exit... 
                              // Actually this specific case DOES terminate.
        }
    }
    return lo;
}

// The REAL infinite loop trap is the mirror image:
int findLastBad(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo < hi)
    {
        int mid = lo + (hi - lo + 1) / 2;   // rounds UP (toward hi) -- needed here

        if (arr[mid] > target)
        {
            hi = mid - 1;
        }
        else
        {
            lo = mid;   // if you'd instead used the DOWN-rounding mid here,
                        // and lo,hi differ by 1, mid == lo, so lo = mid is a no-op
                        // forever -> INFINITE LOOP
        }
    }
    return lo;
}

The rule to memorize: when your update sets lo = mid (keeping mid as still-possibly-correct), you must round mid up (lo + (hi - lo + 1) / 2) to guarantee progress. When your update sets hi = mid, round down (the default). Mixing these causes infinite loops when lo and hi are adjacent.

Category 3: Variants — Lower Bound, Upper Bound, First/Last Occurrence
Q8. Implement lower_bound from scratch. What exactly does it return?

Answer: lower_bound(target) returns the index of the first element >= target (or arr.size() if none exists). It answers "where would I insert target to keep the array sorted, placing it before any equal elements?"

cpp
int lowerBound(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size();   // hi = size(), NOT size()-1 — this is intentional,
                                        // since the answer can legitimately be "one past the end"

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] < target)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid;      // arr[mid] >= target — could be the answer, keep it
        }
    }

    return lo;
}
Q9. Implement upper_bound from scratch. How does it differ from lower_bound?

Answer: upper_bound(target) returns the index of the first element > target — the difference from lower_bound is a single changed comparison operator (<= instead of <):

cpp
int upperBound(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size();

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] <= target)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid;
        }
    }

    return lo;
}

// Together: [lowerBound(target), upperBound(target)) is EXACTLY the range
// of all indices where arr[i] == target — this is how you count occurrences
// of target in O(log n) instead of O(n):
int countOccurrences(const std::vector<int>& arr, int target)
{
    return upperBound(arr, target) - lowerBound(arr, target);
}
Q10. Find the first and last occurrence of a target in a sorted array with duplicates.

Answer: This is LC #34 — Find First and Last Position of Element in Sorted Array. Two separate binary searches, each biased toward one end:

cpp
std::pair<int, int> searchRange(const std::vector<int>& arr, int target)
{
    int n = (int)arr.size();

    // --- Find FIRST occurrence: bias toward lo when arr[mid] == target ---
    int first = -1;
    int lo = 0, hi = n - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target)
        {
            first = mid;
            hi = mid - 1;      // keep searching LEFT for an earlier occurrence
        }
        else if (arr[mid] < target)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }

    if (first == -1)
    {
        return {-1, -1};       // target not present at all
    }

    // --- Find LAST occurrence: bias toward hi when arr[mid] == target ---
    int last = -1;
    lo = 0; hi = n - 1;
    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target)
        {
            last = mid;
            lo = mid + 1;       // keep searching RIGHT for a later occurrence
        }
        else if (arr[mid] < target)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }

    return {first, last};
}

Interviewer follow-up they'll ask: "Can you do it with just lower_bound/upper_bound instead of writing two custom loops?" — Yes: first = lowerBound(target), and check arr[first] == target; last = upperBound(target) - 1.

Q11. Difference between std::binary_search, std::lower_bound, and std::equal_range?

Answer:

Function	Returns	Use case
std::binary_search(first, last, val)	bool — exists or not	Just need a yes/no
std::lower_bound(first, last, val)	iterator to first >= val	Insert position, or check existence via *it == val
std::upper_bound(first, last, val)	iterator to first > val	End of the equal-range
std::equal_range(first, last, val)	pair<iterator, iterator> = {lower_bound, upper_bound}	Both bounds in one call — internally still does two separate binary searches, so no asymptotic saving over calling both, just convenience
cpp
std::vector<int> v = {1, 3, 3, 3, 5, 7};

bool found = std::binary_search(v.begin(), v.end(), 3);          // true

auto lb = std::lower_bound(v.begin(), v.end(), 3);                // iterator to index 1
auto ub = std::upper_bound(v.begin(), v.end(), 3);                // iterator to index 4

int count = (int)(ub - lb);                                        // 3 occurrences

auto [rlo, rhi] = std::equal_range(v.begin(), v.end(), 3);         // same as {lb, ub}

Trap to know: all three assume the range is already sorted (or partitioned by the comparator you pass). Calling them on an unsorted range compiles fine and returns garbage silently — no exception, no assertion in release builds. This is a classic silent-bug interview question.

Category 4: Rotated Sorted Array
Q12. Search in a rotated sorted array (no duplicates) — explain the invariant that makes this work.

Answer: LC #33. The key insight: even though the whole array isn't sorted, at least one half around any mid always IS sorted. You check which half is sorted, then decide whether target could lie within that sorted half.

cpp
int searchRotated(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target)
        {
            return mid;
        }

        if (arr[lo] <= arr[mid])          // LEFT half [lo..mid] is sorted
        {
            if (arr[lo] <= target && target < arr[mid])
            {
                hi = mid - 1;              // target is within the sorted left half
            }
            else
            {
                lo = mid + 1;              // target must be in the right half
            }
        }
        else                                // RIGHT half [mid..hi] is sorted instead
        {
            if (arr[mid] < target && target <= arr[hi])
            {
                lo = mid + 1;               // target is within the sorted right half
            }
            else
            {
                hi = mid - 1;               // target must be in the left half
            }
        }
    }

    return -1;
}

Still O(log n) — you still discard half the search space every iteration, just the reasoning for which half to discard is more involved than in vanilla binary search.

Q13. Search in Rotated Sorted Array II (with duplicates) — why does the worst case become O(n)?

Answer: LC #81. With duplicates, arr[lo] == arr[mid] == arr[hi] is possible, and in that case you cannot tell which half is sorted (e.g. [3,1,2,3,3,3,3] — both halves "look" the same at the boundary values). The only safe move is to shrink the search space by one from each end and try again:

cpp
bool searchRotatedII(const std::vector<int>& arr, int target)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] == target)
        {
            return true;
        }

        if (arr[lo] == arr[mid] && arr[mid] == arr[hi])
        {
            ++lo;              // can't determine sorted half — shrink by 1 on each side
            --hi;
        }
        else if (arr[lo] <= arr[mid])
        {
            if (arr[lo] <= target && target < arr[mid]) hi = mid - 1;
            else                                          lo = mid + 1;
        }
        else
        {
            if (arr[mid] < target && target <= arr[hi]) lo = mid + 1;
            else                                          hi = mid - 1;
        }
    }

    return false;
}

Worst case: an array like [3,3,3,3,3,3,1,3] forces the ++lo; --hi; branch repeatedly, degrading to O(n) in the pathological all-duplicates case. This is a well-known example of duplicates breaking an otherwise-clean O(log n) algorithm — expect the interviewer to ask you to identify this degradation, not just implement the fix.

Q14. Find minimum in a rotated sorted array — what's the search invariant here?

Answer: LC #153. Compare arr[mid] to arr[hi] (not arr[lo] — this distinction matters):

cpp
int findMin(const std::vector<int>& arr)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] > arr[hi])
        {
            lo = mid + 1;       // minimum must be to the RIGHT of mid
        }
        else
        {
            hi = mid;            // arr[mid] <= arr[hi] means mid COULD be the minimum — keep it
        }
    }

    return arr[lo];              // lo == hi here
}

Why compare against arr[hi] and not arr[lo]? Comparing against arr[lo] gives ambiguous information when the array isn't rotated at all (arr[mid] >= arr[lo] is true both when we're in the sorted-from-the-start case AND in some rotated cases) — comparing against arr[hi] cleanly separates "the minimum is past mid" from "the minimum is at or before mid" in every case, rotated or not.

Q15. LC #154 (with duplicates) — how does this change the comparison logic?

Answer:

cpp
int findMinII(const std::vector<int>& arr)
{
    int lo = 0, hi = (int)arr.size() - 1;

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (arr[mid] > arr[hi])
        {
            lo = mid + 1;
        }
        else if (arr[mid] < arr[hi])
        {
            hi = mid;
        }
        else                     // arr[mid] == arr[hi] — ambiguous, can't rule out either side
        {
            --hi;                 // shrink by one, safe because arr[hi] itself is a duplicate
                                  // of a value we already know exists elsewhere in range
        }
    }

    return arr[lo];
}
// Same O(n) worst-case degradation as Q13, for the same reason: all-duplicate runs.
Category 5: Binary Search on Answer (Search Space Reduction)
Q16. What is "binary search on answer"? How do you recognize a problem needs this pattern?

Answer: Instead of searching for a value within a sorted array, you binary search over the space of possible answers (a range of numbers), using a monotonic predicate — a feasible(x) function — to decide which half of the answer-range to keep.

Recognition signals in a problem statement:

"Minimize the maximum X" or "Maximize the minimum X"
"Find the smallest/largest value such that [condition] holds"
The answer itself is a number in some bounded range (not an array index)
As the candidate answer increases, feasibility flips monotonically from false→true (or true→false) exactly once — never oscillates
cpp
// GENERIC TEMPLATE for "find smallest x in [lo,hi] such that feasible(x) is true"
// requires: feasible is false...false, true...true (monotonic)
long long binarySearchOnAnswer(long long lo, long long hi,
                                 std::function<bool(long long)> feasible)
{
    while (lo < hi)
    {
        long long mid = lo + (hi - lo) / 2;

        if (feasible(mid))
        {
            hi = mid;             // mid works — try to do even better (smaller)
        }
        else
        {
            lo = mid + 1;          // mid doesn't work — need to go bigger
        }
    }

    return lo;
}
Q17. Explain the monotonicity requirement with a concrete broken example.

Answer: If feasible(x) is not monotonic — e.g. it's true, false, true, false, true as x increases — binary search silently returns a wrong answer with no error or crash. It just picks whichever transition it happens to land on.

cpp
// A predicate that LOOKS reasonable but ISN'T monotonic:
// "is x a value that appears in this specific unsorted list?"
bool isMemberBAD(int x)
{
    static std::vector<int> allowed = {2, 7, 3, 9, 1};  // unsorted, arbitrary membership
    return std::find(allowed.begin(), allowed.end(), x) != allowed.end();
}
// binary search over [1,9] using isMemberBAD will find SOME true value,
// but not necessarily the smallest one, because true/false alternates
// non-monotonically across the range — this is a silent correctness bug,
// not a crash, which is exactly why it's dangerous in production code.

Interview tell: always explicitly state why your predicate is monotonic before coding — e.g. "if I can ship all packages within D days using capacity C, I can definitely also do it with any capacity > C" — that sentence IS the monotonicity proof, and a strong interviewer wants to hear you say it.

Q18. Koko Eating Bananas / Capacity to Ship Packages — walk through the reduction to binary search on answer.

Answer: LC #875 (Koko Eating Bananas). Answer space = possible eating speeds [1, max(piles)]. Predicate = "can Koko finish all piles within h hours at speed k?" — monotonic because a faster speed can never take more total hours.

cpp
int minEatingSpeed(std::vector<int>& piles, int h)
{
    int lo = 1;
    int hi = *std::max_element(piles.begin(), piles.end());

    auto canFinish = [&](int speed) -> bool
    {
        long long hoursNeeded = 0;

        for (int pile : piles)
        {
            hoursNeeded += (pile + speed - 1) / speed;   // ceiling division
        }

        return hoursNeeded <= h;
    };

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (canFinish(mid))
        {
            hi = mid;          // this speed works — try slower
        }
        else
        {
            lo = mid + 1;       // too slow — need to speed up
        }
    }

    return lo;
}

LC #1011 (Capacity to Ship Packages Within D Days) is structurally identical — answer space is [max(weights), sum(weights)], predicate is "can I ship all packages in days trips at capacity cap?" Recognizing that these are the same template with a different predicate is exactly what a 5-year interview is testing.

cpp
int shipWithinDays(std::vector<int>& weights, int days)
{
    int lo = *std::max_element(weights.begin(), weights.end());
    int hi = std::accumulate(weights.begin(), weights.end(), 0);

    auto canShip = [&](int capacity) -> bool
    {
        int daysNeeded = 1;
        int currentLoad = 0;

        for (int w : weights)
        {
            if (currentLoad + w > capacity)
            {
                ++daysNeeded;
                currentLoad = 0;
            }
            currentLoad += w;
        }

        return daysNeeded <= days;
    };

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;

        if (canShip(mid)) hi = mid;
        else               lo = mid + 1;
    }

    return lo;
}
Category 6: 2D Matrix
Q19. Search a 2D matrix — two different approaches and their complexities.

Answer:

Approach A — LC #74 (rows sorted, and first element of each row > last element of previous row — effectively one giant sorted sequence): treat the matrix as a flattened 1D array and binary search with index math.

cpp
bool searchMatrix(const std::vector<std::vector<int>>& mat, int target)
{
    int rows = (int)mat.size();
    int cols = (int)mat[0].size();

    int lo = 0, hi = rows * cols - 1;

    while (lo <= hi)
    {
        int mid = lo + (hi - lo) / 2;
        int val = mat[mid / cols][mid % cols];   // convert flat index -> (row, col)

        if (val == target)      return true;
        if (val < target)        lo = mid + 1;
        else                     hi = mid - 1;
    }

    return false;
}
// O(log(rows*cols)) — a single binary search over the whole matrix

Approach B — LC #240 (rows sorted left-right, columns sorted top-bottom, but no guarantee row N's last element < row N+1's first element — a weaker sortedness): flattening doesn't work here. Instead, start at the top-right corner and eliminate a full row or column each step:

cpp
bool searchMatrixII(const std::vector<std::vector<int>>& mat, int target)
{
    int rows = (int)mat.size();
    int cols = (int)mat[0].size();

    int row = 0, col = cols - 1;   // start top-right

    while (row < rows && col >= 0)
    {
        if (mat[row][col] == target)
        {
            return true;
        }
        else if (mat[row][col] > target)
        {
            --col;             // eliminate this whole column
        }
        else
        {
            ++row;              // eliminate this whole row
        }
    }

    return false;
}
// O(rows + cols) — NOT O(log(rows*cols))! This is technically NOT binary search
// at all — it's a staircase-elimination technique. A common interview trap is
// claiming this is O(log n) when it's actually linear in rows+cols.

The interviewer's real question here: can you tell the two matrix "sortedness" guarantees apart and pick the right algorithm for each? Applying Approach A's flattening trick to a #240-style matrix silently gives wrong answers, since mid / cols, mid % cols assumes global sortedness that doesn't hold.

Q20. Find the median in a row-wise sorted matrix — how is binary search applied here?

Answer: This is binary search on answer again (Category 5), applied to matrix values. Binary search over the value range [min, max] in the matrix; for each candidate value, count how many elements are <= mid across all rows using upper_bound per row (O(rows × log cols)); adjust the range until you land on the value where exactly (rows*cols)/2 + 1 elements are <= it.

cpp
int findMedian(const std::vector<std::vector<int>>& mat)
{
    int rows = (int)mat.size();
    int cols = (int)mat[0].size();

    int lo = INT_MAX, hi = INT_MIN;
    for (const auto& row : mat)
    {
        lo = std::min(lo, row.front());
        hi = std::max(hi, row.back());
    }

    int desired = (rows * cols) / 2 + 1;   // for odd total count

    while (lo < hi)
    {
        int mid = lo + (hi - lo) / 2;
        int countLessEqual = 0;

        for (const auto& row : mat)
        {
            countLessEqual += (int)(std::upper_bound(row.begin(), row.end(), mid) - row.begin());
        }

        if (countLessEqual < desired)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid;
        }
    }

    return lo;
}
// O(rows * log(cols) * log(value_range)) — binary search on answer, WITH a
// binary search (upper_bound) inside the predicate — a common "nested BS" pattern
Category 7: Floating-Point Binary Search
Q21. How does binary search on real numbers (e.g. computing sqrt(x)) differ from integer binary search in its termination condition?

Answer: With integers, lo == hi is a clean, unambiguous stopping point. With floating point, lo and hi can get arbitrarily close but never exactly equal within finite iterations for irrational answers — so you terminate either by a fixed iteration count or an epsilon tolerance:

cpp
double mySqrt(double x, double eps = 1e-9)
{
    if (x < 0)
    {
        throw std::invalid_argument("sqrt of negative number");
    }

    double lo = 0, hi = std::max(1.0, x);   // handles x < 1 correctly (sqrt(0.25)=0.5 > 0.25)

    // Option A: fixed iteration count (simplest, most common in interviews)
    for (int i = 0; i < 100; ++i)
    {
        double mid = lo + (hi - lo) / 2;

        if (mid * mid < x)
        {
            lo = mid;
        }
        else
        {
            hi = mid;
        }
    }

    return lo;

    // Option B: epsilon-based termination (equivalent, more "self-documenting")
    // while (hi - lo > eps)
    // {
    //     double mid = lo + (hi - lo) / 2;
    //     if (mid * mid < x) lo = mid;
    //     else                hi = mid;
    // }
    // return lo;
}

Why 100 iterations is "enough" and not arbitrary: each iteration halves the interval width. Starting from an interval of size ~10^9 (worst case), 100 iterations shrinks it by a factor of 2^100 ≈ 10^30 — far beyond double's ~15-17 significant decimal digits of precision. Past a certain iteration count you're just re-computing the same floating-point value repeatedly — the loop becomes a no-op, which is why 100 is a safe, standard choice regardless of the actual input range.

Category 8: STL Deep-Dive (You Already Have <algorithm> Basics — This Is the Senior Layer)
Q22. What happens if you call std::lower_bound on a range sorted in descending order?

Answer: Silently wrong — std::lower_bound's default comparator is std::less, which assumes ascending order. On a descending range, it returns a meaningless index with no error. Fix: pass an explicit comparator matching your actual ordering:

cpp
std::vector<int> desc = {9, 7, 5, 3, 1};

// WRONG — assumes ascending, gives garbage on descending data
auto it_wrong = std::lower_bound(desc.begin(), desc.end(), 5);

// CORRECT — tell lower_bound to use greater-than-based comparison
auto it_right = std::lower_bound(desc.begin(), desc.end(), 5, std::greater<int>());
Q23. std::partition_point — what is it, and how does it generalize lower_bound?

Answer: std::partition_point(first, last, pred) finds the boundary in a range that's already partitioned by a predicate (all elements where pred is true come before all elements where it's false) — this is exactly the "binary search on answer" pattern from Category 5, but as a ready-made STL algorithm instead of a hand-rolled loop:

cpp
std::vector<int> v = {1, 3, 5, 7, 9, 11};

// find first element NOT satisfying "< 7" — same result as lower_bound(v, 7)
auto it = std::partition_point(v.begin(), v.end(), [](int x)
{
    return x < 7;
});

std::cout << *it << '\n';   // 7

lower_bound is essentially partition_point specialized for the predicate x < target. Mentioning this connection is a strong senior-level signal — it shows you see the underlying pattern rather than memorizing each function in isolation.

Practice Problem Bank — By Pattern, Across All 5 Platforms

(LC = LeetCode, GFG = GeeksforGeeks, STV = Striver A2Z/SDE Sheet, CN = Coding Ninjas Studio, NKR = Naukri Coding 360)

Pattern 1 — Classic Binary Search
Platform	Problem	Difficulty
LC	#704 Binary Search	Easy
LC	#35 Search Insert Position	Easy
GFG	"Binary Search"	Easy
STV	Binary Search Day 1 — Basic BS	Easy
CN	"Implement Binary Search"	Easy
NKR	Binary Search fundamentals	Easy
Pattern 2 — Lower Bound / Upper Bound / Occurrences
Platform	Problem	Difficulty
LC	#34 Find First and Last Position of Element	Medium
LC	#278 First Bad Version	Easy
LC	#1385 Find the Distance Value Between Two Arrays	Easy
GFG	"First and Last Occurrence of X"	Easy
GFG	"Number of times a sorted array is rotated"	Easy
STV	"Floor and Ceil in Sorted Array"	Easy
STV	"Count occurrences in sorted array"	Easy
CN	"Ceiling in a sorted array"	Easy
NKR	Lower bound / upper bound practice	Easy
Pattern 3 — Rotated Sorted Array
Platform	Problem	Difficulty
LC	#33 Search in Rotated Sorted Array	Medium
LC	#81 Search in Rotated Sorted Array II	Medium
LC	#153 Find Minimum in Rotated Sorted Array	Medium
LC	#154 Find Minimum in Rotated Sorted Array II	Hard
GFG	"Search in Rotated Sorted Array"	Medium
GFG	"Minimum Number in a Rotated Sorted Array"	Medium
STV	"Search in Rotated Sorted Array I & II"	Medium
CN	"Rotated Sorted Array"	Medium
NKR	Rotated array search set	Medium
Pattern 4 — Peak Element / Mountain Array
Platform	Problem	Difficulty
LC	#162 Find Peak Element	Medium
LC	#852 Peak Index in a Mountain Array	Medium
LC	#1095 Find in Mountain Array	Hard
GFG	"Find a Peak Element"	Medium
STV	"Find Peak Element"	Medium
CN	"Peak Element"	Medium
Pattern 5 — Binary Search on Answer
Platform	Problem	Difficulty
LC	#875 Koko Eating Bananas	Medium
LC	#1011 Capacity To Ship Packages Within D Days	Medium
LC	#410 Split Array Largest Sum	Hard
LC	#1482 Minimum Number of Days to Make m Bouquets	Medium
LC	#1231 Divide Chocolate	Hard
LC	#1552 Magnetic Force Between Two Balls	Medium
GFG	"Aggressive Cows"	Hard
GFG	"Allocate Minimum Number of Pages"	Hard
GFG	"Painter's Partition Problem"	Hard
STV	"Book Allocation Problem"	Hard
STV	"Aggressive Cows"	Hard
CN	"Allocate Books"	Hard
NKR	Search-space reduction problem set	Hard
Pattern 6 — 2D Matrix
Platform	Problem	Difficulty
LC	#74 Search a 2D Matrix	Medium
LC	#240 Search a 2D Matrix II	Medium
LC	#378 Kth Smallest Element in a Sorted Matrix	Medium
LC	#668 Kth Smallest Number in Multiplication Table	Hard
GFG	"Median in a Row-wise Sorted Matrix"	Hard
GFG	"Search in a Row-Column Sorted Matrix"	Medium
STV	"Row with Maximum 1s" (binary search per row)	Medium
CN	"Matrix Median"	Hard
Pattern 7 — Floating Point / Numeric Binary Search
Platform	Problem	Difficulty
LC	#69 Sqrt(x)	Easy
LC	#367 Valid Perfect Square	Easy
LC	#4 Median of Two Sorted Arrays	Hard
GFG	"Square Root of a Number using Binary Search"	Easy
GFG	"Nth root of a number using binary search"	Medium
STV	"N-th Root of a Number"	Medium
CN	"Square Root: Precision"	Medium
Pattern 8 — Advanced / Mixed
Platform	Problem	Difficulty
LC	#4 Median of Two Sorted Arrays	Hard
LC	#1300 Sum of Mutated Array Closest to Target	Medium
LC	#1802 Maximum Value at a Given Index in a Bounded Array	Medium
LC	#786 K-th Smallest Prime Fraction	Hard
LC	#274 H-Index	Medium
LC	#275 H-Index II	Medium
LC	#658 Find K Closest Elements	Medium
GFG	"K-th Element of Two Sorted Arrays"	Hard
GFG	"Roti Prata Problem" (binary search on answer variant)	Hard
STV	"Median of 2 Sorted Arrays of different sizes"	Hard
STV	"Kth element of two sorted arrays"	Hard
CN	"Find K-th Element"	Hard
NKR	Advanced binary search assignment set	Hard
Quick-Fire Oral Drill (2-minute recap)
Time complexity          -> O(log n), proven via n/2^k = 1
Overflow-safe mid        -> lo + (hi - lo) / 2, never (lo + hi) / 2
lo <= hi                 -> closed interval, explicit found-check
lo < hi                  -> converges lo==hi, answer read AFTER loop (no mid outside!)
hi = mid vs hi = mid - 1 -> depends whether mid could STILL be the answer
lo = mid                 -> MUST round mid UP or you get an infinite loop
Rotated array trick      -> one half around mid is ALWAYS sorted — check which
Rotated + duplicates     -> arr[lo]==arr[mid]==arr[hi] forces O(n) worst case
Binary search on answer  -> predicate must be MONOTONIC — state why, out loud
2D matrix (LC 74)        -> flatten via mid/cols, mid%cols — needs GLOBAL sort
2D matrix (LC 240)       -> staircase from top-right — O(rows+cols), NOT log!
Float BS termination     -> fixed iteration count (~100) or epsilon, not lo==hi
lower_bound              -> first element >= target
upper_bound              -> first element >  target
count(target)            -> upper_bound(t) - lower_bound(t)

*/
