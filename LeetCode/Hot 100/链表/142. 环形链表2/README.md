# 142.环形链表
## 理解
返回入口处的节点。
### 理解一，floyd判圈法
前面通过floyd判圈法判断是否有环，但快指针与慢指针并不一定在入口处相遇。
所以前面的方法对此题并不适用。
### 理解二，hash表
前面还用过观察重复节点来判断是否有圈，而重复节点刚好就是入口处节点。
通过unorderedset保存遍历的节点，一旦节点重复，那就可以确定入口了。
### 理解三，快慢指针
再次尝试floyd判圈法，尝试通过O（1）的空间复杂度解决该问题。
要怎么做，才能使慢指针刚进入环内，就能与快指针相遇。
想要实现一定在入口处相遇过于困难，无论是限制慢指针的移动还是多相遇几次都不太行。
那有没有一种方法能让快慢指针即使不在入口处相遇也能判断出入口处在哪。
通过快慢指针自带的计数器，获取当前的步数。
在慢指针进圈之后，与快指针的距离会逐渐缩短，直至相遇。
那此时快指针的步数应当是慢指针的两倍，因为它们是同时出发的。
当慢指针进入环内的时候，快指针走了两倍的直线长度。
通过公式推导后发现，环外的长度与指针的计数并非线性关系，该想法并不可行。

## 尝试写
### 函数声明
```cpp
ListNode* detectCycle(ListNode* head);
```
### 方法一，hash表
尝试理解二，用unorderedset。
```cpp
std::unordered_set<ListNode*> nodeMap;
while(head){
    if(!nodeMap.insert(head).second) return head;
    head=head->next;
} 
return nullptr;
```
时间复杂度O（N），空间复杂度O（N）。

## 题解
### 哈希表
一个非常直观的思路是：我们遍历链表中的每个节点，并将它记录下来；一旦遇到了此前遍历过的节点，就可以判定链表中存在环。借助哈希表可以很方便地实现。
```cpp
unordered_set<ListNode *> visited;
while (head != nullptr) {
    if (visited.count(head)) {
        return head;
    }
    visited.insert(head);
    head = head->next;
}
return nullptr;
```
时间复杂度O（N），空间复杂度O（N）。
### 快慢指针
我们使用两个指针，fast 与 slow。它们起始都位于链表的头部。随后，slow 指针每次向后移动一个位置，而 fast 指针向后移动两个位置。
如果链表中存在环，则 fast 指针最终将再次与 slow 指针在环中相遇。如下图所示，设链表中环外部分的长度为 a。slow 指针进入环后，又走了 b 的距离与 fast 相遇。
此时，fast 指针已经走完了环的 n 圈，因此它走过的总距离为 a+n(b+c)+b=a+(n+1)b+nc。
根据题意，任意时刻，fast 指针走过的距离都为 slow 指针的 2 倍。
因此，我们有a+(n+1)b+nc=2(a+b)⟹a=c+(n−1)(b+c)有了 a=c+(n−1)(b+c) 的等量关系，我们会发现：从相遇点到入环点的距离加上 n−1 圈的环长，恰好等于从链表头部到入环点的距离。
因此，当发现 slow 与 fast 相遇时，我们再额外使用一个指针 ptr。起始，它指向链表头部；随后，它和 slow 每次向后移动一个位置。最终，它们会在入环点相遇。
```cpp
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
```
时间复杂度O（N），空间复杂度O（1）。

## 总结
这个快慢指针太难想了，以为可以通过两个指针的计数可以推出入口为止。
没想到，还用了一个指针，在相遇之后开始出发，最终在入口处相遇。
当时被n迷惑了，没想到还有这一层关系。
这个对我来说还是太困难了。