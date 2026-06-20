
#include <iostream>

// Pass by reference to array – size is preserved
void printArray(int (&arr)[5])
{
	std::cout << "size preserved: sizeof(arr) = " << sizeof(arr) << '\n';  // 20
	for (int x : arr) std::cout << x << ' ';
	std::cout << '\n';
}

// Template version – works for any size
	template <std::size_t N>
void printArrayTemplate(int (&arr)[N])
{
	std::cout << "Template: size = " << N << '\n';
	for (int x : arr) std::cout << x << ' ';
	std::cout << '\n';
}

int main()
{
	int arr[5] = {1, 2, 3, 4, 5};
	printArray(arr);
	printArrayTemplate(arr);
	return 0;
}
