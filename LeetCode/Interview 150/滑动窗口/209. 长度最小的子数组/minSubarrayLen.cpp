#include<vector>
#include<climits>

int minSubArrayLen(int target, std::vector<int>& nums){
    int sum=0;
    int result=INT_MAX;
    for(int i=0,j=0;i<nums.size();++i){
        sum+=nums[i];
        while(sum>=target){
            result=std::min(result, i-j+1);
            sum-=nums[j];
            j++;
        }
    }
    return (result==INT_MAX)?0:result;
}