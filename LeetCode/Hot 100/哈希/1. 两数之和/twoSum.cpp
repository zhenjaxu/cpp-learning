#include<vector>
#include<unordered_map>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        return solution1(nums, target);     // 哈希，时间复杂度O(N)，空间复杂度O(N)
    }

private:
    std::vector<int> solution1(std::vector<int>& nums, int target){
        std::unordered_map<int, int> numMap;    
        for(int i=0;i<nums.size();++i){
            if(numMap.count(target-nums[i])){       // key用作O(1)查找是否有满足条件且遍历过的值
                return {i, numMap[target-nums[i]]};     // 借助哈希表返回索引
            }
            numMap[nums[i]]=i;      // 遍历过的值加入哈希
        }
        return {};
    }
};