# 234.回文链表
## 理解
判断是否是回文链表，是返回true，不是返回false。
### 栈
遍历过的加入栈中，可并不知道什么时候可以弹出比较。
### 双指针
并非双向链表，不能从后往前遍历。

## 尝试写
### 函数声明
bool isPalindrome(ListNode* head);
### 栈
虽不知道链表的一半在哪，但借鉴交叉链表的交叉遍历。以尾部（nullptr）作为中间节点，再次遍历链表，此时将栈中元素弹出比较。
如果是回文，第二次遍历比较全部相等；如果不是，则不是全部相等。
```cpp
if(!head) return false;
std::stack<int> nodeStack;
int count=2;            // 遍历链表两次
ListNode* current=head;
while(count){
    if(!current){
        current=head;
        count--;
        continue;
    }
    if(count==2) nodeStack.push(current->val);              // 第一轮添加
    else{                                                   // 第二轮比较
        int expected=nodeStack.top();nodeStack.pop();       
        if(current->val!=expected) return false;
    }
    current=current->next;
}
return true;
```
时间复杂度O（2n），空间复杂度O（n）。

## 题解
### 递归
递归有回溯功能，能在链表中反向遍历。需要借助辅助函数，过程十分夸张。
```cpp
ListNode* pointer;                          // 从头遍历
bool recursivelyCheck(ListNode* head){      // 反向遍历
    if(head){
        if(!recursivelyCheck(head->next)) return false;     // 递归
        if(pointer->val!=head->val) return false;           // 比较
        pointer=pointer->next;               
    }
    return true;
}
```
```cpp
pointer=head;               // 成员变量
return recursivelyCheck(head);
```
时间复杂度O（n），空间复杂度O（n）。
### 快慢指针
通过快慢指针找到链表一半的位置，反转后半链表，然后两个指针分别从头部和一半处出发比较，最后恢复链表。
```cpp
ListNode* reverseList(ListNode* head){
    ListNode* slow=nullptr;
    ListNode* fast=head;
    while(fast){
        ListNode* temp=fast->next;
        fast->next=slow;
        slow=fast;
        fast=temp;
    }
    return slow;
}
ListNode* endOfFirstHalf(ListNode* head){
    if(!head) return nullptr;
    ListNode* fast=head;
    ListNode* slow=head;
    while(fast->next&&fast->next->next){
        fast=fast->next->next;
        slow=slow->next;
    }
    return slow;
}
```
```cpp
if(!head) return false;
ListNode* firstHalfEnd=endOfFirstHalf(head);
ListNode* secondHalfStart=reverseList(firstHalfEnd->next);
ListNode* fast=secondHalfStart;
ListNode* slow=head;
bool result=true;
while(fast){
    if(slow->val!=fast->val){
        result=false;
        break;
    }
    fast=fast->next;
    slow=slow->next;
}
reverseList(secondHalfStart);
return result;
```
时间复杂度O（2n），空间复杂度O（1）。