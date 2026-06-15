#include <iostream>
#include <string>

int main() 
{
	std::string s;

	if (!std::getline(std::cin, s)) 
	{
		std::cerr << "Input error\n";
		return 1;
	}

	// Input length guard
	if (s.length() > 100000) 
	{
		std::cerr << "Input too long\n";
		return 1;
	}

	std::size_t size = s.length();
	std::cout<<"size = '"<<size<<"'\n";
	// Edge cases: empty or single char are always palindromes
	if (size <= 1) 
	{
		std::cout << "Palindrome\n";
		return 0;
	}

	bool isP = true;  // assume true, disprove

	for (std::size_t i = 0; i < size / 2; i++) 
	{
		std::cout<<"s["<<i<<"] = '"<<s[i]<<"'\t"<<"s["<<size - i - 1<<"] = '"<<s[size - i - 1]<<"'\n";
		if (s[i] != s[size - i - 1]) 
		{
			isP = false;
			break;
		}
	}

	std::cout << (isP ? "Palindrome" : "Not Palindrome") << "\n";
	return 0;
}
