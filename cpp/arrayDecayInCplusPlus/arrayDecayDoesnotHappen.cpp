
#include <iostream>

int main()
{
	int arr[5] = {10, 20, 30, 40, 50};

	// 1. sizeof — no decay
	std::cout << "sizeof(arr) = " << sizeof(arr) << " bytes\n";  // 5 * 4 = 20

	// 2. address-of operator — no decay, gives pointer to array
	int (*ptrToArray)[5] = &arr;
	std::cout << "(*ptrToArray)[2] = " << (*ptrToArray)[2] << '\n';  // 30

	// 3. binding to reference to array — no decay
	int (&ref)[5] = arr;
	std::cout << "ref[3] = " << ref[3] << '\n';  // 40

	return 0;
}
