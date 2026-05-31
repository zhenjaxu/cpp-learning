# 283.移动零
## 理解
将数组中的所有零全部移到数组末尾，同时保持非零顺序。只能对原数组操作。
### 理解一，指针
指针判断元素是否为零，是0删除，然后向末尾添加0元素。

## 尝试写
### 函数声明
```cpp
void moveZeroes(std::vector<int>& nums);
```
### 方法一，指针
需要一个计数器，确保只处理n个数。
```cpp
int current=0;
int count=0;    // 用于计数，防止死循环
size_t numSize=nums.size();
while(current<numSize){
    if(nums[current]==0){
        nums.erase(nums.begin()+current);
        nums.emplace_back(0);
    }else current++;
    if(count++==numSize) break;
}
```
时间复杂度O（N），空间复杂度O（1）。

## 题解
### 双指针
原来两个指针都从头开始遍历，可以避免分别从两端遍历造成的乱序。相比方法一的删除和添加，直接交换运行速度更快。
```cpp
int numSize=nums.size();
int left=0, right=0;
while(right<numSize){
    if(nums[right]){
        std::swap(nums[left], nums[right]);
        left++;
    }
    right++;
}
```
时间复杂度O（N），空间复杂度O（1）。