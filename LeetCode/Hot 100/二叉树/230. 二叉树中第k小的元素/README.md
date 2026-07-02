# 230.二叉树中第k小的元素
## 理解
返回二叉搜索树中第k小的元素，k从1开始。

## 函数声明
```cpp
int kthSmallest(TreeNode* root, int k);
```

## 尝试写
使用栈对二叉树进行前序遍历，同时k--，当k等于1时，当前指针指向的即为目标节点。
```cpp
std::stack<TreeNode*> stk;
int result=-1;

TreeNode* curr=root;
while(curr||!stk.empty()){
    while(curr){
        stk.push(curr);
        curr=curr->left;
    }

    curr=stk.top();stk.pop();
    if(k--==1){
        result=curr->val;
        break;
    } 

    curr=curr->right;
}
return result;
```
时间复杂度O(h+k)，空间复杂度O(h)。

## 题解
### 记录节点数
通过左子树节点数与k比较，如果左子树节点总数小于k-1，说明目标节点不在左子树；而等于k-1时，说明根节点即为目标节点；大于k-1，同理。继续搜索左子树时，k无需改变；而搜索右子树时，目标节点实际在右子树的k-left-1处。
```cpp
class MyBst{
public:
    MyBst(TreeNode* root){
        this->root=root;
        countNodeNum(root);
    }

    int kthSmallest(int k){
        TreeNode* node=root;
        while(node!=nullptr){
            int left=getNodeNum(node->left);
            if(left<k-1){
                node=node->right;
                k-=left+1;
            }else if(left==k-1){
                break;
            }else{
                node=node->left;
            }
        }
        return node->val;
    }

private:
    TreeNode* root;
    std::unordered_map<TreeNode*, int> nodeNum;

    int countNodeNum(TreeNode* node){
        if(node==nullptr){
            return 0;
        }
        nodeNum[node]=1+countNodeNum(node->left)+countNodeNum(node->right);
        return nodeNum[node];
    }

    int getNodeNum(TreeNode* node){
        if(node!=nullptr&&nodeNum.count(node)){
            return nodeNum[node];
        }else{
            return 0;
        }
    }
};
```
时间复杂度O(n+h)，空间复杂度O(n)。