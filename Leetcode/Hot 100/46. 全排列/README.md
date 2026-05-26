# 46.全排列
## 理解
返回数组的全排列。
递归后回溯，遍历完一次记录一次，再换另一个分支继续遍历。

## 函数声明
```cpp
std::vector<std::vector<int>> permute(std::vector<int>& nums);
```

## 尝试写
遍历过程中先添加路径和删除数组，再递归，递归回来后再恢复路径和数组，进入下个循环。
```cpp
std::vector<std::vector<int>> result;
void traversal(std::vector<int>& nums, std::vector<int>& path){
    if(nums.empty()){
        result.push_back(path);
        return;
    }
    for(int i=0;i<nums.size();++i){
        int temp=nums[i];
        path.push_back(nums[i]);
        nums.erase(nums.begin()+i); 
        traversal(nums, path);

        path.pop_back();
        nums.insert(nums.begin()+i, temp);
    }
}
```
```cpp
result={};
std::vector<int> path={};
traversal(nums, path);
return result;
```
时间复杂度O(nxn!)，空间复杂度O(nxn)。

## 题解
使用交换元素法避免vector的频繁插入和删除。
```cpp
void backtrack(std::vector<std::vector<int>>& res, vector<int>& output, int first, int len){
    if(first==len){
        res.push_back(output);
        return;
    }
    for(int i=first;i<len;++i){
        std::swap(output[i], output[first]);
        backtrack(res, output, first+1, len);
        
        std::swap(output[i], output[first]);
    }
}
```
```cpp
std::vector<std::vector<int>> res;
backtrack(res, nums, 0, nums.size());
return res;
```
时间复杂度O(nxn!)，空间复杂度O(n)。