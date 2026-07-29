#include <iostream>
#include <cmath>

class Solution 
{
public:
    int minSteps(int n) 
    {
        if (n == 1) return 0;
        int ans = 0;
        int d = 2;
        while (d * d <= n) 
	{
            while (n % d == 0) 
	    {
                ans += d;
		std::cout<<"ans = "<<ans<<" \n";
		std::cout<<"n = "<<n<<" \n";
		std::cout<<"d = "<<d<<" \n";
                n /= d;
            }
            ++d;
	    std::cout<<"\n\n\n";
        }
        if (n > 1) 
	{
		std::cout<<"\nn>1 \nans = "<<ans<<" \n";
		std::cout<<"n = "<<n<<" \n";
		ans += n;
	}
        return ans;
    }
};

// Example usage
int main() 
{
    Solution sol;
    int n;
    std::cout << "Enter n: ";
    std::cin >> n;
    std::cout << "Min operations: " << sol.minSteps(n) << std::endl;
    return 0;
}
