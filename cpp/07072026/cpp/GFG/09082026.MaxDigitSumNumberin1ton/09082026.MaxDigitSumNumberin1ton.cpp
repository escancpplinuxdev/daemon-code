


#include <iostream>
#include <string>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    int findMax(int n) {
        string s = to_string(n);
        int len = s.length();
        
        // Option 1: The number itself
        int maxSum = digitSum(n);
        int result = n;
        
        // Option 2: Try making each prefix smaller and fill rest with 9s
        for (int i = 0; i < len; i++) {
            if (s[i] == '0') continue;
            
            // Create number: prefix - 1 + 9s
            string candidate = s;
            candidate[i] = char(candidate[i] - 1);  // Decrease current digit by 1
            
            // Fill remaining digits with 9
            for (int j = i + 1; j < len; j++) {
                candidate[j] = '9';
            }
            
            // Skip if leading zero
            if (candidate[0] == '0') {
                // Remove leading zero: e.g., "099" -> "99"
                candidate = candidate.substr(1);
                // If candidate is empty, skip
                if (candidate.empty()) continue;
            }
            
            int num = stoi(candidate);
            int sum = digitSum(num);
            
            if (sum > maxSum || (sum == maxSum && num > result)) {
                maxSum = sum;
                result = num;
            }
        }
        
        // ✅ FIX: Option 3 - Try numbers with fewer digits (all 9s)
        // For n = 160155241, this will consider 99999999
        for (int digits = len - 1; digits >= 1; digits--) {
            string candidate = string(digits, '9');
            int num = stoi(candidate);
            int sum = digitSum(num);
            
            if (sum > maxSum || (sum == maxSum && num > result)) {
                maxSum = sum;
                result = num;
            }
        }
        
        return result;
    }
    
private:
    int digitSum(int num) {
        int sum = 0;
        while (num > 0) {
            sum += num % 10;
            num /= 10;
        }
        return sum;
    }
};

int main() {
    Solution solution;
    
    // Example 1
    cout << "Example 1: n=48 -> " << solution.findMax(48) << endl;
    // Expected: 48
    
    // Example 2
    cout << "Example 2: n=90 -> " << solution.findMax(90) << endl;
    // Expected: 89
    
    // Failing test case
    cout << "Test 3: n=160155241 -> " << solution.findMax(160155241) << endl;
    // Expected: 99999999
    
    // Additional test cases
    cout << "Test 4: n=9 -> " << solution.findMax(9) << endl;
    // Expected: 9
    
    cout << "Test 5: n=10 -> " << solution.findMax(10) << endl;
    // Expected: 9
    
    cout << "Test 6: n=19 -> " << solution.findMax(19) << endl;
    // Expected: 19
    
    cout << "Test 7: n=20 -> " << solution.findMax(20) << endl;
    // Expected: 19
    
    cout << "Test 8: n=99 -> " << solution.findMax(99) << endl;
    // Expected: 99
    
    cout << "Test 9: n=100 -> " << solution.findMax(100) << endl;
    // Expected: 99
    
    cout << "Test 10: n=101 -> " << solution.findMax(101) << endl;
    // Expected: 99 (sum=18) vs 101 (sum=2)
    
    return 0;
}


/*

Max Digit Sum Number in 1 to n
Difficulty: EasyAccuracy: 31.76%Submissions: 5K+Points: 2

Given a number n, find a number in the range from 1 to n such that its digit sum is maximum. If there are multiple such numbers, return the largest of them.

Examples:

Input: n = 48
Output: 48
Explanation: There are two numbers with maximum digit sum = 12. The numbers are 48 and 39. Since 48 > 39, so 48 is the answer.

Input: n = 90
Output: 89
Explanation: 89 gives us the largest digit sum in the range from 1 to n. Hence the answer is 89. 

class Solution {
  public:
    int findMax(int n) {
        // code Here
        
    }
};


give this with int main include header files

*/
