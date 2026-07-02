#include<vector>

int rob(std::vector<int>& nums){
    if(nums.size()==0) return 0;
    if(nums.size()==1) return nums[0];
    int curr, prev;
    prev=nums[0];
    curr=std::max(nums[0], nums[1]);
    for(int i=2;i<nums.size();++i){
        int temp=std::max(curr, prev);
        curr=std::max(curr, prev+nums[i]);
        prev=temp;
    }
    return curr;
}