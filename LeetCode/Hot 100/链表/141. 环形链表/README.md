# 141.环形链表
## 理解
判断是否有环，图上特别直观，但对于遍历而言，相当于摸黑
### 理解一
如果我在环的入口处做上记号，那是不是就能解决这个问题
有一种记号就是，在环入口的前面一个节点放置哨兵，一旦我的指针再次遍历到环入口的时候，哨兵的next是否和指针指向同一地方
但这个方法的前提是我得确定哨兵的位置，那只能进行一次一次的迭代，但问题随之而来
怎么进入下层循环，遍历完链表（不可能的），所以这个方法不可行，那还有什么方法呢
### 理解二
该题的环有一个特点，都在链表的末尾，next从尾部指向链表中间节点
那中间节点是同时被两个next指向的，但对于节点自身是不自知的
尾节点的next和入口前面一个节点的next虽指向同一节点，但其指针地址是必然不一样的
如用此进行比较，就可以判断是否出现环了，那怎么取到之前的next呢
当然是刚刚学过的unorderedmap，索引为next->val，值就是next
但这样的话就有边界问题，如果next为空指针呢，所以在存入之前必须判断next是否为空
什么情况下next会为空，当然是最后一个节点，并且没有环
### 理解三
对于环而言，重复是最大的特征
但值得重复不代表就一定是环，节点的重复才是
那只要节点重复出现一次，就可以立马判定有环
那用什么来记录呢，unorderedset足以

## 尝试写
### 函数声明
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
bool hasCycle(ListNode *head);
```
### 方法一
尝试一下理解二
```cpp
std::unordered_map<int, ListNode**> nextMap;
int nextValue;
while(head&&head->next)
{
    nextValue=head->next->val;
    auto it=nextMap.find(nextValue);
    if(it!=nextMap.end()&&it->second!=&head->next) return true;
    nextMap[nextValue]=&head->next;
    head=head->next;
}
return false;
```
操作的逻辑上不存在太大问题，但有一个不适用的情况
整个都是环，头尾相连，那就无法进行比较了
所以在前面加一个dummyhead
```cpp
std::unordered_map<int, ListNode**> nextMap;
int nextValue;
ListNode dummyHead;
dummyHead.next=head;
head=&dummyHead;
while(head&&head->next)
{
    nextValue=head->next->val;
    auto it=nextMap.find(nextValue);
    if(it!=nextMap.end()&&it->second!=&head->next) return true;
    nextMap[nextValue]=&head->next;
    head=head->next;
}
return false;
```
但又发现一个问题，将值作为索引
链表中重复出现同一值就会导致判断结果为真
也就是说map里的值会被覆盖掉
若把int换成节点，其内存开销可想而知
若把int换成节点指针，这开销就会小很多
```cpp
std::unordered_map<ListNode*, ListNode**> nextMap;
ListNode* nextNode;
ListNode dummyHead;
dummyHead.next=head;
head=&dummyHead;
while(head&&head->next)
{
    nextNode=head->next;
    auto it=nextMap.find(nextNode);
    if(it!=nextMap.end()&&it->second!=&head->next) return true;
    nextMap[nextNode]=&head->next;
    head=head->next;
}
return false;
```
时间复杂度O（N），在遍历完后或碰到重复节点的第一时间立马返回，只用遍历一次
空间复杂度O（N），内存开销主要由unorderedmap产生
### 方法二
尝试理解三
```cpp
std::unordered_set<ListNode*> nodeMap;
while(head)
{
    // insert返回一个迭代器和一个布尔值
    // 第一次添加，布尔值为true
    // 而重复添加，布尔值为false
    if(!nodeMap.insert(head).second) return true;
    head=head->next;
}
return false;
```
时间复杂度O（N），空间复杂度O（N）
        
## 题解
### 哈希表
最容易想到的方法是遍历所有节点，每次遍历到一个节点时，判断该节点此前是否被访问过
具体地，我们可以使用哈希表来存储所有已经访问过的节点
每次我们到达一个节点，如果该节点已经存在于哈希表中，则说明该链表是环形链表，否则就将该节点加入哈希表中
重复这一过程，直到我们遍历完整个链表即可
```cpp
unordered_set<ListNode*> seen;
while (head != nullptr) {
    if (seen.count(head)) {
        return true;
    }
    seen.insert(head);
    head = head->next;
}
return false;
```
时间复杂度O（N），空间复杂度O（N）
### 快慢指针
本方法需要读者对「Floyd 判圈算法」（又称龟兔赛跑算法）有所了解
假想「乌龟」和「兔子」在链表上移动，「兔子」跑得快，「乌龟」跑得慢
当「乌龟」和「兔子」从链表上的同一个节点开始移动时，如果该链表中没有环，那么「兔子」将一直处于「乌龟」的前方
如果该链表中有环，那么「兔子」会先于「乌龟」进入环，并且一直在环内移动
等到「乌龟」进入环时，由于「兔子」的速度快，它一定会在某个时刻与乌龟相遇，即套了「乌龟」若干圈
我们可以根据上述思路来解决本题。具体地，我们定义两个指针，一快一慢
慢指针每次只移动一步，而快指针每次移动两步
初始时，慢指针在位置 head，而快指针在位置 head.next
这样一来，如果在移动的过程中，快指针反过来追上慢指针，就说明该链表为环形链表
否则快指针将到达链表尾部，该链表不为环形链表
```cpp
if(!head||!head->next) return false;
ListNode* fast=head->next;
ListNode* slow=head;
while(fast->next&&fast->next->next)
{
    if(fast==slow) return true;
    fast=fast->next->next;
    slow=slow->next;
}
return false;
```
时间复杂度O（N），无环时快指针移动到尾部，时间缩短一半，有环时慢指针至多移动N次就会与快指针重合
空间复杂度O（1）
        
## 总结
Floyed判圈法，很神奇的方法，只要有环，快慢指针必会相见
unorderedset和unorderedmap的底层实现都是哈希表
set的insert除了添加操作外，还会返回相应的迭代器，以及是个布尔值，表示是否成功添加（重复则失败）