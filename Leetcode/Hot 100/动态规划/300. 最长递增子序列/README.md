# 300.最长递增子序列
## 理解
找到最长递增子序列，返回其长度。
通过pair，stack等向后传递子序列信息，都无法解决遍历过程中的复杂和不确定。

## 函数声明
```cpp
int lengthOfLIS(std::vector<int>& nums);
```

## 题解
### 动态规划
通过数组记录前面所有元素的结果，然后逐个扫描取最大值，并更新结果。
```cpp
if(nums.empty()) return 0;
std::vector<int> dp(nums.size(), 1);
int maxAns=1;
for(int i=0;i<nums.size();++i){
    for(int j=0;j<i;++j){
        if(nums[j]<nums[i]){
            dp[i]=std::max(dp[i], dp[j]+1);
        }
    }
    maxAns=std::max(maxAns, dp[i]);
}
return maxAns;
```
时间复杂度O(n2)，空间复杂度O(n)。

### 贪心
维护一个递增序列，不断更新和插入，哪怕序列可能并不符合实际，但由于是替换的，则说明前面有这某种相同长度的序列满足条件。
尽可能的减缓递增速度，给后续更有可能更长的子序列留足空间，同时有没改变当前最长值。
通过二分查找递增序列，时间复杂度更低。
```cpp
int len=1, n=nums.size();
if(n==0) return 0;

std::vector<int> d(n+1);
d[len]=nums[0];
for(int i=1;i<n;++i){
    if(nums[i]>d[len]){
        d[++len]=nums[i];
    }else{
        int l=1, r=len+1, pos=0;
        while(l<r){
            int mid=(l+r)/2;
            if(d[mid]<nums[i]){
                pos=mid;
                l=mid+1;
            }else{
                r=mid;
            }
        }
        d[pos+1]=nums[i];
    }
}
return len;
```
时间复杂度O(nlogn)，空间复杂度O(n)。