# 98.验证二叉搜索树
## 问题描述
验证该树是否为二叉搜索树，即保证左子树全部都是小于当前节点，右子树则全部大于，且子树也为二叉搜索树。
## 函数声明
```cpp
bool isValidBST(TreeNode* root);
```
## 尝试写
以左中右中序遍历二叉树。通过全局变量pre记录前一个节点值，与当前值进行比较，pre值始终小于当前节点。
```cpp
long pre=LONG_MIN;
```
```cpp
if(!root) return true;
if(isValidBST(root->left)){
    if(root->val>pre){
        pre=root->val;
        return isValidBST(root->right);
    }
}
return false;
```
时间复杂度O(N)，空间复杂度O(N)。
## 题解
以同样思路通过栈实现迭代写法。
```cpp
long pre=LONG_MIN;
std::stack<TreeNode*> stk;

while(!stk.empty()||root){
    while(root){
        stk.push(root);
        root=root->left;
    }
    root=stk.top();
    stk.pop();
    if(root->val<=pre){
        return false;
    }
    root=root->right;
}
return true;
```
时间复杂度O(N)，空间复杂度O(N)。