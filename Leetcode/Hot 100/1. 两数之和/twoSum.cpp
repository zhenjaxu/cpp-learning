#include <vector>
#include <unordered_map>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        std::unordered_map<int, int> hash;
        for(int i=0;i<nums.size();++i)
        {
            // 用值作为索引的好处在这里体现
            auto it=hash.find(target-nums[i]);
            if(it!=hash.end())
            {
                return {it->second, i};
            }
            hash[nums[i]]=i;
        }
        return {};
    }
};