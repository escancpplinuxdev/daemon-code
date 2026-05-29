
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution
{
	public:
		int compress(vector<char>& chars)
		{
			int idx = 0;
			int n = chars.size();

			for (int i = 0; i < n; i++)
			{
				char ch = chars[i];
				int count = 0;

				while (i < n && chars[i] == ch)
				{
					count++;
					i++;
				}

				if (count == 1)
				{
					chars[idx++] = ch;
				}
				else
				{
					chars[idx++] = ch;
					string str = to_string(count);
					for (char dig : str)
					{
						chars[idx++] = dig;
					}
				}
				i--;  // adjust because for loop increments i
			}
			chars.resize(idx);
			return idx;
		}
};

void stringCompresion(vector<char>& chars)
{

	cout << "Input:  ";
	for (char c : chars) cout << c << " ";
	cout << endl;

	Solution sol;
	int newLength = sol.compress(chars);

	cout << "Output: ";
	for (int i = 0; i < newLength; i++)
	{
		cout << chars[i] << " ";
	}
	cout << endl;

}

int main()
{
	vector<char> chars1 = {'a', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'};
	stringCompresion(chars1);
	vector<char> chars2 = {'a'};
	stringCompresion(chars2);
	vector<char> chars3 = {'a','a','b','b','c','c','c'};
	stringCompresion(chars3);
	vector<char> chars4 = {'a','b','b','b','c','c','c'};
	stringCompresion(chars4);
	
	return 0;
}
