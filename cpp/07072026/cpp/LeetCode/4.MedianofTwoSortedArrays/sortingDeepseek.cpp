// all_sorts.cpp
// Compile: g++ -std=c++17 -O2 -Wall -Wextra all_sorts.cpp -o all_sorts
// Run:     ./all_sorts

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// ============================================================
// Utility: print a vector
// ============================================================
static void printVec(const std::string &label, const std::vector<int> &v)
{
	std::cout << label << ": [ ";
	for (int x : v)
		std::cout << x << ' ';
	std::cout << "]\n";
}

// ============================================================
// 1. BUBBLE SORT
//    Time: O(n^2) worst/avg, O(n) best (already sorted, with flag)
//    Space: O(1) auxiliary
//    Stable: Yes
// ============================================================
void bubbleSort(std::vector<int> &a)
{
	const int n = static_cast<int>(a.size());
	for (int i = 0; i < n - 1; ++i)
	{
		bool swapped = false;
		for (int j = 0; j < n - 1 - i; ++j)
		{
			if (a[j] > a[j + 1])
			{
				std::swap(a[j], a[j + 1]);
				swapped = true;
			}
		}
		if (!swapped)
			break; // already sorted
	}
}

// ============================================================
// 2. SELECTION SORT
//    Time: O(n^2) worst/avg/best
//    Space: O(1) auxiliary
//    Stable: No (standard in-place version swaps non-adjacent)
// ============================================================
void selectionSort(std::vector<int> &a)
{
	const int n = static_cast<int>(a.size());
	for (int i = 0; i < n - 1; ++i)
	{
		int minIdx = i;
		for (int j = i + 1; j < n; ++j)
			if (a[j] < a[minIdx])
				minIdx = j;
		if (minIdx != i)
			std::swap(a[i], a[minIdx]);
	}
}

// ============================================================
// 3. INSERTION SORT
//    Time: O(n^2) worst/avg, O(n) best (already sorted)
//    Space: O(1) auxiliary
//    Stable: Yes
// ============================================================
void insertionSort(std::vector<int> &a)
{
	const int n = static_cast<int>(a.size());
	for (int i = 1; i < n; ++i)
	{
		int key = a[i];
		int j = i - 1;
		while (j >= 0 && a[j] > key)
		{
			a[j + 1] = a[j];
			--j;
		}
		a[j + 1] = key;
	}
}

// ============================================================
// 4. MERGE SORT (top-down, stable)
//    Time: O(n log n) worst/avg/best
//    Space: O(n) auxiliary
//    Stable: Yes
// ============================================================
static void mergeSortImpl(std::vector<int> &a, std::vector<int> &tmp, int lo,	int hi)
{
	if (lo >= hi)
		return;
	int mid = lo + (hi - lo) / 2;
	mergeSortImpl(a, tmp, lo, mid);
	mergeSortImpl(a, tmp, mid + 1, hi);

	// merge [lo..mid] and [mid+1..hi] into tmp
	int i = lo, j = mid + 1, k = lo;
	while (i <= mid && j <= hi)
		tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
	while (i <= mid)
		tmp[k++] = a[i++];
	while (j <= hi)
		tmp[k++] = a[j++];
	for (int p = lo; p <= hi; ++p)
		a[p] = tmp[p];
}

void mergeSort(std::vector<int> &a)
{
	if (a.size() < 2)
		return;
	std::vector<int> tmp(a.size());
	mergeSortImpl(a, tmp, 0, static_cast<int>(a.size()) - 1);
}

// ============================================================
// 5. QUICK SORT (Lomuto partition, last element as pivot)
//    Time: O(n log n) avg, O(n^2) worst (already sorted + bad pivot)
//    Space: O(log n) avg recursion stack, O(n) worst
//    Stable: No
// ============================================================
static int partitionLomuto(std::vector<int> &a, int lo, int hi)
{
	int pivot = a[hi];
	int i = lo - 1;
	for (int j = lo; j < hi; ++j)
	{
		if (a[j] <= pivot)
		{
			++i;
			std::swap(a[i], a[j]);
		}
	}
	std::swap(a[i + 1], a[hi]);
	return i + 1;
}

static void quickSortImpl(std::vector<int> &a, int lo, int hi)
{
	if (lo >= hi)
		return;
	int p = partitionLomuto(a, lo, hi);
	quickSortImpl(a, lo, p - 1);
	quickSortImpl(a, p + 1, hi);
}

void quickSort(std::vector<int> &a)
{
	if (a.size() < 2)
		return;
	quickSortImpl(a, 0, static_cast<int>(a.size()) - 1);
}

// ============================================================
// 6. HEAP SORT (max-heap, in-place)
//    Time: O(n log n) worst/avg/best
//    Space: O(1) auxiliary
//    Stable: No
// ============================================================
static void siftDown(std::vector<int> &a, int n, int i)
{
	while (true)
	{
		int largest = i;
		int l = 2 * i + 1;
		int r = 2 * i + 2;
		if (l < n && a[l] > a[largest])
			largest = l;
		if (r < n && a[r] > a[largest])
			largest = r;
		if (largest == i)
			break;
		std::swap(a[i], a[largest]);
		i = largest;
	}
}

void heapSort(std::vector<int> &a)
{
	const int n = static_cast<int>(a.size());
	// build max-heap
	for (int i = n / 2 - 1; i >= 0; --i)
		siftDown(a, n, i);
	// extract max repeatedly
	for (int end = n - 1; end > 0; --end)
	{
		std::swap(a[0], a[end]);
		siftDown(a, end, 0);
	}
}

// ============================================================
// 7. COUNTING SORT (non-negative ints only)
//    Time: O(n + k) where k = max value
//    Space: O(k) auxiliary
//    Stable: Yes (with the cumulative-count technique below)
// ============================================================
void countingSort(std::vector<int> &a)
{
	if (a.empty())
		return;
	int maxVal = *std::max_element(a.begin(), a.end());
	if (maxVal < 0)
		return; // not supported (negatives)
	std::vector<int> count(maxVal + 1, 0);
	for (int x : a)
		++count[x];
	int idx = 0;
	for (int v = 0; v <= maxVal; ++v)
		while (count[v]-- > 0)
			a[idx++] = v;
}

// ============================================================
// 8. RADIX SORT (LSD, base 10, non-negative ints)
//    Time: O(d * (n + b)) where d = digits, b = base (10)
//    Space: O(n + b) auxiliary
//    Stable: Yes
// ============================================================
static void countingSortByDigit(std::vector<int> &a, int exp)
{
	const int n = static_cast<int>(a.size());
	std::vector<int> output(n);
	int count[10] = {0};
	for (int x : a)
		++count[(x / exp) % 10];
	for (int i = 1; i < 10; ++i)
		count[i] += count[i - 1];
	for (int i = n - 1; i >= 0; --i)
	{
		int digit = (a[i] / exp) % 10;
		output[--count[digit]] = a[i];
	}
	a = output;
}

void radixSort(std::vector<int> &a)
{
	if (a.empty())
		return;
	int maxVal = *std::max_element(a.begin(), a.end());
	if (maxVal < 0)
		return; // not supported (negatives)
	for (int exp = 1; maxVal / exp > 0; exp *= 10)
		countingSortByDigit(a, exp);
}

// ============================================================
// main: run every sort on the same input and verify
// ============================================================
int main()
{
	const std::vector<int> original = {5, 2, 9, 1, 5, 6};

	std::cout << "Original input:\n";
	printVec("  nums", original);
	std::cout << '\n';

	// Pair of (name, function) — using function pointers to keep it simple
	struct Case
	{
		const char *name;
		void (*fn)(std::vector<int> &);
	};
	const Case cases[] = {
		{"Bubble Sort", bubbleSort},       {"Selection Sort", selectionSort},
		{"Insertion Sort", insertionSort}, {"Merge Sort", mergeSort},
		{"Quick Sort", quickSort},         {"Heap Sort", heapSort},
		{"Counting Sort", countingSort},   {"Radix Sort", radixSort},
	};

	bool allOk = true;
	for (const auto &c : cases)
	{
		std::vector<int> v = original; // fresh copy each time
		c.fn(v);

		std::vector<int> expected = original;
		std::sort(expected.begin(), expected.end());
		bool ok = (v == expected);
		allOk = allOk && ok;

		std::cout << (ok ? "[OK]   " : "[FAIL] ") << c.name << '\n';
		printVec("        result", v);
	}

	std::cout << '\n'
		<< (allOk ? "All sorts produced correct output.\n"
				: "Some sorts FAILED.\n");
	return allOk ? 0 : 1;
}

/*

Quick comparison table
Sort	Best	Average	Worst	Space	Stable?	Works on
Bubble	O(n)	O(n²)	O(n²)	O(1)	✅	any comparable
Selection	O(n²)	O(n²)	O(n²)	O(1)	❌	any comparable
Insertion	O(n)	O(n²)	O(n²)	O(1)	✅	any comparable
Merge	O(n log n)	O(n log n)	O(n log n)	O(n)	✅	any comparable
Quick	O(n log n)	O(n log n)	O(n²)	O(log n)	❌	any comparable
Heap	O(n log n)	O(n log n)	O(n log n)	O(1)	❌	any comparable
Counting	O(n+k)	O(n+k)	O(n+k)	O(k)	✅	non-negative ints
Radix (LSD)	O(d(n+b))	O(d(n+b))	O(d(n+b))	O(n+b)	✅	non-negative ints

k = range of values, d = number of digits, b = base (10 here).
Notes for the interviewer / production

    Merge sort is stable and predictable O(n log n) — preferred when stability or worst-case guarantees matter. std::stable_sort uses it (or a variant).

    Quick sort is the fastest in practice on average due to cache locality, but worst-case O(n²) — production code uses introsort (std::sort in libstdc++): quicksort + heapsort fallback + insertion sort for small ranges.

    Heap sort gives O(n log n) in-place with O(1) extra space — good when memory is tight.

    Counting/Radix beat comparison sorts asymptotically but require integer keys in a bounded range; negatives need an offset or a split-and-merge pass.

    Insertion sort is the best choice for tiny or nearly-sorted arrays — that's why real implementations switch to it below a size threshold (typically 16).

    The input {5, 2, 9, 1, 5, 6} contains a duplicate 5 — good test for stability (Merge/Counting/Radix/Bubble/Insertion preserve relative order; Selection/Quick/Heap may not).

Want me to add instrumentation (comparison/swap counters per algorithm), a benchmark harness comparing them on random inputs of size 10⁴–10⁶, or a negatives-supporting radix sort?

*/
