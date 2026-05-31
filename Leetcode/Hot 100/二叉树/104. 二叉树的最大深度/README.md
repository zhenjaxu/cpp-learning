# 104.二叉树的最大深度
## 理解
最大深度是指根节点到最远叶子节点的距离。
### 理解一，中序遍历
二叉树遍历，分层序和中序。可通过中序深入最底层，返回层数，并不断比较取最大值。
### 理解二，层序遍历
层序遍历通过队列依次添加下一层元素，但这样只会找到最后一层的最后一个节点。
因为是一次性遍历完的，每一层之间没有明显的划分，也不好记录层数。感觉并不可行。

## 尝试写
### 函数声明
```cpp
int maxDepth(TreeNode* root);
```
### 方法一，中序遍历
主要通过递归来实现，因为需要知道深度，所以借助辅助函数来实现。
```cpp
int maxDepth_with_depth(TreeNode* root, int depth){
    if(!root) return depth;
    return max(maxDepth_with_depth(root->left, depth+1),
                maxDepth_with_depth(root->right, depth+1));
}
```
主函数里面就返回辅助函数即可。
```cpp
return maxDepth_with_depth(root, 0);
```
时间复杂度O（N），空间复杂度O（N）。

## 题解
### 深度优先搜索
官方提供的答案则不需要辅助函数也能实现。
故重新思考深度参数的必要性。方法一的深度参数只是为了在递归的过程中去计数，在最后返回计数并比较，最终返回计数的最大值。
而题解中最后返回的是0，各分支的末尾开始计数，选出到达根节点的最大距离。相比从根节点开始计数，这样的计数方式则无需深度参数。
```cpp
if (root == nullptr) return 0;
return max(maxDepth(root->left), maxDepth(root->right)) + 1;
```
时间复杂度O（N），空间复杂度O（N）。
### 广度优先搜索
题解不同于正常的广度优先搜索，在每层开始前保存了size，这样就知道该层是否遍历完。
而不是一层while循环到底，可以说确实巧妙。
```cpp
if(!root) return 0;
std::queue<TreeNode*> nodeQueue;
nodeQueue.push(root);
int layer=0;
while(!nodeQueue.empty()){
    int layerSize=nodeQueue.size();
    while(layerSize){
        TreeNode* front=nodeQueue.front();
        if(front->left) nodeQueue.push(front->left);
        if(front->right) nodeQueue.push(front->right);
        nodeQueue.pop();
        --layerSize;
    }
    ++layer;
}
return layer;
```
时间复杂度O（N），空间复杂度O（N）。

## 总结
深度优先和广度优先都是二叉树最常用的搜索方式，深度通过递归或者回溯，广度通过队列和循环。