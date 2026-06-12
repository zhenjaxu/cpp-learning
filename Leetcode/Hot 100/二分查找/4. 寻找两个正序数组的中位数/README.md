# 4.寻找两个正序数组的中位数
## 问题
给定长度m和n的两个正序数组，返回它们中位数。

## 题解
### 划分数组
通过保持分割线左边两数组元素个数之和固定，二分查找较短的数组，保证中位数与分割线两侧元素有关。若查找较长数组，则会导致有固定值求出来的索引不在较短数组中，所以需要保证二分查找的对象是较短数组。对于i,j==0或m等极端情况，需保证分割线两侧取值不被影响，使用INT_MAX和INT_MIN解决。最后分奇偶计算中位数。
```cpp
if(nums1.size()>nums2.size()){
    return findMedianSortedArrays(nums2, nums1);
}

int m=nums1.size();
int n=nums2.size();

int left=0, right=m;
int median1=0, median2=0;

while(left<=right){
    int i=(left+right)/2;
    int j=(m+n+1)/2-i;

    int nums_im1=(i==0?INT_MIN:nums1[i-1]);
    int nums_i=(i==m?INT_MAX:nums1[i]);
    int nums_jm1=(j==0?INT_MIN:nums2[j-1]);
    int nums_j=(j==n?INT_MAX:nums2[j]);

    if(nums_im1<=nums_j){
        median1=std::max(nums_im1, nums_jm1);
        median2=std::min(nums_i, nums_j);
        left=i+1;
    }else{
        right=i-1;
    }
}

return (m+n)%2==0?(median1+median2)/2.0:median1;
```
时间复杂度O(log(min(m,n)))，空间复杂度O(1)。