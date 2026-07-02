#include<vector>

std::vector<std::vector<int>> subsets(std::vector<int>& nums){
    std::vector<int> t;
    std::vector<std::vector<int>> ans;
    int n=nums.size();
    for(int mask=0;mask<(1<<n);++mask){     // 1<<n=2^n
        t.clear();
        for(int i=0;i<n;++i){
            if(mask&(1<<i)){    // 如001, 010, 100等
                t.push_back(nums[i]);
            }
        }
        ans.push_back(t);
    }
    return ans;
}