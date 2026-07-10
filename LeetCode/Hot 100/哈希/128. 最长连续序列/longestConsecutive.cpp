#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<climits>

class Solution {
public:
    int longestConsecutive(std::vector<int>& nums) {
        // return solution1(nums);     // 哈希、并查集
        return solution2(nums);     // 哈希
    }

private:
    // 哈希、并查集
    int solution1(std::vector<int>& nums){
        std::unordered_map<int, int> countMap;
        std::unordered_map<int, int> numMap;
        for(const auto& n:nums){
            int m=n-1;      // 前一个数
            int o=n+1;      // 后一个数

            // 是否遍历过
            bool IsCount_m=numMap.count(m);
            bool IsCount_o=numMap.count(o);
            bool IsCount_n=numMap.count(n);

            // m/n/o都未遍历
            if(!IsCount_m&&!IsCount_o&&!IsCount_n){
                numMap[n]=n;
                countMap[n]=1;
            }
            
            // 存在m/o，但n未遍历
            else if(!IsCount_n){
                if(IsCount_m&&IsCount_o){
                    // 延numMap找有countMap记录的根
                    int root_m=numMap[m];
                    while(!countMap.count(root_m)){
                        root_m=numMap[root_m];
                    }

                    int root_o=numMap[o];
                    while(!countMap.count(root_o)){
                        root_o=numMap[root_o];
                    }

                    numMap[root_o]=root_m;      // 将root_o并入root_m
                    numMap[n]=root_m;

                    countMap[root_m]+=countMap[root_o]+1;   // 更新合并后的计数
                    countMap.erase(root_o);     // 并入后root_o为假根，从countMap中移除
                }
                
                // m遍历过，同上更新计数
                else if(IsCount_m){
                    int root_m=numMap[m];
                    while(!countMap.count(root_m)){
                        root_m=numMap[root_m];
                    }

                    numMap[n]=root_m;
                    countMap[root_m]++;
                }
                
                // o遍历过，同上更新计数
                else{
                    int root_o=numMap[o];
                    while(!countMap.count(root_o)){
                        root_o=numMap[root_o];
                    }

                    numMap[n]=root_o;
                    countMap[root_o]++;
                }   
            }

            // 遍历过n则不处理
        }

        int maxCount=INT_MIN;
        for(auto& c:countMap){
            maxCount=std::max(maxCount, c.second);
        }

        return maxCount==INT_MIN?0:maxCount;
    }

    // 哈希
    int solution2(std::vector<int>& nums){
        std::unordered_set<int> numSet;
        // 去重，加入hash
        for(const auto& n:nums){
            numSet.insert(n);
        }

        int longestStreak=0;
        for(const auto& n:numSet){
            if(!numSet.count(n-1)){                     // 搜索到第一个数，开始遍历
                int currentNum=n;
                int currentStreak=1;

                while(numSet.count(currentNum+1)){      // 连续向后O(1)查找，直到搜索完整段连续序列
                    currentNum+=1;
                    currentStreak+=1;
                }

                longestStreak=std::max(longestStreak, currentStreak);   // 更新计数
            }
        }

        return longestStreak;
    }
};