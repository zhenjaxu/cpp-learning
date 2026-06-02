# 153.寻找旋转排序数组中的最小值
## 理解
以O(logn)的时间复杂度找出旋转排序数组中的最小值。

## 函数声明
```cpp
int findMin(std::vector<int>& nums);
```

## 尝试写
与33搜索旋转排序数组相似，通过避开断层，不断更新最小值。
```cpp
int min=INT_MAX;
int left, right;
left=0;
right=nums.size();
while(left<right){
    int middle=(left+right)/2;
    if(nums[left]<nums[middle]){
        min=std::min(nums[left], min);
        left=middle+1;
    }else{
        min=std::min(nums[middle], min);
        right=middle;
    }
}
return min;
```
时间复杂度O(logn)，空间复杂度O(1)。

## 题解
题解则是将left指针锁定在最小值位置。与前面的方法不同，必须判断右侧是否为升序数组。
因为并不知道是否存在断层，而最小值很可能在最左侧。
其次是终止条件，在middle等于right发生前就需终止，让left始终停在最小值处。
```cpp
int left=0;
int right=nums.size()-1;
while(left<right){
    int middle=(left+right)/2;
    if(nums[middle]<nums[right]){
        right=middle;
    }else{
        left=middle+1;
    }
}
return nums[left];
```