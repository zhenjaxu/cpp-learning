// Definition for singly-linked list.
struct ListNode{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution{
    ListNode* pointer;                          // 从头遍历
    bool recursivelyCheck(ListNode* head){      // 反向遍历
        if(head){
            if(!recursivelyCheck(head->next)) return false;     // 递归
            if(pointer->val!=head->val) return false;           // 比较
            pointer=pointer->next;               
        }
        return true;
    }
public:
    bool isPalindrome(ListNode* head){
        pointer=head;               // 成员变量
        return recursivelyCheck(head);
    }
};