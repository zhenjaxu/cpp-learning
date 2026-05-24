#include<vector>

int searchInsert(std::vector<int>& nums, int target){
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
}