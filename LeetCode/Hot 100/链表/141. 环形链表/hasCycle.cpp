// Definition for singly-linked list.
struct ListNode{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

bool hasCycle(ListNode *head) {
    if(!head||!head->next) return false;
    ListNode* fast=head->next;
    ListNode* slow=head;
    while(fast->next&&fast->next->next)
    {
        if(fast==slow) return true;
        fast=fast->next->next;
        slow=slow->next;
    }
    return false;
}