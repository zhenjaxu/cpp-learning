#include<vector>

int search(std::vector<int>& nums, int target){
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
}