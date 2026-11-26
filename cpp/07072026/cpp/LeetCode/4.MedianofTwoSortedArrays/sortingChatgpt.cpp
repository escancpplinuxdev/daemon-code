#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdint>
#include <limits>

using namespace std;

// --------------------------------------------------
// 1. BUBBLE SORT
// --------------------------------------------------
void bubbleSort(vector<int>& nums)
{
    int n = nums.size();

    for (int i = 0; i < n - 1; i++)
    {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++)
        {
            if (nums[j] > nums[j + 1])
            {
                swap(nums[j], nums[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
        {
            break;
        }
    }
}

// --------------------------------------------------
// 2. SELECTION SORT
// --------------------------------------------------
void selectionSort(vector<int>& nums)
{
    int n = nums.size();

    for (int i = 0; i < n - 1; i++)
    {
        int minIndex = i;

        for (int j = i + 1; j < n; j++)
        {
            if (nums[j] < nums[minIndex])
            {
                minIndex = j;
            }
        }

        swap(nums[i], nums[minIndex]);
    }
}

// --------------------------------------------------
// 3. INSERTION SORT
// --------------------------------------------------
void insertionSort(vector<int>& nums)
{
    int n = nums.size();

    for (int i = 1; i < n; i++)
    {
        int key = nums[i];
        int j = i - 1;

        while (j >= 0 && nums[j] > key)
        {
            nums[j + 1] = nums[j];
            j--;
        }

        nums[j + 1] = key;
    }
}

// --------------------------------------------------
// 4. MERGE SORT
// --------------------------------------------------
void merge(vector<int>& nums, int left, int mid, int right)
{
    vector<int> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right)
    {
        if (nums[i] <= nums[j])
        {
            temp.push_back(nums[i]);
            i++;
        }
        else
        {
            temp.push_back(nums[j]);
            j++;
        }
    }

    while (i <= mid)
    {
        temp.push_back(nums[i]);
        i++;
    }

    while (j <= right)
    {
        temp.push_back(nums[j]);
        j++;
    }

    for (int k = 0; k < temp.size(); k++)
    {
        nums[left + k] = temp[k];
    }
}

void mergeSort(vector<int>& nums, int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(nums, left, mid);
    mergeSort(nums, mid + 1, right);

    merge(nums, left, mid, right);
}

// --------------------------------------------------
// 5. QUICK SORT (THREE-WAY PARTITION)
// --------------------------------------------------
void quickSort(vector<int>& nums, int low, int high)
{
    if (low >= high)
    {
        return;
    }

    int pivot = nums[high];

    int smaller = low;
    int current = low;
    int greater = high;

    while (current <= greater)
    {
        if (nums[current] < pivot)
        {
            swap(nums[smaller], nums[current]);
            smaller++;
            current++;
        }
        else if (nums[current] > pivot)
        {
            swap(nums[current], nums[greater]);
            greater--;
        }
        else
        {
            current++;
        }
    }

    quickSort(nums, low, smaller - 1);
    quickSort(nums, greater + 1, high);
}

// --------------------------------------------------
// 6. HEAP SORT
// --------------------------------------------------
void heapify(vector<int>& nums, int n, int root)
{
    int largest = root;

    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if (left < n && nums[left] > nums[largest])
    {
        largest = left;
    }

    if (right < n && nums[right] > nums[largest])
    {
        largest = right;
    }

    if (largest != root)
    {
        swap(nums[root], nums[largest]);

        heapify(nums, n, largest);
    }
}

void heapSort(vector<int>& nums)
{
    int n = nums.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(nums, n, i);
    }

    // Extract maximum one by one
    for (int i = n - 1; i > 0; i--)
    {
        swap(nums[0], nums[i]);

        heapify(nums, i, 0);
    }
}

// --------------------------------------------------
// 7. COUNTING SORT
// Supports negative and positive integers.
// Memory depends on the value range.
// --------------------------------------------------
void countingSort(vector<int>& nums)
{
    if (nums.empty())
    {
        return;
    }

    auto minMax = minmax_element(nums.begin(), nums.end());

    int minValue = *minMax.first;
    int maxValue = *minMax.second;

    long long range =
        static_cast<long long>(maxValue) - minValue + 1;

    vector<size_t> count(static_cast<size_t>(range), 0);

    // Count occurrences
    for (int value : nums)
    {
        count[static_cast<size_t>(
            static_cast<long long>(value) - minValue
        )]++;
    }

    // Reconstruct sorted array
    int index = 0;

    for (long long i = 0; i < range; i++)
    {
        while (count[static_cast<size_t>(i)] > 0)
        {
            nums[index++] = static_cast<int>(i + minValue);
            count[static_cast<size_t>(i)]--;
        }
    }
}

// --------------------------------------------------
// 8. RADIX SORT
// Supports negative and positive 32-bit integers.
// Uses 4 byte-wise counting passes.
// --------------------------------------------------
void radixSort(vector<int>& nums)
{
    static_assert(sizeof(int) == sizeof(uint32_t),
                  "Radix sort requires 32-bit int");

    if (nums.empty())
    {
        return;
    }

    vector<int> output(nums.size());

    // Convert signed ordering into unsigned ordering.
    // This makes negative numbers come before positive numbers.
    const uint32_t signBit = 0x80000000u;

    for (int shift = 0; shift < 32; shift += 8)
    {
        size_t count[256] = {};

        // Count byte occurrences
        for (int value : nums)
        {
            uint32_t key =
                static_cast<uint32_t>(value) ^ signBit;

            uint32_t byte = (key >> shift) & 0xFFu;

            count[byte]++;
        }

        // Calculate cumulative positions
        for (int i = 1; i < 256; i++)
        {
            count[i] += count[i - 1];
        }

        // Stable placement (process from right to left)
        for (int i = static_cast<int>(nums.size()) - 1;
             i >= 0; i--)
        {
            uint32_t key =
                static_cast<uint32_t>(nums[i]) ^ signBit;

            uint32_t byte = (key >> shift) & 0xFFu;

            output[--count[byte]] = nums[i];
        }

        nums.swap(output);
    }
}

// --------------------------------------------------
// DISPLAY VECTOR
// --------------------------------------------------
void display(const vector<int>& nums)
{
    for (int value : nums)
    {
        cout << value << " ";
    }

    cout << endl;
}

// --------------------------------------------------
// MAIN FUNCTION
// --------------------------------------------------
int main()
{
    vector<int> nums = {5, 2, 9, 1, 5, 6};

    cout << "Original Array: ";
    display(nums);

    vector<int> arr;

    // 1. Bubble Sort
    arr = nums;
    bubbleSort(arr);

    cout << "1. Bubble Sort:    ";
    display(arr);

    // 2. Selection Sort
    arr = nums;
    selectionSort(arr);

    cout << "2. Selection Sort: ";
    display(arr);

    // 3. Insertion Sort
    arr = nums;
    insertionSort(arr);

    cout << "3. Insertion Sort: ";
    display(arr);

    // 4. Merge Sort
    arr = nums;
    mergeSort(arr, 0, static_cast<int>(arr.size()) - 1);

    cout << "4. Merge Sort:     ";
    display(arr);

    // 5. Quick Sort
    arr = nums;
    quickSort(arr, 0, static_cast<int>(arr.size()) - 1);

    cout << "5. Quick Sort:     ";
    display(arr);

    // 6. Heap Sort
    arr = nums;
    heapSort(arr);

    cout << "6. Heap Sort:      ";
    display(arr);

    // 7. Counting Sort
    arr = nums;
    countingSort(arr);

    cout << "7. Counting Sort:  ";
    display(arr);

    // 8. Radix Sort
    arr = nums;
    radixSort(arr);

    cout << "8. Radix Sort:     ";
    display(arr);

    return 0;
}

/*

4. Time and Space Complexity – Interview Table
Algorithm	Best Time	Average Time	Worst Time	Auxiliary Space

Bubble Sort	O(n)		O(n²)		O(n²)		O(1)
Selection Sort	O(n²)		O(n²)		O(n²)		O(1)
Insertion Sort	O(n)		O(n²)		O(n²)		O(1)
Merge Sort	O(n log n)	O(n log n)	O(n log n)	O(n)
Quick Sort	O(n log n)	O(n log n)	O(n²)		O(n) worst-case stack
Heap Sort	O(n log n)	O(n log n)	O(n log n)	O(1)
Counting Sort	O(n + k)	O(n + k)	O(n + k)	O(k)
Radix Sort	O(d(n + b))	O(d(n + b))	O(d(n + b))	O(n + b)


Where:
- n = number of elements.
- k = range of input values in Counting Sort.
- d = number of digit/byte passes in Radix Sort.
- b = radix/base (256 in this Radix Sort implementation).
5. Important Interview Points
1. Bubble Sort: Repeatedly compares adjacent elements and swaps them if they are in the wrong order.
2. Selection Sort: Finds the minimum element and places it at the beginning of the unsorted portion.
3. Insertion Sort: Takes one element at a time and inserts it into its correct position in the sorted portion.
4. Merge Sort: Divides the array into smaller parts, sorts them and merges them.
5. Quick Sort: Selects a pivot and partitions elements around it.
6. Heap Sort: Builds a max heap and repeatedly extracts the maximum element.
7. Counting Sort: Counts the frequency of each value. It is useful when the value range is reasonably small.
8. Radix Sort: Sorts values digit by digit or byte by byte using a stable counting-sort pass.
Important for your 5-year C++ interview: Be prepared to explain why std::sort() is generally preferred in production C++ code.
It provides O(n log n) worst-case complexity and is typically implemented using
->  introsort (a combination of quicksort, heapsort and insertion sort). The handwritten algorithms are important for understanding and interviews.


4. Time and Space Complexity – Interview Table
Algorithm       Best Time       Average Time    Worst Time      Auxiliary Space

Counting Sort   O(n + k)        O(n + k)        O(n + k)        O(k)
Radix Sort      O(d(n + b))     O(d(n + b))     O(d(n + b))     O(n + b)
Heap Sort       O(n log n)      O(n log n)      O(n log n)      O(1)
Merge Sort      O(n log n)      O(n log n)      O(n log n)      O(n)
Quick Sort      O(n log n)      O(n log n)      O(n²)           O(n) worst-case stack
Bubble Sort     O(n)            O(n²)           O(n²)           O(1)
Insertion Sort  O(n)            O(n²)           O(n²)           O(1)
Selection Sort  O(n²)           O(n²)           O(n²)           O(1)


*/



/*

8 Sorting Algorithms – Complete Dry Run and Complexity Calculation
Input vector:
vector<int> nums = {5, 2, 9, 1, 5, 6};

We will sort the numbers in ascending order using all 8 algorithms and understand:
1. Every pass and comparison.
2. Every swap or movement.
3. How the time complexity is calculated from loops and operations.
4. How auxiliary space is calculated from variables, arrays and recursion.
5. Why the algorithms have different best, average and worst cases.
1. Input analysis
Index:   0  1  2  3  4  5
Value:   5  2  9  1  5  6

Number of elements:
n = 6

Minimum value:
min = 1

Maximum value:
max = 9

For Counting Sort:
k = max - min + 1
  = 9 - 1 + 1
  = 9

For Radix Sort in our previous program:
d = 4 byte passes
b = 256 (possible byte values)
n = 6

Final sorted output for every algorithm:
1 2 5 5 6 9

2. Bubble Sort – Dry Run
Logic: Compare adjacent elements. If the left element is greater than the right element, swap them.



Compare 5 and 2 → swap
Scan → left to right: positions 1–7Fixed suffix grows ← from the right edge
Pass 1Comparison 1 of 61 swap



2

5

7

1

6

3

4


1
2
3
4
5
6
7










Swap
Result: 2, 5; 5 moves one place right

Sorting step





ShufflePlayStepFinish pass






Give feedback
Initial array:
5  2  9  1  5  6

Pass 1
Comparison	Condition	Action	Array after action
5, 2	5 > 2	Swap	2 5 9 1 5 6
5, 9	5 > 9	False	2 5 9 1 5 6
9, 1	9 > 1	Swap	2 5 1 9 5 6
9, 5	9 > 5	Swap	2 5 1 5 9 6
9, 6	9 > 6	Swap	2 5 1 5 6 9


After Pass 1:
2 5 1 5 6 | 9

Largest element 9 is in its correct position.
Pass 2
Comparison	Condition	Action	Array
2, 5	False	No swap	2 5 1 5 6 9
5, 1	True	Swap	2 1 5 5 6 9
5, 5	False	No swap	2 1 5 5 6 9
5, 6	False	No swap	2 1 5 5 6 9


After Pass 2:
2 1 5 5 | 6 9

Pass 3
Comparison	Condition	Action	Array
2, 1	True	Swap	1 2 5 5 6 9
2, 5	False	No swap	1 2 5 5 6 9
5, 5	False	No swap	1 2 5 5 6 9


After Pass 3:
1 2 5 | 5 6 9

Pass 4
1 2 5 5 6 9

No swap occurs, so the optimized algorithm stops.

Total comparisons:
5 + 4 + 3 + 3 = 15

The last pass performs 3 comparisons because the loop terminates after detecting that no swaps occurred.

Total swaps:
5 + 1 + 1 + 0 = 7

Time Complexity Calculation
The outer loop performs up to n - 1 passes.

The inner loop performs:
(n - 1) + (n - 2) + ... + 1

For n = 6:
5 + 4 + 3 + 2 + 1 = 15

Using the arithmetic series formula:
\[
\frac{n(n-1)}{2}
\]
For n = 6:
\[
\frac{6(6-1)}{2} = 15
\]
Therefore:
- Best: O(n), when the array is already sorted and the optimized version detects no swaps in the first pass.
- Average: O(n²).
- Worst: O(n²), when the array is reverse sorted.
Space Complexity Calculation
Additional variables:
int n;
int i;
int j;
bool swapped;

No additional array or recursion is used.

Number of extra variables remains constant regardless of input size.

Auxiliary Space = O(1).
3. Selection Sort – Dry Run
Logic: Find the smallest element from the unsorted portion and place it at the beginning.



Pass 1: scan the unsorted suffix
Comparison 1 of 7 comparisons0 sortedMinimum 4Comparing 1


4

1

7

3

8

5

2

6


1
2
3
4
5
6
7
8









Compare 1 with 4; the bars stay fixed while this pass scans

Drag to inspect




Shuffle + restartPlay throughStep onceFinish this pass






Give feedback
Initial array:
5 2 9 1 5 6

Pass 1
Find the minimum from index 0 to 5.
5 2 9 1 5 6
    |
Minimum = 1

Swap 5 and 1.
1 2 9 5 5 6

Pass 2
Find minimum from index 1 to 5.
1 | 2 9 5 5 6
    Minimum = 2

Already at correct position.
1 2 9 5 5 6

Pass 3
Find minimum from index 2 to 5.
1 2 | 9 5 5 6
       Minimum = 5

Swap 9 and 5.
1 2 5 9 5 6

Pass 4
Find minimum from index 3 to 5.
1 2 5 | 9 5 6
         Minimum = 5

Swap 9 and 5.
1 2 5 5 9 6

Pass 5
Find minimum from index 4 to 5.
1 2 5 5 | 9 6
           Minimum = 6

Swap 9 and 6.
1 2 5 5 6 9

Final output:
1 2 5 5 6 9

Time Complexity Calculation
The outer loop runs:
n - 1 = 5 times

The inner loop performs:
Pass 1 = 5 comparisons
Pass 2 = 4 comparisons
Pass 3 = 3 comparisons
Pass 4 = 2 comparisons
Pass 5 = 1 comparison

Total:
5 + 4 + 3 + 2 + 1 = 15

Formula:
\[
\frac{n(n-1)}{2}
\]
For n = 6:
\[
\frac{6 \times 5}{2} = 15
\]
The number of comparisons does not depend on whether the array is already sorted or reverse sorted.

Therefore:
- Best: O(n²).
- Average: O(n²).
- Worst: O(n²).
Space Complexity Calculation
Additional variables:
int n;
int i;
int j;
int minIndex;

No extra array or recursion.

Auxiliary Space = O(1).
4. Insertion Sort – Dry Run
Logic: Take one element and insert it into the correct position in the already sorted portion.



Start with one sorted value
Pass 1 of 61 sorted
Sorted prefix



5

2

9

1

5

6

7


1
2
3
4
5
6
7










Insertion step





ShufflePlayStepFinish pass





Give feedback
Initial array:
5 2 9 1 5 6

Pass 1
Key = 2

5 | 2 9 1 5 6

Compare:
5 > 2

Shift 5 right.
5 5 9 1 5 6

Insert key 2.
2 5 9 1 5 6

Pass 2
2 5 | 9 1 5 6

Key = 9

Compare:
5 > 9 = False

No shifting.
2 5 9 1 5 6

Pass 3
2 5 9 | 1 5 6

Key = 1

Comparisons and shifts:
9 > 1 → Shift 9
5 > 1 → Shift 5
2 > 1 → Shift 2

Insert 1.
1 2 5 9 5 6

Pass 4
1 2 5 9 | 5 6

Key = 5

Compare:
9 > 5 → Shift 9
5 > 5 → False

Insert 5.
1 2 5 5 9 6

Pass 5
1 2 5 5 9 | 6

Key = 6

Compare:
9 > 6 → Shift 9
5 > 6 → False

Insert 6.
1 2 5 5 6 9

Time Complexity Calculation
Outer loop:
for (int i = 1; i < n; i++)

Runs n - 1 times.

Best case: Already sorted.

Each key needs only one comparison.
Comparisons = n - 1
            = 6 - 1
            = 5

Best Time = O(n).

Worst case: Reverse-sorted array.

For each pass, all previously sorted elements need shifting.
Pass 1 = 1 shift
Pass 2 = 2 shifts
Pass 3 = 3 shifts
Pass 4 = 4 shifts
Pass 5 = 5 shifts

Total shifts:
1 + 2 + 3 + 4 + 5 = 15

Formula:
\[
\frac{n(n-1)}{2}
\]
For n = 6:
\[
\frac{6 \times 5}{2} = 15
\]
Therefore:
- Best: O(n).
- Average: O(n²).
- Worst: O(n²).
Space Complexity Calculation
Additional variables:
int n;
int i;
int key;
int j;

Only constant extra variables.

Auxiliary Space = O(1).
Continuing the dry run for the remaining four algorithms, using the exact same input:
vector<int> nums = {5, 2, 9, 1, 5, 6};

5. Merge Sort – Dry Run
Logic: Merge Sort uses Divide and Conquer:
1. Divide the array into two halves.
2. Continue dividing until each subarray contains one element.
3. Merge the subarrays in sorted order.



52915678







Divide · 0 comparisons
The values begin in their original order.

Merge step




8 · even7 · uneven





Shuffle and restart





Give feedback
Step 1: Divide the array
Original:
             5  2  9  1  5  6

Divide into two halves:
          [5  2  9]       [1  5  6]

Divide again:
       [5  2] [9]       [1  5] [6]

Divide again:
      [5] [2] [9]       [1] [5] [6]

Now every subarray has one element.
Step 2: Merge in sorted order
Merge [5] and [2]:
Compare 5 and 2

2 < 5

Result: [2 5]

Merge [2 5] and [9]:
Compare 2 and 9 → Take 2
Compare 5 and 9 → Take 5
Remaining 9

Result: [2 5 9]

On the right side, merge [1] and [5]:
Compare 1 and 5

Result: [1 5]

Merge [1 5] and [6]:
Compare 1 and 6 → Take 1
Compare 5 and 6 → Take 5
Remaining 6

Result: [1 5 6]

Step 3: Final merge
Now merge:
Left:  [2 5 9]
Right: [1 5 6]

Comparison	Selected value	Temporary array
2 vs 1	1	1
2 vs 5	2	1 2
5 vs 5	5	1 2 5
9 vs 5	5	1 2 5 5
9 vs 6	6	1 2 5 5 6
Remaining 9	9	1 2 5 5 6 9


Final:
1 2 5 5 6 9

Time Complexity Calculation
Merge Sort divides the input into two halves at each level.

For n = 6, the levels are approximately:
Level 0:       6 elements
Level 1:       3 + 3 elements
Level 2:       2 + 1 + 2 + 1 elements
Level 3:       Single-element subarrays

The number of division levels is approximately:
log₂(n)

At every level, merging processes all n elements.

Therefore:
Work per level = O(n)

Number of levels = O(log n)

Total = O(n) × O(log n)
      = O(n log n)

For n = 6:
log₂(6) ≈ 2.585

There are approximately 3 division levels.

The actual recursion tree has a maximum of 3 splitting levels, and merging across each level processes a total of 6 elements.

Therefore:
- Best: O(n log n).
- Average: O(n log n).
- Worst: O(n log n).
Space Complexity Calculation
In our implementation:
vector<int> temp;

During merging, temporary storage is required for the elements being merged.

Maximum temporary elements:
n = 6

Temporary array space:
O(n)

Recursive function calls also use stack space:
O(log n)

Total:
O(n) + O(log n)

The O(n) temporary array dominates.

Auxiliary Space = O(n).
6. Quick Sort – Dry Run
Logic: Quick Sort selects a pivot and partitions the array:
- Values smaller than the pivot go to the left.
- Values equal to the pivot stay in the middle.
- Values greater than the pivot go to the right.
Our previous program uses a three-way partition and selects the last element as the pivot.

Initial array:
5 2 9 1 5 6

Step 1: First partition
Pivot:
pivot = 6

We maintain three regions:
[Less than pivot] [Equal to pivot] [Unknown] [Greater than pivot]

Initial:
5 2 9 1 5 6
          ↑
        Pivot

Process each element:
Current value	Comparison with 6	Action	Array
5	5 < 6	Move to smaller region	5 2 9 1 5 6
2	2 < 6	Move to smaller region	5 2 9 1 5 6
9	9 > 6	Move to greater region	5 2 5 1 9 6
1	1 < 6	Move to smaller region	5 2 5 1 9 6
9	Already greater	Continue	5 2 5 1 9 6
6	6 == 6	Equal region	5 2 5 1 6 9


Partition result:
5 2 5 1 | 6 | 9

Now 6 is in its correct position.
Step 2: Sort left partition
Left partition:
5 2 5 1

Pivot:
pivot = 1

All other elements are greater than 1.

Result:
1 | 2 5 5

Step 3: Sort remaining left partition
Subarray:
2 5 5

Pivot:
pivot = 5

Partition:
2 | 5 5

Both 5s are equal to the pivot.
Step 4: Combine partitions
Left:       1 2 5 5
Pivot:              6
Right:                9

Final sorted array:
1 2 5 5 6 9

Time Complexity Calculation
Quick Sort performance depends on how balanced the partitions are.

Best case:

Every partition divides the array approximately into two equal halves.
Level 0:       n
Level 1:      n/2 + n/2
Level 2:     n/4 + n/4 + n/4 + n/4

Each level processes approximately n elements.

Number of levels:
log₂(n)

Therefore:
O(n log n)

Average case:

Partitions are reasonably balanced on average.

Therefore:
O(n log n)

Worst case:

One partition contains n - 1 elements and the other contains zero elements.

Example: A poor pivot repeatedly chosen from an already sorted array.
Level 0: 6 elements
Level 1: 5 elements
Level 2: 4 elements
Level 3: 3 elements
Level 4: 2 elements
Level 5: 1 element

Total work:
5 + 4 + 3 + 2 + 1
= 15

General formula:
\[
\frac{n(n-1)}{2}
\]
Therefore:
Worst Time = O(n²)

Important: Our three-way partition handles duplicate values efficiently. However, because the pivot is always the last element, an unfavourable pivot can still produce O(n²) worst-case time.
Space Complexity Calculation
Our implementation uses recursive calls:
quickSort(nums, low, smaller - 1);
quickSort(nums, greater + 1, high);

Best/average case:
Recursion depth = O(log n)

Worst case:
Recursion depth = O(n)

No separate array is allocated.

Therefore:
- Best/Average auxiliary space: O(log n).
- Worst auxiliary space: O(n).
7. Heap Sort – Dry Run
Logic: Heap Sort uses a Binary Max Heap.

A Max Heap has the property:
Parent >= Left Child
Parent >= Right Child

The largest element is always at the root.

Initial array:
5 2 9 1 5 6

Step 1: Build Max Heap
For an array, the child indexes are:
Left child  = 2 * i + 1
Right child = 2 * i + 2

For n = 6, start heapifying from:
i = n/2 - 1
  = 6/2 - 1
  = 2

Initial tree:
             5
           /   \
          2     9
         / \   /
        1   5 6

Heapify index 2:

Node = 9

Child = 6

9 is already greater.

No swap.

Heapify index 1:

Node = 2

Children = 1 and 5

Largest = 5

Swap 2 and 5.

Array:
5 5 9 1 2 6

Heapify index 0:

Node = 5

Children = 5 and 9

Largest = 9

Swap 5 and 9.

Array:
9 5 5 1 2 6

Max Heap tree:
             9
           /   \
          5     5
         / \   /
        1   2 6

Step 2: Extract Maximum
Swap root with last element:
9 5 5 1 2 6

After swap:
6 5 5 1 2 | 9

Heapify remaining 5 elements:
5 2 5 1 6 9

Now the largest element 6 is in its correct position.

Next extraction:
5 2 5 1 | 6 9

Swap root 5 with last element of heap. Since both are 5, no visible change.

Next extraction:
1 2 5 | 5 6 9

Next extraction:
2 1 | 5 5 6 9

Heapify:
2 1 5 5 6 9

Next extraction:
1 | 2 5 5 6 9

Final:
1 2 5 5 6 9

Time Complexity Calculation
Heap Sort has two major stages.

Stage 1: Build Max Heap

Building a heap takes:
O(n)

It is not O(n log n), because most nodes are near the bottom and require very little heap adjustment.

Stage 2: Extract Maximum

For every extraction:
- Swap root with last heap element.
- Heapify the root.
- Heapify takes O(log n).
There are n - 1 extractions.

Therefore:
O(n) + O(n log n)

Dominant term:
O(n log n)

For n = 6, there are 5 extractions, and the heap height is approximately:
log₂(6) ≈ 2.585

Each extraction may require up to approximately 3 levels of adjustment.

Therefore:
- Best: O(n log n).
- Average: O(n log n).
- Worst: O(n log n).
Space Complexity Calculation
Our implementation uses:
void heapify(vector<int>& nums, int n, int root)

It modifies the original vector.

It uses a constant number of variables:
int largest;
int left;
int right;

The heapify function is recursive, but the recursion depth is O(log n).

Technical correction: The previous complexity table showed Heap Sort as O(1) auxiliary space. That is true for the conventional iterative heapify implementation. Our recursive heapify() implementation uses O(log n) call-stack space in the worst case.

If we change heapify() to an iterative implementation, auxiliary space becomes O(1).
8. Counting Sort – Dry Run
Logic: Counting Sort counts how many times each number occurs and reconstructs the sorted array.

It is especially useful when the input value range is small.

Initial:
5 2 9 1 5 6

Minimum:
min = 1

Maximum:
max = 9

Range:
k = max - min + 1

k = 9 - 1 + 1

k = 9

Step 1: Create Counting Array
Index represents the value.
Value:  1  2  3  4  5  6  7  8  9
Count:  0  0  0  0  0  0  0  0  0

Step 2: Count Frequencies
Input:
5 2 9 1 5 6

Process one by one:
Input value	Action	Count array (values 1–9)
5	Increment count[5]	0 0 0 0 1 0 0 0 0
2	Increment count[2]	0 1 0 0 1 0 0 0 0
9	Increment count[9]	0 1 0 0 1 0 0 0 1
1	Increment count[1]	1 1 0 0 1 0 0 0 1
5	Increment count[5]	1 1 0 0 2 0 0 0 1
6	Increment count[6]	1 1 0 0 2 1 0 0 1


Final frequency array:
Value:  1  2  3  4  5  6  7  8  9
Count:  1  1  0  0  2  1  0  0  1

Step 3: Reconstruct Sorted Array
Read the count array from left to right.
1 occurs 1 time → 1
2 occurs 1 time → 2
3 occurs 0 times
4 occurs 0 times
5 occurs 2 times → 5 5
6 occurs 1 time → 6
7 occurs 0 times
8 occurs 0 times
9 occurs 1 time → 9

Final output:
1 2 5 5 6 9

Time Complexity Calculation
Counting Sort performs three operations:
1. Find minimum and maximum: O(n).
2. Count occurrences: O(n).
3. Reconstruct output: O(n + k).
Total:
O(n) + O(n) + O(n + k)

Remove constants and combine:
O(n + k)

For our example:
n = 6
k = 9

n + k = 6 + 9 = 15

Therefore:
- Best: O(n + k).
- Average: O(n + k).
- Worst: O(n + k).
Important: Counting Sort is not always faster than comparison-based sorting. If the range of values is enormous compared with the number of elements, its memory and runtime costs can become impractical.
Space Complexity Calculation
Our implementation creates:
vector<size_t> count(range, 0);

The size of this array is:
k = 9

No second output vector is used.

Therefore:
Auxiliary Space = O(k)

The original vector is sorted in place using the count array.
9. Radix Sort – Dry Run
Logic: Radix Sort sorts numbers digit by digit or byte by byte using a stable sorting operation.

Our previous program uses LSD Radix Sort with 4 byte-wise counting passes, because an ordinary 32-bit integer contains 4 bytes.

For the input:
5 2 9 1 5 6

We can understand the sorting using decimal digits first. It produces the same final ordering as the byte-wise implementation.
Step 1: Understand decimal digits
All numbers are single-digit numbers.
Number:  5  2  9  1  5  6
Digit:   5  2  9  1  5  6

Pass 1: Sort by units digit
Since every number has only one digit, the units digit is the complete number.
Input:

5 2 9 1 5 6

Sorted by units digit:

1 2 5 5 6 9

Final:
1 2 5 5 6 9

No further decimal digit passes are needed for these particular values.

But there is an important difference: The program we wrote does not use one decimal digit pass. It uses four byte passes, including passes for the higher bytes.

For positive numbers from 0 to 9, the higher three bytes are all zero, so those passes leave the ordering unchanged.
Time Complexity Calculation
Our implementation uses:
for (int shift = 0; shift < 32; shift += 8)

Passes:
Shift 0  → First byte
Shift 8  → Second byte
Shift 16 → Third byte
Shift 24 → Fourth byte

Therefore:
d = 4

For each pass:
1. Count byte frequencies: O(n).
2. Calculate cumulative positions: O(b).
3. Place elements into output: O(n).
Total per pass:
O(n + b)

For d passes:
O(d(n + b))

For our example:
n = 6
b = 256
d = 4

d(n + b)
= 4(6 + 256)
= 4 × 262
= 1048

This is the operation-growth expression for the implementation, not a literal count of 1,048 CPU operations.

Therefore:
- Best: O(d(n + b)).
- Average: O(d(n + b)).
- Worst: O(d(n + b)).
For fixed-width 32-bit integers, d = 4 and b = 256 are constants, so the complexity is effectively O(n) with respect to the number of input elements.
Space Complexity Calculation
Our program creates:
vector<int> output(nums.size());

This needs n additional elements.

It also creates:
size_t count[256] = {};

This requires b additional counters.

Therefore:
Auxiliary Space = O(n + b)

For our example:
n = 6
b = 256

Extra output elements = 6
Extra counters = 256

10. Complete Time Complexity Comparison – Ascending Order of Worst-Case Growth
Here is the requested table, arranged from lower to higher worst-case growth.
Order	Algorithm	Best Time	Average Time	Worst Time	Auxiliary Space
1	Counting Sort	O(n + k)	O(n + k)	O(n + k)	O(k)
2	Radix Sort	O(d(n + b))	O(d(n + b))	O(d(n + b))	O(n + b)
3	Heap Sort	O(n log n)	O(n log n)	O(n log n)	O(log n)*
4	Merge Sort	O(n log n)	O(n log n)	O(n log n)	O(n)
5	Quick Sort	O(n log n)	O(n log n)	O(n²)	O(n) worst-case
6	Bubble Sort	O(n)	O(n²)	O(n²)	O(1)
7	Insertion Sort	O(n)	O(n²)	O(n²)	O(1)
8	Selection Sort	O(n²)	O(n²)	O(n²)	O(1)


*Our recursive Heap Sort implementation uses O(log n) auxiliary call-stack space. An iterative heapify implementation uses O(1).

Important clarification about ordering: Counting Sort and Radix Sort are not universally ordered relative to each other. Their performance depends on k, d, and b. Likewise, Heap Sort and Merge Sort both have O(n log n) worst-case time; their order in this table is just a presentation order, not a claim that Heap Sort is always faster.
11. Understand the Difference Between Time Complexity and Space Complexity
This is a common interview question.
Time Complexity
Time complexity tells us how the number of operations grows as input size increases.

For example, consider Bubble Sort:
for (int i = 0; i < n - 1; i++)
{
    for (int j = 0; j < n - i - 1; j++)
    {
        if (nums[j] > nums[j + 1])
        {
            swap(nums[j], nums[j + 1]);
        }
    }
}

Two nested loops:
Outer loop = n - 1

Inner loop = n - i - 1

Total comparisons:
\[
(n-1)+(n-2)+\cdots+1
\]
\[
=\frac{n(n-1)}{2}
\]
Expanding:
\[
=\frac{n^2-n}{2}
\]
Big-O removes constants and lower-order terms:
\[
O(n^2)
\]
Space Complexity
Space complexity tells us how much additional memory an algorithm needs as input size increases.

For example, Bubble Sort:
int n = nums.size();

for (int i = 0; i < n - 1; i++)
{
    bool swapped = false;
    // ...
}

Additional memory:
n          → 1 integer
i          → 1 integer
j          → 1 integer
swapped    → 1 boolean

Regardless of whether the vector contains 6, 100 or 1,000,000 elements, these additional variables remain constant.

Therefore:
O(1)

Compare this with Merge Sort:
vector<int> temp;

The temporary vector may contain all n elements.

Therefore:
O(n)

12. How Complexity Changes When Input Size Increases
Suppose we increase the number of elements.
Algorithm	n = 10	n = 100	n = 1,000	Growth
O(1)	1	1	1	Constant
O(log n)	~3	~7	~10	Logarithmic
O(n)	10	100	1,000	Linear
O(n log n)	~33	~664	~9,966	Linearithmic
O(n²)	100	10,000	1,000,000	Quadratic


These are illustrative growth values, not measured execution times.



O(1)
O(log n)
O(n)
O(n log n)
O(n²)
O(2ⁿ)

10203040506050100150200Input size nGrowth





\(O(1):\;\text{1},\quad O(\log_2 n):\;\text{4}\)
\(O(n):\;\text{16},\quad O(n\log_2 n):\;\text{64}\)
\(O(n^2):\;\text{256},\quad O(2^n):\;\text{65,536}\)

View
\(\text{All curves}\)\(O(1)\)\(O(\log_2 n)\)\(O(n)\)\(O(n\log_2 n)\)\(O(n^2)\)\(O(2^n)\)




\(n\)
48163264










Give feedback
How to explain this to the interviewer:

If input size increases from 100 to 1,000:
- O(n) grows approximately 10 times.
- O(n log n) grows approximately 15 times.
- O(n²) grows 100 times.
This is why O(n log n) sorting algorithms are generally preferred over O(n²) sorting algorithms for large datasets.
13. Important Corrections and Interview Traps
These points are worth remembering from the actual implementations.
Question	Correct answer
Is Selection Sort O(n) when the array is already sorted?	No. It still performs O(n²) comparisons.
Is Insertion Sort O(n) for an already sorted array?	Yes.
Is optimized Bubble Sort O(n) for an already sorted array?	Yes.
Is Merge Sort O(n log n) even for sorted input?	Yes, in this implementation.
Is Quick Sort always O(n log n)?	No. Worst case is O(n²).
Is Heap Sort O(1) space in every implementation?	No. Our recursive heapify uses O(log n) stack space.
Does Counting Sort work with negative numbers?	Yes, our implementation offsets values using the minimum.
Does Radix Sort work with negative numbers?	Yes, our implementation transforms signed ordering using the sign bit.
Is Counting Sort a comparison-based sort?	No.
Is Radix Sort a comparison-based sort?	No.
Is std::sort() stable?	No. Use std::stable_sort() when stability is required.


One correction to the earlier Bubble Sort dry run: For the optimized code provided, the comparisons are 5 + 4 + 3 + 2 = 14, not 15. The final pass detects that no swaps occurred and stops. The worst-case comparison count remains 15 for n = 6.
14. Final Interview Answer: Which Sorting Algorithm Would You Choose?
If an interviewer asks:

"You have 1 million integers. Which sorting algorithm will you use?"

A good answer is:

"I would generally prefer std::sort() from the C++ STL. It provides O(n log n) worst-case complexity and typically uses introsort, combining quicksort, heapsort and insertion sort.

If I need a stable sort, I would consider std::stable_sort().

If the input contains integers with a small, known value range, Counting Sort can be more efficient.

If I am sorting fixed-width integers and the data characteristics are suitable, Radix Sort is another option.

The final choice depends on input distribution, memory constraints, stability requirements and performance measurements."
15. What to Memorize for Your 5-Year C++ Interview
Algorithm	Main concept	Most important complexity
Bubble	Adjacent swapping	O(n²)
Selection	Find minimum	O(n²)
Insertion	Shift and insert	O(n²)
Merge	Divide and conquer	O(n log n)
Quick	Pivot and partition	O(n²) worst
Heap	Max heap	O(n log n)
Counting	Frequency array	O(n + k)
Radix	Digit/byte-wise sorting	O(d(n + b))


My recommendation: Practise writing Bubble, Selection, Insertion, Merge, Quick and Heap Sort without looking at the code. Then focus on explaining the complexity calculations. For a 5-year C++ Linux developer interview, being able to derive complexity from the actual loops, recursion and memory allocations is much more valuable than simply memorizing the Big-O table.

*/

