# 33.搜索旋转排序数组
## 理解
给定旋转后的升序不重复数组并检索给定值。
### 二分查找
旋转后的数组成局部升序，限制二分查找，需借助左右两边的判断。

## 尝试写
### 函数声明
```cpp
int search(std::vector<int>& nums, int target);
```
### 二分查找
尝试之后发现，由于断裂点未知导致target的位置无法锁定在某一半。而断点的检索又需要遍历**整个**数组。
但直接检索target才只需遍历一次数组，简直多此一举。
```cpp
int left, right;
left=0;
right=nums.size();
while(left<right){
    int middle=(left+right)/2;
    if(target<nums[middle]){
        if(target<nums[left]){
            // 断点在middle左侧 right=middle;
            // 断点在middle右侧 left=middle+1;
        }else right=middle;
    }else if(target>nums[middle]){
        if(target>nums[right]){
            // 断点在middle左侧 right=middle;
            // 断点在middle右侧 left=middle+1;
        }else left=middle+1;
    }else return middle;
}
return -1;
```
并未找到合适的解决方案。

## 题解
题解直接忽略断点的判断，但仍然可以求解。
其原因是通过不断地二分缩小范围，导致断点出现在查找范围外。而范围内元素有序排列，与普通二分查找相同。
```cpp
int left, right;
left=0;
right=nums.size();
while(left<right){
    int middle=(left+right)/2;
    if(nums[middle]==target) return middle;

    if(nums[left]<nums[middle]){
        if(target>=nums[left]&&target<nums[middle]){
            right=middle;
        }else{
            left=middle+1;
        }
    }else{
        if(target<=nums[right-1]&&target>nums[middle]){
            left=middle+1;
        }else{
            right=middle;
        }
    }
}
return -1;
```
时间复杂度O（logn），空间复杂度O（1）。