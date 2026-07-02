# 88.合并两个有序数组
## 问题描述
合并两个有序数组，且nums1作为合并后的数组。
## 函数声明
```cpp
void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n);
```
## 尝试写
通过两个指针操作nums1，p1作为读取指针，而p2作为写指针。通过从后往前，从大到小的顺序执行合并，可以避免正序遍历造成的读取指针跳变的问题。而读取nums2的指针可由p2与p1之间的距离算出，即p2-p1-1。其中当m=0时需要注意判断条件，确保不会出现UB行为。
```cpp
int p1=m-1, p2=m+n-1;
while(p1<p2){
    if(p1!=-1&&nums1[p1]>nums2[p2-p1-1]){
        nums1[p2]=nums1[p1];
        p1--;
    }else{
        nums1[p2]=nums2[p2-p1-1];
    }
    p2--;
}
```
时间复杂度O(N+M)，空间复杂度O(1)。解法与题解逆序双指针思路一致。