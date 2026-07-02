#include<vector>
#include<unordered_map>

int subarraySum(std::vector<int>& nums, int k);{
    std::unordered_map<int, int> map;
    map[0]=1;
    int count=0, pre=0;
    for(auto& x:nums){
        pre+=x;
        if(map.count(pre-k)){
            count+=map[pre-k];
        }
        map[pre]++;
    }
    return count;
}