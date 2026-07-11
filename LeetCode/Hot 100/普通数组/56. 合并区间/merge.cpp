#include<vector>
#include<algorithm>

class Solution {
public:
    std::vector<std::vector<int>> merge(std::vector<std::vector<int>>& intervals) {
        return solution1(intervals);        // 排序、贪心，时间复杂度O(NlogN)，空间复杂度O(logN)
    }

private:
    std::vector<std::vector<int>> solution1(std::vector<std::vector<int>>& intervals){
        if(intervals.size()==0) return {};

        std::vector<std::vector<int>> result;
        // sort默认从两个数组的第一个元素开始比较
        std::sort(intervals.begin(), intervals.end(),
            [](std::vector<int>& a, std::vector<int>& b){ return a[0]<b[0];});    // 注：需要严格大于或小于
        result.push_back(intervals[0]);     // 放入第一个元素
        
        for(int i=1;i<intervals.size();++i){
            // 前一个元素
            int left1=result.back()[0];
            int right1=result.back()[1];

            // 当前元素
            int left2=intervals[i][0];
            int right2=intervals[i][1];

            // 相交或包含，触发合并
            if(right1>=left2) result.back()[1]=std::max(right1, right2);            // 通过比较选出右边界
            else result.push_back({left2, right2});                                 // 不需要合并则放入当前元素
        }
        return result;
    }
};