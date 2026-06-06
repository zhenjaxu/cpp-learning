# 24.两两交换链表中的节点
## 理解
链表节点两两交换，返回头结点。

## 函数声明
```cpp
ListNode* swapPairs(ListNode* head);
```

## 尝试写
采用递归的方式，交换当前一对节点，将递归返回交换后的头结点拼接其后。
```cpp
if(!head) return nullptr;
if(!head->next) return head;

ListNode* temp=head->next;
head->next=swapPairs(head->next->next);
temp->next=head;

return temp;
```
时间复杂度O(n/2)，空间复杂度O(n/2)。

## 题解
一对节点的交换需要涉及四个节点。由于next的存在，实际需要三个指针即可实现。开头一对节点通过加入一个虚头节点解决，最后返回虚头节点的next指针。注意堆区的资源释放。
```cpp
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
```
时间复杂度O(n/2)，空间复杂度O(1)。