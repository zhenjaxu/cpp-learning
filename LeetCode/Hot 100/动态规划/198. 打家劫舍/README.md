# 198.打家劫舍
## 理解
相邻房间不能同时偷，选择最优盗窃方案。
遍历数组，比较不同组合方式的金额。

## 函数声明
```cpp
int rob(std::vector<int>& nums);
```

## 尝试写
通过两个变量存放前两次最大收益，搜索每个房间并更新收益。
```cpp
if(nums.size()==0) return 0;
if(nums.size()==1) return nums[0];
int curr, prev;
prev=nums[0];
curr=std::max(nums[0], nums[1]);
for(int i=2;i<nums.size();++i){
    int temp=std::max(curr, prev);
    curr=std::max(curr, prev+nums[i]);
    prev=temp;
}
return curr;
```
时间复杂度O(n)，空间复杂度O(1)。与题解一致，算法优良。