# 53.最大子数组和
## 问题描述
返回其连续非空子数组的最大和。

## 函数声明
```cpp
int maxSubArray(std::vector<int>& nums);
```

## 尝试写
通过当前和与前缀和之差，得到中间不重合子数组的和。保证前缀和始终保持最小，才能尽可能保证目标子数组和更大。
```cpp
int sum=0;
int max=INT_MIN;
int pre=0;
for(const auto& x:nums){
    sum+=x;
    max=std::max(max, sum-pre);
    if(sum<pre) pre=sum;
}
return max;
```
时间复杂度O(N)，空间复杂度O(1)。

## 题解
### 动态规划
题解通过保证之前子数组的和始终大于0，这样其子数组和就能不断增加。
```cpp
int pre=0, maxAns=nums[0];
for(const auto& x:nums){
    pre=std::max(pre+x, x);
    maxAns=std::max(maxAns, pre);
}
return maxAns;
```
时间复杂度O(N)，空间复杂度O(1)。
### 分治
通过递归方式，获取到最小子段状态，即四个sum均为该数。随后回升过程中，通过左右子段的四种sum，分别合并与更新sum的值。
```cpp
struct Status{
    int lSum, rSum, mSum, iSum;
};

Status pushUp(Status l, Status r){
    int iSum=l.iSum+r.iSum;
    int lSum=std::max(l.lSum, l.iSum+r.lSum);
    int rSum=std::max(r.rSum, r.iSum+l.rSum);
    int mSum=std::max(std::max(l.mSum, r.mSum), l.rSum+r.lSum);
    return (Status){lSum, rSum, mSum, iSum};
}

Status get(std::vector<int>& a, int l, int r){
    if(l==r){
        return (Status){a[l], a[l], a[l], a[l]};
    }

    int m=(l+r)>>1;
    Status lSub=get(a, l, m);
    Status rSub=get(a, m+1, r);
    return pushUp(lSub, rSub);
}
```
```cpp
return get(nums, 0, nums.size()-1).mSum;
```
时间复杂度O(logN)，空间复杂度O(logN)。