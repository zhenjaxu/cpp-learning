# 160.相交链表
## 理解
返回两链表相交的起始节点，没有返回null。
### 哈希
insert添加遍历过的节点，一旦失败就返回当前节点。

## 尝试写
### 函数声明
```cpp
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB);
```
### 哈希
```cpp
std::unordered_set<ListNode*> nodeSet;
while(headA||headB){
    if(headA){
        if(!nodeSet.insert(headA).second) return headA;
        headA=headA->next;
    }
    if(headB){
        if(!nodeSet.insert(headB).second) return headB;
        headB=headB->next;
    }
}
return nullptr;
```
时间复杂度O（n），空间复杂度O（2n）。

## 题解
### 双指针
两个指针同时出发，遍历完后继续遍历另个链表。如果相交且不相交部分一样长，则在遍历过程中在交点相遇；
如果相交且不相交部分不一样长，则在第二次遍历中在交点相遇；
如果不相交且长度相同，则第一次同时遍历到末尾。
如果不相交且不一样长，则第二次同时遍历到末尾。
```cpp
if(!headA&&!headB) return nullptr;
ListNode* pA=headA;
ListNode* pB=headB;
while(pA!=pB){
    pA=pA?pA->next:headB;
    pB=pB?pB->next:headA;
}
return pA;      // 如果不相交，pA==nullptr
```
时间复杂度O（2n），空间复杂度O（1）。