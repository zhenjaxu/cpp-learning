#include<vector>
#include<string>
#include<algorithm>

class Solution {
public:
    std::vector<int> findAnagrams(std::string s, std::string p) {
        // return solution1(s, p);     // 排序，时间复杂度O(MN)，空间复杂度O(1)
        return solution2(s, p);     // 滑动窗口，时间复杂度O(M+N)，空间复杂度O(1)
    }

private:
    // 排序，运行超时
    std::vector<int> solution1(std::string& s, std::string& p){
        if(s.size()<p.size()) return {};

        std::vector<int> result; 
        std::sort(p.begin(), p.end());      // 对p排序
        int pSize=p.size();
        for(int i=0;i<s.size()-pSize+1;++i){    // i不超过sSize-pSize
            std::string sub=s.substr(i, pSize);      // 取pSize大小子串排序，与排序后的p相同则为异位词
            std::sort(sub.begin(), sub.end());
            if(sub==p) result.push_back(i);
        }
        return result;
    }

    // 滑动窗口
    std::vector<int> solution2(std::string& s, std::string& p){
        int sSize=s.size(), pSize=p.size();
        if(sSize<pSize) return {};

        std::vector<int> result;
        std::vector<int> count(26);     // 26字母计数器
        for(int i=0;i<pSize;++i){
            ++count[s[i]-'a'];
            --count[p[i]-'a'];
        }

        int diff=0;     // 记录子串中与p不同数量的字母个数
        for(int i=0;i<26;++i){
            if(count[i]!=0) diff++;
        }
        if(diff==0) result.emplace_back(0);

        for(int i=0;i<sSize-pSize;++i){
            if(count[s[i]-'a']==1) --diff;      // 窗口中s[i]的数量减一后，与字符串p中的数量相同
            else if(count[s[i]-'a']==0) ++diff;     // 窗口中s[i]的数量减一后，与字符串p中的数量变成不同
            --count[s[i]-'a'];

            if(count[s[i+pSize]-'a']==-1) --diff;      // 窗口中s[i+pSize]的数量加一后，与字符串p中的数量相同
            else if(count[s[i+pSize]-'a']==0) ++diff;     // 窗口中s[i+pSize]的数量加一后，与字符串p中的数量变成不同
            ++count[s[i+pSize]-'a'];

            if(diff==0){
                result.emplace_back(i+1);       // 窗口左端位置实际为i+1
            }
        }
        return result;
    }
};