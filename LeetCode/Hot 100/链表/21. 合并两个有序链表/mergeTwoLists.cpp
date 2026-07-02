// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution { 
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        if(!list1)
        {
            return list2;
        }
        else if(!list2)
        {
            return list1;
        }
        // 判断为空后直接返回，因为自身就是升序
        // 后续无需再比较，即无需继续递归
        // 需要比较才递归
        else
        {
            if(list1->val<=list2->val)
            {
                // list1这个节点依然确定，故让list1后面的节点和list2的节点进入递归
                list1->next=mergeTwoLists(list1->next, list2);
                // 后面的节点全部合并完毕，返回
                return list1;
            }
            else
            {
                list2->next=mergeTwoLists(list1, list2->next);
                return list2;
            }
        }
    }
};