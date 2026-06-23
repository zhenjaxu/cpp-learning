#include<stack>
#include<climits>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

bool isValidBST(TreeNode* root){
    long pre=LONG_MIN;
    std::stack<TreeNode*> stk;

    while(!stk.empty()||root){
        while(root){
            stk.push(root);
            root=root->left;
        }
        root=stk.top();
        stk.pop();
        if(root->val<=pre){
            return false;
        }
        root=root->right;
    }
    return true;
}