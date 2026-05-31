# 组合总和
## 理解
找出和为target的所有组合，元素可重复选择。
元素可重复选择，则递归可以继续从当前元素深入。

## 函数声明
```cpp
std::vector<std::vector<int>> combinationSum(std::vector<int>& candidates, int target)
```

## 尝试写
递归函数中传入路径，当前元素索引，当前目标。当目标为0时，记录组合。通过当前索引保证后面的元素不会往前遍历，避免组合重复。
```cpp
std::vector<int> path;
std::vector<std::vector<int>> result;
void backtrack(std::vector<int>& candidates, std::vector<int>& path, int current, int target){
    if(target==0){
        result.push_back(path);
        return;
    }

    for(int i=current;i<candidates.size();++i){
        if(candidates[i]>target) continue;
        path.push_back(candidates[i]);
        backtrack(candidates, path, i, target-candidates[i]);
        path.pop_back();
    }
}
```
```cpp
path={};
result={};
backtrack(candidates, path, 0, target);
return result;
```
时间复杂度O(s)，空间复杂度O(target)。

## 题解
题解则是通过跳过和选择，而非循环遍历。但最终效果是一样的。
```cpp
// 直接跳过
dfs(candidates, target, ans, combine, idx + 1);
// 选择当前数
if (target - candidates[idx] >= 0) {
    combine.emplace_back(candidates[idx]);
    dfs(candidates, target - candidates[idx], ans, combine, idx);
    combine.pop_back();
}
```
```cpp
if(current==candidates.size()) return;

if(target==0){
    result.push_back(path);
    return;
}

backtrack(candidates, path, current+1, target);

if(target-candidates[current]>=0){
    path.push_back(candidates[current]);
    backtrack(candidates, path, current, target-candidates[current]);
    path.pop_back();
}
```
时间复杂度O(s)，空间复杂度O(target)。