// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

#include<cmath>

class Solution{
    int maxDiameter;

    int depth(TreeNode* root){
        if(!root) return 0;

        int leftDepth=depth(root->left);
        int rightDepth=depth(root->right);
        
        maxDiameter=std::max(maxDiameter, leftDepth+rightDepth);        // 直径为左右深度之和

        return std::max(leftDepth, rightDepth)+1;       // 当前深度为左右深度最大值加1
    }

public:
    int diameterOfBinaryTree(TreeNode* root){
        maxDiameter=0;
        depth(root);
        return maxDiameter;
    }
};
