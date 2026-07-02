# 21.合并两个有序链表

## 尝试写

### 方法一

欲使两个升序链表合并成一个新的链表，两种方法
一种定义result链表，两个指针分别从两链表头节点开始比较
谁小谁先进，相等一起进，进完一起向后移，全部移动到末尾就结束

```cpp
// 添加并移动指针
void add_listNode_to_result(ListNode* & list_p, ListNode* & current)
{
    ListNode* temp=new ListNode(list_p->val);
    current->next=temp;
    list_p=list_p->next;
    current=current->next;
}
```

```cpp
// result链表
ListNode* result_head=new ListNode();
// 一个指针遍历result
ListNode* current=result_head;
// 循环
while(list1!=nullptr||list2!=nullptr)
{
    // 如果有一个链表已经遍历完了，就不需要比较
    if(list1==nullptr)
    {
        add_listNode_to_result(list2, current);
    }
    else if(list2==nullptr)
    {
        add_listNode_to_result(list1, current);
    }
    // 比较
    else
    {
        if(list1->val<=list2->val)
        {
            add_listNode_to_result(list1, current);
        }
        else
        {
            add_listNode_to_result(list2, current);
        }
    }
}
return result_head->next;
```

### 方法二

一种在原来的基础上，通过比较，改变指针
固定返回list1，需要一个指针listhead的next指向list1
从listhead开始遍历，比较list1和list2，小的就加到后面

```cpp
// listhead
ListNode list_head(0, list1);
// current
ListNode* current=&list_head;
// 开始循环
while(list1||list2)
{
    if(!list2)
    {
        // list1本身就连接好了，直接返回
        return list_head->next;
    }
    else if(!list1)
    {
        // list2本身升序，一整个连在后面即可
        current->next=list2;
        return list_head->next;
    }
    else
    {
        if(list1->val<=list2->val)
        {
            current=current->next;
            list1=list1->next;
        }
        else
        {
            current->next=list2;
            list2=list2->next;
            current->next->next=list1;
            current=current->next;
        }
    }
}
return list_head.next;
```

## 题解

### 递归

上面两个呢就是自己写的方法，在看完题解之后
对于有循环的可以尝试下递归
递归的思路跟方法2很像，用到现有的资源，尽可能少地改动

```cpp
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
```

时间复杂度O（n），n为两个链表长度之和，递归相当于在节点，每个节点至多一次，至多遍历n个节点
空间复杂度O（n），n为两个链表长度之和，函数的调用需要消耗栈的空间，栈空间的大小取决于递归的深度，同样最多递归n次，遍历所有节点

### 迭代

再来看看题解中的迭代
迭代呢用的是双指针，双指针也是代替循环的一种方法
这个迭代和我自己写的方法二也很相似
不过比我写的逻辑更清晰，更合理

```cpp
// 首先是一个头节点
ListNode prehead(-1);
// 需要一个指针从头节点开始遍历，让next不断向后连接
ListNode* prev=&prehead;
// 开始循环
while(list1&&list2)
{
    // 比较
    if(list1->val<=list2->val)
    {
        prev->next=list1;
        list1=list1->next;
        // prev=prev->next;
    }
    else
    {
        prev->next=list2;
        list2=list2->next;
        // prev=prev->next;
    }
    // if-else中重复的部分可以尝试放在外面
    prev=prev->next;
}
// 循环结束说明有一个链表已经遍历完了
// 无需比较，整个连接到prev后面即可
// if(!list1)
// {
//     prev->next=list2;
// }
// else
// {
//     prev->next=list1;
// }
// if-else的简单赋值可以尝试三元表达式
prev->next=!list1?list2:list1;
return prehead.next;
```

时间复杂度O（n），n为合并后链表的长度，list1和list2都是指针向后遍历，至多遍历完所有节点
空间复杂度O（1），变量一直是这三个指针和一个头节点，只需要常数的空间存放这些变量

## 总结

合并两个链表，关键在于链表
既然这些节点都已经是现成的了，那咱们完全可以尝试用极小的成本（改动指针）解决这个问题，前提是这两个链表我可以改动
这样的话就可以尽可能缩短运行时间，因为内存的分配也是相当耗时的
合理的遍历每个节点，降低时间复杂度
递归确实巧妙，写出来的代码也很简单，牺牲的是空间复杂度
