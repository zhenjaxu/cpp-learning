#include<vector>

class Solution{
    void backtrack(std::vector<std::vector<int>>& res, std::vector<int>& output, int first, int len){
        if(first==len){
            res.push_back(output);
            return;
        }
        for(int i=first;i<len;++i){
            std::swap(output[i], output[first]);
            backtrack(res, output, first+1, len);
            
            std::swap(output[i], output[first]);
        }
    }

public:
    std::vector<std::vector<int>> permute(std::vector<int>& nums){
        std::vector<std::vector<int>> res;
        backtrack(res, nums, 0, nums.size());
        return res;
    }
};
