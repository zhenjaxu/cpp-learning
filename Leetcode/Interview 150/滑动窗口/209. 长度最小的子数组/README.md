# 209.长度最小的子数组
## 问题描述
给定一个正整数的数组和一个正整数的target，求和大于等于target的连续子数组的最小长度。
## 函数声明
```cpp
int minSubArrayLen(int target, std::vector<int>& nums);
```
## 尝试写
遍历时，记录前缀和的值，并计算中间子数组之和是否大于等于target。满足条件时，更新最小长度。
```cpp
std::vector<int> pre(nums.size()+1, 0);
int sum=0;
int result=INT_MAX;
for(int i=0,j=0;i<nums.size();++i){
    sum+=nums[i];
    while(j<i+1&&sum-pre[j]>=target){
        result=std::min(result, i-j+1);
        j++;
    }
    pre[i+1]=sum;
}
return (result==INT_MAX)?0:result;
```
时间复杂度O(N)，空间复杂度O(N)。
## 题解
前缀和虽适用于连续子数组求和，但本身过于累赘。在窗口左侧后移时可以对sum做减法，而不需要耗费大量内存空间去存储sum。
```cpp
int sum=0;
int result=INT_MAX;
for(int i=0,j=0;i<nums.size();++i){
    sum+=nums[i];
    while(sum>=target){
        result=std::min(result, i-j+1);
        sum-=nums[j];
        j++;
    }
}
return (result==INT_MAX)?0:result;
```
时间复杂度O(N)，空间复杂度O(1)。