# 102.二叉树的层序遍历
## 理解
逐层返回从左到右的所有节点。
### 理解一，广搜
如何将层序遍历的结果按照要求全部输出。
跟最大深度的思路是一样的。由于普通的广搜每层的界限是模糊的。
在刚进入循环的时候，获取当前层的大小。

## 尝试写
### 函数声明
```cpp
vector<vector<int>> levelOrder(TreeNode* root);
```
### 方法一，广搜
```cpp
if(!root) return {};
std::queue<TreeNode*> nodeQueue;
std::vector<std::vector<int>> result;
nodeQueue.push(root);
while(!nodeQueue.empty()){
    size_t size=nodeQueue.size();
    std::vector<int> layer={};
    while(size--){
        TreeNode* front=nodeQueue.front();
        layer.push_back(front->val);
        nodeQueue.pop();
        if(front->left) nodeQueue.push(front->left);
        if(front->right) nodeQueue.push(front->right);
    }
    result.push_back(layer);
}
return result;
```
时间复杂度O（N），空间复杂度O（N），主要由两个数组一个队列开销。

## 题解
### 广搜
与方法一思路一样。
```cpp
vector <vector <int>> ret;
if (!root) {
    return ret;
}
queue <TreeNode*> q;
q.push(root);
while (!q.empty()) {
    int currentLevelSize = q.size();
    ret.push_back(vector <int> ());
    for (int i = 1; i <= currentLevelSize; ++i) {
        auto node = q.front(); q.pop();
        ret.back().push_back(node->val);
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}
return ret;
```
时间复杂度和空间复杂度都是O（N）。