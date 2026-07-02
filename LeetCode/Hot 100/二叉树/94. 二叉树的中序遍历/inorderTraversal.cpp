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

std::vector<int> inorderTraversal(TreeNode* root){
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
}