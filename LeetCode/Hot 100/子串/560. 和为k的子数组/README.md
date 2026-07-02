# 560.和为k的子数组
## 问题描述
统计和为k的连续非空子数组的个数。

## 函数声明
```cpp
int subarraySum(std::vector<int>& nums, int k);
```

## 尝试写
通过两层循环遍历子数组，将和为k的统计出来。其中当right指向0时，其上一次统计的子数组都可以满足要求。且如果right指向的值本身为k，则还需加上自身，然后更新两个计数器。
```cpp
int left=0, right;
int count=0;
int preCount=0;
for(right=0;right<nums.size();++right){
    if(nums[right]==0){
        count+=preCount;
        if(nums[right]==k){
            count++;
            preCount++;
        }
        continue;
    }

    int sum=0;
    preCount=0;
    for(left=right;left>=0;--left){
        sum+=nums[left];
        if(sum==k) preCount++;
    }
    count+=preCount;
}

return count;
```
时间复杂度O(N2)，空间复杂度O(1)。

## 题解
题解使用动态规划和哈希表巧妙地避免了重复的求和过程。通过判断当前子串和与之前记录的子串和之差是否为k，来锁定和k的子数组（这两个子串不重叠部分）。
```cpp
std::unordered_map<int, int> map;
map[0]=1;
int count=0, pre=0;
for(auto& x:nums){
    pre+=x;
    if(map.count(pre-k)){
        count+=map[pre-k];
    }
    map[pre]++;
}
return count;
```
时间复杂度O(N)，空间复杂度O(N)。