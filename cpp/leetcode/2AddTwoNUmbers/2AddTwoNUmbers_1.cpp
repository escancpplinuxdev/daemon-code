
#include <iostream>
#include <vector>
using namespace std;

struct ListNode
{
	int val;
	ListNode *next;
	ListNode() : val(0), next (NULL) {}
	ListNode(int x) : val(x), next(NULL) {}
	ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
	public:
		ListNode *addTwoNumbers(ListNode*l1, ListNode*l2)
		{
		
			ListNode dummy(0);
			ListNode *current= &dummy;
			int carry = 0;
			while(l1 != NULL || l2 !=NULL ||carry)
			{
				int sum = carry;
				if(l1 != NULL)
				{
					sum += l1->val;
					l1=l1->next;
				}
				if(l2 != NULL)
				{
					sum += l2->val;
					l2=l2->next;
				}
				carry=sum/10;
				current -> next=new ListNode(sum%10); // if we not add new ListNode(sum%10) to curent->next we lose track of next address.
				current=current->next;
			}
			return dummy.next;
		}
};
//If we not add new ListNode(sum%10) to curent->next we lose track of next address.

//If we wrote current = new ListNode(...), we would lose the link from the previous node.

//We need the previous node’s next pointer to point to the new node so the list stays connected.

//So we always:

//    Attach the new node to current->next.

//    Advance current to that new node.
    
ListNode * createNode(const vector<int>&vals)
{
	ListNode dummy(0);
	ListNode *current = &dummy;
	for(int v : vals)
	{
		current -> next = new ListNode(v);
		current = current -> next;
	}
	return dummy.next;
}

void PrintList ( ListNode * head)
{
	cout<<"[";
	while(head != NULL)
	{
		cout<<head->val;
		if(head->next != NULL)cout<<",";
		head=head->next;
	}
	cout<<"]";
}
int main()
{

	Solution sol;

	vector<int>val1={2,4,6}; // addition happen as  2 -> 4 -> 6
	vector<int>val2={5,6,4}; //		     +	5 -> 6 -> 4 
				//		       [7  , 0	, 1 , 1]  
	ListNode* l1=createNode(val1);
	PrintList(l1);
	cout<<endl;
	ListNode* l2=createNode(val2);
	PrintList(l2);
	cout<<endl;

	ListNode* result=sol.addTwoNumbers(l1,l2);

	PrintList(result);
	cout<<endl;
	return 0;
}
