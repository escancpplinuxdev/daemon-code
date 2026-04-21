ListNode* insertionSortList(ListNode* head)
{
    if (!head || !head->next)
        return head;

    ListNode* dummy = new ListNode(0); // dummy head (like index -1)
    dummy->next = head;

    ListNode* sorted = head;     // last node of sorted part
    ListNode* curr = head->next; // like i = 1

    sorted->next = nullptr; // break sorted list

    while (curr)  // for(i=1;i<n;i++)
    {
        ListNode* nextNode = curr->next; // store next (like i++ safe)

        ListNode* prev = dummy;
        ListNode* temp = dummy->next; // start from beginning (like j=i-1 but reset to head)

        // while(j>=0 && a[j] > key)
        while (temp && temp->val < curr->val)
        {
            prev = temp;           // move left pointer (like shifting position)
            temp = temp->next;     // j--
        }

        // insert at correct position (same as a[j+1] = key)
        prev->next = curr;
        curr->next = temp;

        curr = nextNode; // move to next unsorted node
    }

    ListNode* newHead = dummy->next;
    delete dummy;
    return newHead;
}


