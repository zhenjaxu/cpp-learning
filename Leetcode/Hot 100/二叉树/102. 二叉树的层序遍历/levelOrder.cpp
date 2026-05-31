// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

#include <vector>
#include <queue>

std::vector<std::vector<int>> levelOrder(TreeNode* root){
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
}