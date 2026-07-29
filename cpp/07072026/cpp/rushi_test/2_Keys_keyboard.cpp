
#include <iostream>
#include <vector>
#include <climits>   // for INT_MAX

int minSteps(int n) 
{
	if (n == 1) return 0;

	std::vector<int> dp(n + 1, INT_MAX);
	dp[1] = 0;

	for (int i = 2; i <= n; ++i) 
	{
		std::cout<<"i = "<<i<<"\n";
		for (int j = 1; j < i; ++j) 
		{
			std::cout<<"j = "<<j<<"\n";
			if (i % j == 0) 
			{
				// we can get i by copying from j and pasting (i/j - 1) times
				int steps = dp[j] + i / j;
				
				std::cout<<"steps = "<<steps<<" = "<<dp[j]<<"+"<<i<<"/"<<j<<"\n";
				std::cout<<"dp["<<i<<"] = "<<dp[i]<<"\n";
/*
				if (steps < dp[i])
				{
					dp[i] = steps;
					std::cout<<"dp["<<i<<"] = "<<dp[i]<<"\n";
				}
				*/
				dp[i] = std::min(steps,dp[i]);
			}
		}
		std::cout<<"\n\n";
	}

	return dp[n];
}

int main() 
{
	int n;
	std::cout << "Enter n: ";
	std::cin >> n;
	std::cout << "Min operations: " << minSteps(n) << std::endl;
	return 0;
}
