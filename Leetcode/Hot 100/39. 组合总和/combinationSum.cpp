#include<vector>

class Solution {
    std::vector<int> path;
    std::vector<std::vector<int>> result;
    void backtrack(std::vector<int>& candidates, std::vector<int>& path, int current, int target){
        if(target==0){
            result.push_back(path);
            return;
        }

        for(int i=current;i<candidates.size();++i){
            if(candidates[i]>target) continue;
            path.push_back(candidates[i]);
            backtrack(candidates, path, i, target-candidates[i]);
            path.pop_back();
        }
    }
public:
    std::vector<std::vector<int>> combinationSum(std::vector<int>& candidates, int target) {
        path={};
        result={};
        backtrack(candidates, path, 0, target);
        return result;
    }
};