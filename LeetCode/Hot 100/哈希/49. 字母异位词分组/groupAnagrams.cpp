#include<vector>
#include<string>
#include<unordered_map>
#include<algorithm>

std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs){
    std::vector<std::vector<std::string>> result;
    std::unordered_map<std::string, std::vector<std::string>> map;
    for(auto& str:strs){
        std::string key=str;
        std::sort(key.begin(), key.end());
        map[key].push_back(str);
    }
    for(auto& [k, v]:map){
        result.push_back(v);
    }
    return result;
}