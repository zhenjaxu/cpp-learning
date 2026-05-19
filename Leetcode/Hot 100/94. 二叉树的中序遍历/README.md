# 94.二叉树的中序遍历
## 理解
返回中序遍历结果。
### 递归
将子树返回的结果拼接在根的两边。
### 回溯
中序遍历整个二叉树，并将值记录到数组中。

## 尝试写
### 函数声明
std::vector<int> inorderTraversal(TreeNode* root);
### 递归
通过insert拼接数组。
```cpp
if(!root) return {};
vector<int> left=inorderTraversal(root->left);          // 左
left.push_back(root->val);                            // 中
vector<int> right=inorderTraversal(root->right);        // 右
left.insert(left.end(), right.begin(), right.end());
return left;
```
时间复杂度O（n），空间复杂度O（n）
### 回溯
递归搜索整棵树，回溯可以从子节点回到根节点。
```cpp
std::vector<int> result;
```
```cpp
void recursivelyTraversal(TreeNode* root){
    if(!root) return;
    recursivelyTraversal(root->left);
    result.push_back(root->val);
    recursivelyTraversal(root->right);
}
```
```cpp
result={};
recursivelyTraversal(root);
return result;
```
时间复杂度O（n），空间复杂度O（n）

## 题解
### 迭代
root必须始终是未遍历的节点。一旦root变成了遍历过的节点，就会出现反复添加的滑稽操作。
没遍历过的root放入栈中，弹出时将值添加进result中。
```cpp
std::stack<TreeNode*> nodeStack;
std::vector<int> result;
while(root||!nodeStack.empty()){
    while(root){
        nodeStack.push(root);
        root=root->left;
    }
    root=nodeStack.top();nodeStack.pop();
    result.push_back(root->val);
    root=root->right;           // root指向未遍历的右子树
}
return result;
```
时间复杂度O（n），空间复杂度O（n）
### Morris中序遍历
通过与左子树中序遍历的最后一个节点建立连接，即最后一个节点的right指向root，达到回溯的效果。
第一次到达root时，predecessor与root建立连接，而第二次到达root是通过predecessor建立的连接回来的。
此时，predecessor的right是直接指向root的，说明root左子树的最后一个节点已经遍历了，断开桥梁，复原左子树。
最后遍历到最右边，root为空，循环结束。
```cpp
std::vector<int> result;
TreeNode* predecessor;
while(root){
    if(root->left){
        predecessor=root->left;
        while(predecessor->right&&predecessor->right!=root){
            predecessor=predecessor->right;         // 左子树最右边节点
        }
        if(!predecessor->right){            // 第一次建立连接
            predecessor->right=root;
            root=root->left;
        }else{                              // 第二次断开连接
            predecessor->right=nullptr;
            result.push_back(root->val);
            root=root->right;               // 继续遍历右子树
        }
    }else{
        result.push_back(root->val);
        root=root->right;
    }
}
return result;
```
时间复杂度O（2n），空间复杂度O（1）。