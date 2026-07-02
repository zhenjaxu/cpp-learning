// Definition for singly-linked list.
struct ListNode{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode *getIntersectionNode(ListNode *headA, ListNode *headB){
    if(!headA&&!headB) return nullptr;
        ListNode* pA=headA;
        ListNode* pB=headB;
        while(pA!=pB){
            pA=pA?pA->next:headB;
            pB=pB?pB->next:headA;
        }
        return pA;      // 如果不相交，pA==nullptr
}