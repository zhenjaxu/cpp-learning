#include<vector>

void moveZeroes(std::vector<int>& nums){
    int numSize=nums.size();
    int left=0, right=0;
    while(right<numSize){
        if(nums[right]){
            std::swap(nums[left], nums[right]);
            left++;
        }
        right++;
    }
}