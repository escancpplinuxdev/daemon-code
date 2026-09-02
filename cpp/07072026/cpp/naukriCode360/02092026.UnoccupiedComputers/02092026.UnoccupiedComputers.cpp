/*
Unoccupied Computers
Difficulty: EasyAccuracy: 52.2%Submissions: 7K+Points: 2Average Time: 20m

A cafe has n computers. The customer events are represented by a string s of uppercase English letters, where each distinct letter appears exactly twice:

    The first occurrence denotes the customer's arrival.
    The second occurrence denotes the customer's departure.

A customer is assigned a computer only if one is available at the time of arrival, otherwise the customer is rejected and does not use a computer.

Return the number of customers who could not be assigned a computer upon arrival.

Examples:

Input: n = 3, s = "GACCBDDBAGEE"
Output: 1
Explanation: Only D will not be able to get any computer. So the answer is 1.

Input: n = 1, s = "ABCBAC"
Output: 2
Explanation: B and C will not be able to get any computers. So the answer is 2.

class Solution {
  public:
    int solve(int n, string s) {
        // code here
        
    }
};

give this with int main()


*/


#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int solve(int n, string s) {
        // Map to track which computer a customer is using (-1 if rejected)
        unordered_map<char, int> computerAssigned;
        // Array to track occupied computers
        vector<bool> occupied(n, false);
        int available = n;
        int rejected = 0;
        
        for (char customer : s) {
            if (computerAssigned.find(customer) == computerAssigned.end()) {
                // Customer is arriving (first occurrence)
                if (available > 0) {
                    // Find the first free computer
                    for (int i = 0; i < n; i++) {
                        if (!occupied[i]) {
                            occupied[i] = true;
                            computerAssigned[customer] = i;
                            available--;
                            break;
                        }
                    }
                } else {
                    // No computers available - reject this customer
                    computerAssigned[customer] = -1;  // -1 means rejected
                    rejected++;
                }
            } else {
                // Customer is departing (second occurrence)
                int comp = computerAssigned[customer];
                if (comp != -1) {
                    // Customer was using a computer - free it
                    occupied[comp] = false;
                    available++;
                }
                // If comp == -1, customer was rejected - do nothing
                computerAssigned.erase(customer);
            }
        }
        
        return rejected;
    }
};

int main() {
    Solution solution;
    
    // Test case 1
    string s1 = "GACCBDDBAGEE";
    int n1 = 3;
    cout << "n=3, s=\"GACCBDDBAGEE\"" << endl;
    cout << "Output: " << solution.solve(n1, s1) << endl;
    // Expected: 1
    cout << endl;
    
    // Test case 2
    string s2 = "ABCBAC";
    int n2 = 1;
    cout << "n=1, s=\"ABCBAC\"" << endl;
    cout << "Output: " << solution.solve(n2, s2) << endl;
    // Expected: 2
    cout << endl;
    
    // Additional test cases
    string s3 = "ABBA";
    int n3 = 1;
    cout << "n=1, s=\"ABBA\": " << solution.solve(n3, s3) << endl;
    // Expected: 1
    
    string s4 = "ABCDDCBA";
    int n4 = 2;
    cout << "n=2, s=\"ABCDDCBA\": " << solution.solve(n4, s4) << endl;
    // Expected: 2
    
    string s5 = "ABAB";
    int n5 = 1;
    cout << "n=1, s=\"ABAB\": " << solution.solve(n5, s5) << endl;
    // Expected: 1
    
    string s6 = "ABCABC";
    int n6 = 2;
    cout << "n=2, s=\"ABCABC\": " << solution.solve(n6, s6) << endl;
    // Expected: 1
    
    string s7 = "AABB";
    int n7 = 1;
    cout << "n=1, s=\"AABB\": " << solution.solve(n7, s7) << endl;
    // Expected: 0
    
    return 0;
}


/*


🔍 Trace for n=3, s="GACCBDDBAGEE"

Let me trace through the string to verify:
text

s = G A C C B D D B A G E E
n = 3 computers available

Customer G: arrives → assign computer 0 → available=2
Customer A: arrives → assign computer 1 → available=1  
Customer C: arrives → assign computer 2 → available=0
Customer C: departs → free computer 2 → available=1
Customer B: arrives → assign computer 2 → available=0
Customer D: arrives → NO COMPUTER → REJECTED! → rejected=1
Customer D: departs → was rejected → skip
Customer B: departs → free computer 2 → available=1
Customer A: departs → free computer 1 → available=2
Customer G: departs → free computer 0 → available=3
Customer E: arrives → assign computer 0 → available=2
Customer E: departs → free computer 0 → available=3

Rejected customers: 1 (only D)
Output: 1 ✅

📊 Expected Output
text

n=3, s="GACCBDDBAGEE"
Output: 1

n=1, s="ABCBAC"
Output: 2

n=1, s="ABBA": 1
n=2, s="ABCDDCBA": 2
n=1, s="ABAB": 1
n=2, s="ABCABC": 1
n=1, s="AABB": 0

💡 Key Insight

The problem is the original code had a bug where computerOf.find(customer) == computerOf.end() was used incorrectly. Since we erase customers from the map when they depart, the condition computerOf.find(customer) == computerOf.end() works correctly for both:

    First occurrence (arrival) → customer not in map

    Second occurrence (departure) → customer already in map (unless they were rejected and erased, but they shouldn't be)

The key fix is:

    Arrival: if not in map → arrival

    Departure: if in map → departure

    When rejecting, store -1 in the map so departure knows the customer was rejected

This should now give the correct output of 1 for the test case!
*/
