# 543.二叉树的直径
## 理解
任意两节点之间的最长路径长度（边数）。
### 递归
通过递归返回左右两子树深度，并在过程中更新最长路径。

## 尝试写
### 函数声明
int diameterOfBinaryTree(TreeNode* root);
### 递归
适用辅助函数遍历整棵树，并在遍历过程中计算直径。
```cpp
int maxDiameter;
int depth(TreeNode* root){
    if(!root) return 0;

    int leftDepth=depth(root->left);
    int rightDepth=depth(root->right);
    
    maxDiameter=std::max(maxDiameter, leftDepth+rightDepth);

    return std::max(leftDepth, rightDepth)+1;
}
```
```cpp
maxDiameter=0;
depth(root);
return maxDiameter;
```
时间复杂度O（n），空间复杂度O（height）。