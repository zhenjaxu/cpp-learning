// Definition for singly-linked list.
struct ListNode{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* swapPairs(ListNode* head){
    ListNode* dummyHead=new ListNode(0);
    dummyHead->next=head;

    ListNode* temp=dummyHead;
    while(temp->next&&temp->next->next){
        ListNode* node1=temp->next;
        ListNode* node2=temp->next->next;
        
        temp->next=node2;
        node1->next=node2->next;
        node2->next=node1;
        temp=node1;
    }

    ListNode* ans=dummyHead->next;
    delete dummyHead;
    return ans;
}