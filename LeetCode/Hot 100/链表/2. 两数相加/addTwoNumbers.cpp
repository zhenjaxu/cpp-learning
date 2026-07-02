// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

int flag=0;

ListNode* addTwoNumbers(ListNode* l1, ListNode* l2){
    if(!l1&&!l2) return flag?new ListNode(flag):nullptr;

    auto head=new ListNode();
    int val1=l1?l1->val:0;
    int val2=l2?l2->val:0;
    int sum=val1+val2+flag;
    head->val=sum%10;
    flag=sum/10;

    ListNode* next1=l1?l1->next:nullptr;
    ListNode* next2=l2?l2->next:nullptr;
    head->next=addTwoNumbers(next1, next2);

    return head;
}