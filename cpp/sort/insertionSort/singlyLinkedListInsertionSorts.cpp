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

			while (curr) // 								for(i=1;i<n;i++)
			{
				ListNode* nextNode = curr->next; // store next node before breaking ;

				// Find the insertion point in the sorted list
				ListNode* prev = dummy;
				ListNode* temp = dummy->next;						// key=a[i];
	//                             								 while (j>=0 && a[j] > key)
				while (temp && curr->val > temp->val) 
				{
					prev = temp;							 // a[j+1]=a[j];
					temp = temp->next;						 // j--;
				}

				// Insert curr between prev and temp
				prev->next = curr;
				curr->next = temp; 							// a[j+1]=key;

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
ListNode* createList(int a[], int n) 
{
	if (n == 0) return nullptr;
	ListNode* head = new ListNode(a[0]);
	ListNode* curr = head;
	for (int i = 1; i < n; i++) 
	{
		curr->next = new ListNode(a[i]);
		curr = curr->next;
	}
	return head;
}

int main() 
{
	// Example: unsorted list 5 -> 8 -> 6 -> 3 -> 9 -> 1 -> 7 -> 2 -> 4 -> 0
	int a[] = {5, 8, 6, 3, 9, 1, 7, 2, 4, 0};
	int n = sizeof(a) / sizeof(a[0]);

	ListNode* head = createList(a, n);

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
