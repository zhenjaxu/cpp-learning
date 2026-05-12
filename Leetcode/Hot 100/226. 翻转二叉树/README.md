# 226.翻转二叉树
## 理解
翻转整颗二叉树，返回根节点。
### 理解一，深度优先
首先想到的就是递归。通过递归先将两颗子树替换位置，然后向下递归，最终会全部翻转。
但是需返回根节点。就不得不借助辅助函数执行无返回的翻转，待翻转完成后，返回root即可。
那可以不用辅助函数完成吗？其实是可以的。无需在意子树的返回，返回root。
### 理解二，广度优先
是否可以将队列换成栈。那势必要将所有节点全部塞入栈中，然后一次性弹出，连成新的二叉树，即可实现翻转。
但需对各个根节点进行标记，才可判断弹出的是否为根节点。
可若不用队列，就不知道是否遍历完整颗二叉树。既有队列又有栈，其过程过于复杂。

## 尝试写
### 函数声明
```cpp
TreeNode* invertTree(TreeNode* root);
```
### 方法一，深度优先
通过递归实现翻转。
```cpp
if(!root||(!root->left&&!root->right)) return root;
TreeNode* temp;
temp=invertTree(root->left);
root->left=invertTree(root->right);
root->right=temp;
return root;
```
时间复杂度O（n），空间复杂度（highth）。

## 题解
### 递归
这是一道很经典的二叉树问题。显然，我们从根节点开始，递归地对树进行遍历，并从叶子节点先开始翻转。
如果当前遍历到的节点 root 的左右两棵子树都已经翻转，那么我们只需要交换两棵子树的位置，即可完成以 root 为根节点的整棵子树的翻转。
```cpp
if (root == nullptr) {
    return nullptr;
}
TreeNode* left = invertTree(root->left);
TreeNode* right = invertTree(root->right);
root->left = right;
root->right = left;
return root;
```
时间复杂度度和空间复杂度均为O（N）。
### 广搜
原来用队列同样可以实现。我想的是通过栈就能实现自动翻转，结果越来越复杂，是我有点天马行空了。
在子节点放入队列前后，都可以进行旋转。
```cpp
if(!root) return root;
std::queue<TreeNode*> nodeQueue;
nodeQueue.push(root);
while(!nodeQueue.empty()){
    TreeNode* front=nodeQueue.front();
    std::swap(front->left, front->right);
    if(front->left) nodeQueue.push(front->left);
    if(front->right) nodeQueue.push(front->right);
    nodeQueue.pop();
} 
return root;
```
时间复杂度O（N），空间复杂度O（N）。

## 总结
对于二叉树的大部分问题都可通过深度（递归）或者广度（循环+队列）解决。