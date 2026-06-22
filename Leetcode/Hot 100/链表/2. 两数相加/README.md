# 2.两数之和
## 问题描述
两条链表分别从个位开始向后延伸，完成两条链表的相加。

## 函数声明
```cpp
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2);
```
## 尝试写
通过递归的方法完成链表相加，再通过一个标志位表示进位。最后当两条链表都遍历完后，根据flag选择是否返回最后一个节点。
```cpp
int flag=0;
```
```cpp
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
```
时间复杂度O(max(M,N))，空间复杂度O(max(M,N))。题解通过迭代实现，方法一样但空间复杂度O(1)。