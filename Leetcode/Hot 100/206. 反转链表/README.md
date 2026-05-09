# 206.反转链表
## 理解
看来下题目，感觉可以在末尾加个尾巴

## 尝试写
### 方法一
先用一个指针遍历到末尾，指向尾部
这样的话这次遍历就没有起到什么特别的效果
然后遍历，不断地向尾部添加
所以还是两个指针，一前一后，不断向中间靠拢
```cpp
if(!head) return head;
ListNode* back=head;
while(back)
{
    back=back->next;
}
while(head->next)
{
    ListNode* temp=head;
    head=head->next;
    temp->next=back;
    back=temp;
}
// 遍历到末尾后，与反转的链表直接相连
head->next=back;
return head;
```
时间复杂度O（2*n）,两次遍历
空间复杂度O（1）
仔细想来，back真的需要吗，back用来从尾部添加
但实际运作下来只是将链表一分为二
既如此，何不从头开始一分为二
```cpp
ListNode* back=nullptr;
ListNode* temp;
while(head)
{
    temp=head;
    head=head->next;
    temp->next=back;
    back=temp;
}
return back;
```
这么下来，一次遍历足够
时间复杂度O（N）
空间复杂度O（1）
### 方法二
再看看递归
两种情况，咱们分析一下
一种是去掉头部，反转后面的
这样的话，去掉的头部需要连接到递归返回的链表最后面
相当于递归一次，就要循环一次
另一种，去掉尾部，反转前面的
这样的话，去掉的尾部需要循环一次才行
也是递归一次，循环一次
```cpp
if(!head||!head->next) return head;
ListNode* temp=head->next;
ListNode* rhead=reverseList(head->next);
temp->next=head;
return rhead;
```
写出来和最开始想的不太一样，刚开始是像找到尾部，取出来然后反转前面的
但是发现要把尾部给断开才行，但是back已经到了尾部，反不回去了
这么想着就很复杂，果断放弃
尝试取头部，反转后面的
结果发现一个很巧妙的事，就根本不需要遍历到尾部，因为只要开始在head->next挂个指针，它就会自动到尾部
果断用一个temp指向next，反转后，直接在temp后面连上取出的头节点，然后返回反转后得到的头节点
结果报错了，怎么回事呢
有一个致命的问题，就是head的next还是连着temp，应当置空的，否则全部搅在一起了
那这么说，temp不就没什么用了，故而稍微修改一下
```cpp
if(!head||!head->next) return head;
ListNode* rhead=reverseList(head->next);
head->next->next=head;
head->next=nullptr;
return rhead;
```
时间复杂度O（N），同样要遍历所有节点
空间复杂度O（N），递归深度就是节点数，每层递归就是些指针

## 题解
### 迭代
这个方法和修改的方法一是一样的
两个指针一前一后，将链表一分为二
```cpp
ListNode* prev = nullptr;
ListNode* curr = head;
while (curr) {
    ListNode* next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
}
return prev;
```
时间复杂度O（N），空间复杂度O（1）
### 递归
和方法二是一样，很神奇的解法
```cpp
if (!head || !head->next) {
    return head;
}
ListNode* newHead = reverseList(head->next);
head->next->next = head;
head->next = nullptr;
return newHead;
```
时间复杂度和空间复杂度都是O（N）

## 总结
这道题做得还不错，挺开心的
不要怕犯错，敢于去写，去思考，你就会成功
加油各位，还有努力的自己