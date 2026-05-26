# 78.子集
## 理解
返回所有可能得子集。
遍历一遍数组，将当前值与结果集里的一一组合后得到新的子集，记录后还原。

## 函数声明
```cpp
std::vector<std::vector<int>> subsets(std::vector<int>& nums);
```

## 尝试写
只需遍历完整个数组。
```cpp
std::vector<std::vector<int>> res;
res.push_back({});
for(int i=0;i<nums.size();++i){
    int resSize=res.size();
    for(int j=0;j<resSize;++j){
        res[j].push_back(nums[i]);
        res.push_back(res[j]);

        res[j].pop_back();
    }
}
return res;
```
时间复杂度O(nx2^n)，空间复杂度O(1)。

## 题解
### 二进制枚举
通过二进制特性，按位枚举。通过与（1<<i）按位与，获取需要添加的元素。
```cpp
std::vector<int> t;
std::vector<std::vector<int>> ans;
int n=nums.size();
for(int mask=0;mask<(1<<n);++mask){     // 1<<n=2^n
    t.clear();
    for(int i=0;i<n;++i){
        if(mask&(1<<i)){    // 如001, 010, 100等
            t.push_back(nums[i]);
        }
    }
    ans.push_back(t);
}
return ans;
```
时间复杂度O(nx2^n)，空间复杂度O(n)。
### 递归
每个位置有取和不取两种，递归到最后时记录答案。
```cpp
std::vector<int> t;
std::vector<std::vector<int>> ans;
void dfs(int cur, std::vector<int>& nums){
    if(cur==nums.size()){
        ans.push_back(t);
        return;
    }
    t.push_back(nums[cur]);
    dfs(cur+1, nums);
    t.pop_back();

    dfs(cur+1, nums);
}
```
```cpp
dfs(0, nums);
return ans;
```
时间复杂度O(nx2^n)，空间复杂度O(2n)。