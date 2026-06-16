
#include <iostream>
#include <vector>
using namespace std;

// definition for singly-linked list

struct ListNode
{
	int val;
	ListNode *next;
	ListNode() : val(0), next(NULL) {}
	ListNode(int x) : val(x), next(NULL) {}
	ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
	public:
		ListNode* addTwoNumbers(ListNode*l1, ListNode*l2)
		{
			ListNode dummy(0);
			ListNode* current = &dummy;
			int carry = 0;

			while(l1!= NULL || l2!=NULL || carry) // to move next you need while() if use if() then only 1st add happen
			{
				int sum = carry;
				if(l1 != NULL)
				{
					sum += l1 -> val;
					l1=l1->next;
				}
				if(l2!=NULL)
				{
					sum += l2->val;
					l2=l2->next;
				}
				carry = sum /10;
				current -> next = new ListNode(sum % 10);
				current = current -> next;
			}
			return dummy.next;
		}
};

ListNode * createList(const vector<int>&vals)
{
	ListNode dummy(0);
	ListNode *current = &dummy;
	for(int v : vals)
	{
		current -> next= new ListNode(v);
		current = current->next;
	}
	return dummy.next;
}
void printList(ListNode* head)
{
	cout<<"[ ";
	while(head != NULL)
	{
		cout<<head->val;
		if(head->next != NULL) cout<<"'";

		head= head->next;
		
	}
	cout<<"]";
}

int main()
{

	Solution sol;

	vector<int>val1={2,4,3};
	vector<int>val2={5,6,4};
	ListNode * l1 = createList(val1);
	printList(l1);
	cout<<" + ";
	ListNode * l2 = createList(val2);
	printList(l2);
	ListNode * result = sol.addTwoNumbers(l1,l2);
	cout<<"Example 1 : ";
	printList(result);
	cout<<endl;


	return 0;
}
