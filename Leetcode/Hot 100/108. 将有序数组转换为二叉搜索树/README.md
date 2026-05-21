# 108.将有序数组转换为二叉搜索树
## 理解
将**升序**数组转换成**平衡**二叉搜索树。
### 递归
升序数组的**中间**元素可以作为整颗树的**根**节点。

## 尝试写
### 函数声明
```cpp
TreeNode* sortedArrayToBST(std::vector<int>& nums);
```
### 递归
取出中间元素作为根节点的值，与剩余两边数组的递归结果返回的根**连接**，形成新的二叉树。
```cpp
TreeNode* traversal(vector<int> nums){
    if(nums.size()==0) return nullptr;

    int middle=nums.size()/2;
    TreeNode* root=new TreeNode(nums[middle]);

    root->left=traversal(std::vector<int>(nums.begin(), nums.begin()+middle));
    root->right=traversal(std::vector<int>(nums.begin()+middle+1, nums.end()));

    return root;
}
```
```cpp
return traversal(nums);
```
时间复杂度O（n），空间复杂度O（logn）。

## 题解
### 深搜
通过传入左右索引值（左闭右开），避免构造带来的不必要的损耗。
```cpp
TreeNode* traversal(std::vector<int>& nums, int left, int right){
    if(left==right) return nullptr;
    
    int middle=(left+right)/2;
    TreeNode* root=new TreeNode(nums[middle]);

    root->left=traversal(nums, left, middle);
    root->right=traversal(nums, middle+1, right);

    return root;
}
```
```cpp
return traversal(nums, 0, nums.size());
```
时间复杂度O（n），空间复杂度O（logn）。