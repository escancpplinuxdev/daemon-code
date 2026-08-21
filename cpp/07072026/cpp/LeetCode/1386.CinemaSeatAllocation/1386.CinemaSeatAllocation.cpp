#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    int maxNumberOfFamilies(int n, vector<vector<int>>& reservedSeats) 
    {
        // Map each row to a bitmask of reserved seats (only seats 2-9 matter)
        unordered_map<int, int> rowMask;
        
        for (auto& seat : reservedSeats) 
        {
            int row = seat[0];
            int col = seat[1];
            if (col >= 2 && col <= 9) 
            {
                rowMask[row] |= (1 << (col - 2));
            }
        }
        
        // For rows with no reservations, we can place 2 groups
        // Total rows = n, rows with reservations = rowMask.size()
        int maxGroups = (n - (int)rowMask.size()) * 2;
        
        // For rows that have reservations, check possibilities
        for (auto& entry : rowMask) 
        {
            int mask = entry.second;
            
            // Check if left block (2,3,4,5) is free
            bool leftFree = (mask & 0b1111) == 0;  // bits 0-3
            // Check if middle block (4,5,6,7) is free
            bool midFree = (mask & 0b00111100) == 0;  // bits 2-5
            // Check if right block (6,7,8,9) is free
            bool rightFree = (mask & 0b11110000) == 0;  // bits 4-7
            
            if (leftFree && rightFree) 
            {
                maxGroups += 2;
            } 
            else if (leftFree || midFree || rightFree) 
            {
                maxGroups += 1;
            }
        }
        
        return maxGroups;
    }
};

int main() 
{
    Solution sol;
    
    // Example 1
    vector<vector<int>> reservedSeats1 = {{1,2},{1,3},{1,8},{2,6},{3,1},{3,10}};
    cout << "Example 1: " << sol.maxNumberOfFamilies(3, reservedSeats1) << endl;  // Expected: 4
    
    // Example 2
    vector<vector<int>> reservedSeats2 = {{2,1},{1,8},{2,6}};
    cout << "Example 2: " << sol.maxNumberOfFamilies(2, reservedSeats2) << endl;  // Expected: 2
    
    // Example 3
    vector<vector<int>> reservedSeats3 = {{4,3},{1,4},{4,6},{1,7}};
    cout << "Example 3: " << sol.maxNumberOfFamilies(4, reservedSeats3) << endl;  // Expected: 4
    
    return 0;
}

/*

1386. Cinema Seat Allocation
Medium
Topics
premium lock iconCompanies
Hint

A cinema has n rows of seats, numbered from 1 to n. Each row has 10 seats, numbered from 1 to 10.

You are given a 2D integer array reservedSeats, where reservedSeats[i] = [rowi, seati] means that seat seati in row rowi is already reserved.

A four-person group must be assigned to four seats in the same row. The group can be seated in one of the following seat blocks:

    seats 2, 3, 4, 5
    seats 4, 5, 6, 7
    seats 6, 7, 8, 9

A block can be used only if none of its seats are reserved. Each seat can be assigned to at most one group.

Return an integer denoting the maximum number of four-person groups that can be assigned.

 

Example 1:

Input: n = 3, reservedSeats = [[1,2],[1,3],[1,8],[2,6],[3,1],[3,10]]
Output: 4
Explanation: The figure above shows an optimal allocation of four groups. Seats marked in blue are already reserved, and each set of four contiguous seats marked in orange is assigned to one group.

Example 2:

Input: n = 2, reservedSeats = [[2,1],[1,8],[2,6]]
Output: 2

Example 3:

Input: n = 4, reservedSeats = [[4,3],[1,4],[4,6],[1,7]]
Output: 4

class Solution {
public:
    int maxNumberOfFamilies(int n, vector<vector<int>>& reservedSeats) {
        
    }
};

give this with int main()

*/
