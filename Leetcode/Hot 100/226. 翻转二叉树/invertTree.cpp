// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

#include <queue>

TreeNode* invertTree(TreeNode* root){
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
}