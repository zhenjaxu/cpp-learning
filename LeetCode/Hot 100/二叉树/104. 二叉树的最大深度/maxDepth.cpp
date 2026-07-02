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

int maxDepth(TreeNode* root) {
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
}