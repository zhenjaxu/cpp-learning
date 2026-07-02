# 搜索插入位置
## 理解
保证升序，找到插入位置。
### 二分查找
二分查找因未找到而**退出**的位置就是插入位置。

## 尝试写
### 函数声明
```cpp
int searchInsert(vector<int>& nums, int target);
```
### 二分查找
保证left、right左闭右开。退出循环时left与right重合，此处即为插入位置。
```cpp
int left, right;
left=0;
right=nums.size();
while(left<right){
    int middle=(left+right)/2;
    if(nums[middle]>target){
        right=middle;
    }else if(nums[middle]<target){
        left=middle+1;
    }else{
        return middle;      // 相等直接插入
    }
}
return left;        // 退出点即插入位置
```
时间复杂度O（logn），空间复杂度O（1）。