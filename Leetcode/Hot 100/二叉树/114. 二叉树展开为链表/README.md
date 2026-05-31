# 二叉树展开为链表
## 理解
将二叉树以先序遍历的形式展开为链表。
通过递归分别将左右子树展开，并将左子树插入到右子树。

## 函数声明
```cpp
void flatten(TreeNode* root);
```

## 尝试写
```cpp
if(!root) return;
flatten(root->left);        // 将左子树压平
flatten(root->right);       // 将右子树压平
// 找到左子树的末尾
TreeNode* cur=root->left;
if(cur){
    while(cur->right){
        cur=cur->right;
    }
    // 插入右子树
    TreeNode* temp=root->right;
    root->right=root->left;
    cur->right=temp;
    root->left=nullptr;     // 左子树置空
}
```
时间复杂度O(n2)，空间复杂度O(n)。

## 题解
### 先序遍历
通过先序遍历将遍历结果存入数组中，然后遍历数组更改左右子树。
```cpp
void preorderTraversal(TreeNode* root, std::vector<TreeNode*>& l){
    if(root){
        l.push_back(root);
        preorderTraversal(root->left, l);
        preorderTreversal(root->right, l);
    }
}
```
```cpp
std::vector<TreeNode*> l;
preorderTraversal(root, l);
int n=l.size();
for(int i=1;i<n;++i){
    TreeNode* prev=l.at(i-1), *curr=l.at(i);
    prev->left=nullptr;
    prev->right=curr;
}
```
时间复杂度O(n)，空间复杂度O(2n)。
除了递归可以先序遍历，迭代同样可以（借助栈）。
```cpp
std::vector<TreeNode*> v={};
std::stack<TreeNode*> stk={};
TreeNode* node=root;
while(node||!stk.empty()){
    while(node!=nullptr){
        v.push_back(node);
        stk.push(node);
        node=node->left;
    }
    node=stk.top();stk.pop();
    node=node->right;
}

int size=v.size();
for(int=1;i<size;++i){
    auto prev=v.at(i-1), curr=v.at(i);
    prev->left=nullptr;
    prev->right=curr;
}
```
时间复杂度O(2n)，空间复杂度O(2n)。
将迭代的两个步骤合而为一，遍历和展开同时进行。利用prev指针保存前面一个节点，以便与curr连接。
```cpp
if(!root) return;
        
auto stk=std::stack<TreeNode*>();
stk.push(root);
TreeNode* prev=nullptr;
while(!stk.empty()){
    TreeNode* curr=stk.top();stk.pop();

    if(prev){
        prev->left=nullptr;
        prev->right=curr;
    }

    TreeNode* left=curr->left;
    TreeNode* right=curr->right;
    if(right){
        stk.push(right);
    }
    if(left){
        stk.push(left);
    }
    
    prev=curr;
}
```
时间复杂度O(n)，空间复杂度O(n)。