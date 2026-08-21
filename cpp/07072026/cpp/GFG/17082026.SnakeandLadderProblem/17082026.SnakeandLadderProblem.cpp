#include <bits/stdc++.h>
using namespace std;

class Solution 
{
public:
    int minThrows(int n, vector<int>& lad, vector<int>& sn) 
	{
        int total = n * n;
        vector<int> board(total + 1, 0);
        
        // Ladders: lad[2*i] -> start, lad[2*i+1] -> end
        for (int i = 0; i < (int)lad.size(); i += 2) 
	{
            board[lad[i]] = lad[i + 1];
        }
        // Snakes: similar
        for (int i = 0; i < (int)sn.size(); i += 2) 
	{
            board[sn[i]] = sn[i + 1];
        }
        
        // BFS from cell 1 to total
        vector<int> dist(total + 1, -1);
        queue<int> q;
        dist[1] = 0;
        q.push(1);
        
        while (!q.empty()) 
	{
            int u = q.front(); q.pop();
            if (u == total) break;
            
            for (int dice = 1; dice <= 6; ++dice) 
	{
                int v = u + dice;
                if (v > total) continue;
                if (board[v] != 0) v = board[v];   // snake or ladder
                if (dist[v] == -1) 
	{
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        
        return dist[total];
    }
};

// Example usage
int main() 
{
    Solution sol;
    
    // Example 1
    vector<int> lad1 = {3, 22, 5, 8, 11, 35, 20, 32};
    vector<int> sn1  = {17, 4, 19, 7, 34, 1, 21, 9};
    cout << "Example 1: " << sol.minThrows(6, lad1, sn1) << endl;  // Expected 3
    
    // Example 2
    vector<int> lad2 = {2, 8};
    vector<int> sn2  = {7, 3};
    cout << "Example 2: " << sol.minThrows(3, lad2, sn2) << endl;  // Expected 2
    
    return 0;
}

/*

Snake and Ladder Problem
Difficulty: MediumAccuracy: 34.65%Submissions: 62K+Points: 4Average Time: 20m

Given an integer n such that there is n × n Snakes and Ladders board with cells numbered from 1 to n*n, find the minimum number of dice throws required to reach cell n*n starting from cell 1. Given two arrays of even lengths:

    lad[], where each pair (lad[2*i], lad[2*i + 1]) represents the start and end of a ladder.
    sn[], where each pair (sn[2*i], sn[2*i + 1]) represents the start and end of a snake.

If you land on the start cell of a snake or ladder, you must immediately move to its corresponding end cell.

You have complete control over the outcome of each dice throw i.e., in a single move,  you can move forward by any number of cells from 1 to 6. 

If it is impossible to reach cell n*n, return -1.

Examples:

Input: n = 6, lad[] = [3, 22, 5, 8, 11, 35, 20, 32], sn[] = [17, 4, 19, 7, 34, 1, 21, 9]
Output: 3
Explanation: For the 6 × 6 board, the minimum number of dice throws needed to reach cell 36 from cell 1 is 3.
One optimal path is:
Throw 4 to move from 1 to 5, then take the ladder to 8
Throw 3 to move from 8 to 11, then take the ladder to 35
Throw 1 to move from 35 to 36
So the destination is reached in 3 dice throws.

Input: n = 3, lad[] = [2, 8], sn[] = [7, 3]
Output: 2
Explanation: For the 3 × 3 board, the minimum number of dice throws needed to reach cell 9 from cell 1 is 2.
One optimal path is:
Throw 1 to move from 1 to 2, then take the ladder to 8.
Throw 1 to move from 8 to 9.
So the destination is reached in 2 dice throws.

class Solution {
  public:
    int minThrows(int n, vector<int>& lad, vector<int>& sn) {
        // code here
        
    }
};


*/
