
#include <iostream>
#include <vector>
#include <algorithm>   // for std::max

int maxA(int n) 
{
	if (n <= 0) return 0;

	std::vector<int> dp(n + 1, 0);

	for (int i = 1; i <= n; ++i) 
	{
		std::cout<<"i = "<<i<<"\n";
		// Option 1: just press 'A'
		dp[i] = dp[i - 1] + 1;
		std::cout<<"outer dp["<<i<<"] = "<<dp[i]<<"\n";


		// Option 2: copy-paste from some previous state j
		// Need at least 3 presses: Ctrl-A, Ctrl-C, and one Ctrl-V
		for (int j = 1; j <= i - 3; ++j) 
		{
			std::cout<<"j = "<<j<<"\n";
			int multiply = i - j - 1;   // (t + 1) where t = number of pastes
			std::cout<<"multiply = "<<multiply<<"\n";
			std::cout<<"dp["<<j<<"] * multiply = "<<dp[j] * multiply<<"\n";
			dp[i] = std::max(dp[i], dp[j] * multiply);
			std::cout<<"max dp["<<i<<"] = "<<dp[i]<<"   <--\n";
		}
		std::cout<<"\n\n\n";
	}
			std::cout<<"Final dp["<<n<<"] = "<<dp[n]<<"   <--\n";

	return dp[n];
}

int main() 
{
	int n;
	std::cout << "Enter n: ";
	std::cin >> n;
	std::cout << "Max A's: \n" << maxA(n) << std::endl;
	return 0;
}
