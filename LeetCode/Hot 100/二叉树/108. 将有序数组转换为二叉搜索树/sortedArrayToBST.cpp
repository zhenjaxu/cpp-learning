// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

#include<vector>

TreeNode* sortedArrayToBST(std::vector<int>& nums){
    return traversal(nums, 0, nums.size());
}

// left，right，左闭右开
TreeNode* traversal(std::vector<int>& nums, int left, int right){
    if(left==right) return nullptr;
    
    int middle=(left+right)/2;
    TreeNode* root=new TreeNode(nums[middle]);

    root->left=traversal(nums, left, middle);
    root->right=traversal(nums, middle+1, right);

    return root;
}