// Definition for singly-linked list.
struct ListNode{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* detectCycle(ListNode* head){
    ListNode* fast=head;
    ListNode* slow=head;
    while(fast&&fast->next){
        fast=fast->next->next;
        slow=slow->next;
        if(fast==slow){
            while(slow!=head){
                head=head->next;
                slow=slow->next;
            }
            return head;
        }   
    }
    return nullptr;
}