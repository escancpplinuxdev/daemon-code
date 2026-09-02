#include <bits/stdc++.h>
using namespace std;

// Definition for singly-linked list.
struct ListNode 
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution 
{
public:
    vector<int> nodesBetweenCriticalPoints(ListNode* head) 
    {
        vector<int> criticalPositions;
        int pos = 1; // 1-indexed position
        ListNode* prev = head;
        ListNode* curr = head->next;
        
        while (curr != nullptr && curr->next != nullptr) 
        {
            // Check if curr is a critical point
            if ((curr->val > prev->val && curr->val > curr->next->val) ||
                (curr->val < prev->val && curr->val < curr->next->val)) 
            {
                criticalPositions.push_back(pos);
            }
            prev = curr;
            curr = curr->next;
            pos++;
        }
        
        if (criticalPositions.size() < 2) 
        {
            return {-1, -1};
        }
        
        int minDist = INT_MAX;
        int maxDist = criticalPositions.back() - criticalPositions.front();
        
        for (int i = 1; i < criticalPositions.size(); i++) 
        {
            minDist = min(minDist, criticalPositions[i] - criticalPositions[i - 1]);
        }
        
        return {minDist, maxDist};
    }
};

// Helper function to build linked list from vector
ListNode* buildList(vector<int>& nums) 
{
    if (nums.empty()) return nullptr;
    ListNode* head = new ListNode(nums[0]);
    ListNode* curr = head;
    for (int i = 1; i < nums.size(); i++) 
    {
        curr->next = new ListNode(nums[i]);
        curr = curr->next;
    }
    return head;
}

// Helper function to print vector
void printVector(vector<int>& v) 
{
    cout << "[";
    for (int i = 0; i < v.size(); i++) 
    {
        cout << v[i];
        if (i < v.size() - 1) cout << ",";
    }
    cout << "]";
}

int main() 
{
    Solution sol;
    
    // Example 1: [3,1] -> [-1,-1]
    vector<int> nums1 = {3, 1};
    ListNode* head1 = buildList(nums1);
    vector<int> result1 = sol.nodesBetweenCriticalPoints(head1);
    cout << "Example 1: ";
    printVector(result1);
    cout << endl;  // Expected: [-1,-1]
    
    // Example 2: [5,3,1,2,5,1,2] -> [1,3]
    vector<int> nums2 = {5, 3, 1, 2, 5, 1, 2};
    ListNode* head2 = buildList(nums2);
    vector<int> result2 = sol.nodesBetweenCriticalPoints(head2);
    cout << "Example 2: ";
    printVector(result2);
    cout << endl;  // Expected: [1,3]
    
    // Example 3: [1,3,2,2,3,2,2,2,7] -> [3,3]
    vector<int> nums3 = {1, 3, 2, 2, 3, 2, 2, 2, 7};
    ListNode* head3 = buildList(nums3);
    vector<int> result3 = sol.nodesBetweenCriticalPoints(head3);
    cout << "Example 3: ";
    printVector(result3);
    cout << endl;  // Expected: [3,3]
    
    return 0;
}


/*

2058. Find the Minimum and Maximum Number of Nodes Between Critical Points
Solved
Medium
Topics
premium lock iconCompanies
Hint

A critical point in a linked list is defined as either a local maxima or a local minima.

A node is a local maxima if the current node has a value strictly greater than the previous node and the next node.

A node is a local minima if the current node has a value strictly smaller than the previous node and the next node.

Note that a node can only be a local maxima/minima if there exists both a previous node and a next node.

Given a linked list head, return an array of length 2 containing [minDistance, maxDistance] where minDistance is the minimum distance between any two distinct critical points and maxDistance is the maximum distance between any two distinct critical points. If there are fewer than two critical points, return [-1, -1].

 

Example 1:

Input: head = [3,1]
Output: [-1,-1]
Explanation: There are no critical points in [3,1].

Example 2:

Input: head = [5,3,1,2,5,1,2]
Output: [1,3]
Explanation: There are three critical points:
- [5,3,1,2,5,1,2]: The third node is a local minima because 1 is less than 3 and 2.
- [5,3,1,2,5,1,2]: The fifth node is a local maxima because 5 is greater than 2 and 1.
- [5,3,1,2,5,1,2]: The sixth node is a local minima because 1 is less than 5 and 2.
The minimum distance is between the fifth and the sixth node. minDistance = 6 - 5 = 1.
The maximum distance is between the third and the sixth node. maxDistance = 6 - 3 = 3.

Example 3:

Input: head = [1,3,2,2,3,2,2,2,7]
Output: [3,3]
Explanation: There are two critical points:
- [1,3,2,2,3,2,2,2,7]: The second node is a local maxima because 3 is greater than 1 and 2.
- [1,3,2,2,3,2,2,2,7]: The fifth node is a local maxima because 3 is greater than 2 and 2.
Both the minimum and maximum distances are between the second and the fifth node.
Thus, minDistance and maxDistance is 5 - 2 = 3.
Note that the last node is not considered a local maxima because it does not have a next node.


*/
