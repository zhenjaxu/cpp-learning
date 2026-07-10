#include<vector>
#include<algorithm>

class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        return solution1(nums);     // 排序、双指针，时间复杂度O(N^2)，空间复杂度O(1)
    }

private:
    std::vector<std::vector<int>> solution1(std::vector<int>& nums) {
        std::vector<std::vector<int>> result;
        std::sort(nums.begin(), nums.end());        // 排序
        for(int i=0;i<nums.size();++i){
            if(i>0&&nums[i]==nums[i-1]) continue;   // 需要与上次不同
            int target=-nums[i];
            int left=i+1;
            int right=nums.size()-1;
            for(;left<nums.size();++left){
                if(left>i+1&&nums[left]==nums[left-1]) continue;    // 保证left不重复，则right必然不会重复
                while(left<right&&nums[left]+nums[right]>target) right--;   // 保证left在right左侧，搜索到合适位置
                if(left==right) break;      // 如果left==right时，两数之和还是大于target，则后续将没有符合的值
                if(nums[left]+nums[right]==target) result.push_back({nums[i], nums[left], nums[right]});
                // 如果和小于target，则增大左值继续搜索
            }
        }
        return result;
    }
};