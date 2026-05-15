# 101.对称二叉树
## 理解
### 理解一，递归
根节点本身就是对称的，通过对比左右两个子树是否对称。
可当子树不对称时，该树也并非不对称。
故而通过判断子树是否对称来判断整颗树对称并不现实。
### 理解二，迭代
两边同时向下搜索对比。

## 尝试写
### 函数声明
```cpp
bool isSymmetric(TreeNode* root);
```
### 方法一，迭代
尝试理解二，逐层对比。使用双端队列，左子树的全部放前面，右子树的全部放后面。
```cpp
if(!root) return true;
std::deque<TreeNode*> nodeQue;
nodeQue.push_front(root->left);
nodeQue.push_back(root->right);
while(!nodeQue.empty()){
    TreeNode* left=nodeQue.front();nodeQue.pop_front();
    TreeNode* right=nodeQue.back();nodeQue.pop_back();
    if(!left&&!right) continue;
    if(!left||!right||left->val!=right->val) return false;
    nodeQue.push_front(left->left);
    nodeQue.push_front(left->right);
    nodeQue.push_back(right->right);
    nodeQue.push_back(right->left);
}
return true;
```
时间复杂度和空间复杂度都是O（N）。

## 题解
### 递归
题解的递归通过辅助函数（必要的），判断左右根节点的值是否相同，再判断是否镜像对称。
```cpp
bool compare(TreeNode* left, TreeNode* right){
if(!left&&!right) return true;
if(!left||!right) return false;
return left->val==right->val&&
        compare(left->left, right->right)&&
        compare(left->right, right->left);
}
```
```cpp
if(!root) return true;
return compare(root->left, root->right);
```
时间复杂度和空间复杂度都是O（N）。
### 迭代
同样借助辅助函数（其实可以不用）实现。
不过官方并没有使用双端队列，而是交叉放入队列中，不失为一种方法。
```cpp
bool check(TreeNode *u, TreeNode *v) {
    queue <TreeNode*> q;
    q.push(u); q.push(v);
    while (!q.empty()) {
        u = q.front(); q.pop();
        v = q.front(); q.pop();
        if (!u && !v) continue;
        if ((!u || !v) || (u->val != v->val)) return false;
        q.push(u->left); 
        q.push(v->right);
        q.push(u->right); 
        q.push(v->left);
    }
    return true;
}
```
```cpp
return check(root, root);
```
时间复杂度和空间复杂度都是O（N）。