#include <iostream>
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
		ListNode* insertionSortList(ListNode* head) 
		{
			// Edge cases: empty list or single node
			if (!head || !head->next)
				return head;

			// Dummy node simplifies insertion at the beginning
			ListNode* dummy = new ListNode(0);
			dummy->next = head;

			ListNode* sorted = head;     // last node of the sorted part
			ListNode* curr = head->next; // first unsorted node

			// Separate sorted part from the rest
			sorted->next = nullptr;

			while (curr) 
			{
				ListNode* nextNode = curr->next; // store next node before breaking

				// Find the insertion point in the sorted list
				ListNode* prev = dummy;
				ListNode* temp = dummy->next;
				while (temp && temp->val < curr->val) 
				{
					prev = temp;
					temp = temp->next;
				}

				// Insert curr between prev and temp
				prev->next = curr;
				curr->next = temp;

				// Move to the next unsorted node
				curr = nextNode;
			}

			ListNode* newHead = dummy->next;
			delete dummy;
			return newHead;
		}
};

// Helper function to print the linked list
void printList(ListNode* head) 
{
	while (head) 
	{
		cout << head->val;
		if (head->next) cout << " -> ";
		head = head->next;
	}
	cout << endl;
}

// Helper function to create a list from an array
ListNode* createList(int arr[], int n) 
{
	if (n == 0) return nullptr;
	ListNode* head = new ListNode(arr[0]);
	ListNode* curr = head;
	for (int i = 1; i < n; i++) 
	{
		curr->next = new ListNode(arr[i]);
		curr = curr->next;
	}
	return head;
}

int main() 
{
	// Example: unsorted list 5 -> 8 -> 6 -> 3 -> 9 -> 1 -> 7 -> 2 -> 4 -> 0
	int arr[] = {5, 8, 6, 3, 9, 1, 7, 2, 4, 0};
	int n = sizeof(arr) / sizeof(arr[0]);

	ListNode* head = createList(arr, n);

	cout << "Original list: ";
	printList(head);

	Solution sol;
	ListNode* sortedHead = sol.insertionSortList(head);

	cout << "Sorted list:   ";
	printList(sortedHead);

	// Free memory (optional, for completeness)
	while (sortedHead) 
	{
		ListNode* temp = sortedHead;
		sortedHead = sortedHead->next;
		delete temp;
	}

	return 0;
}
