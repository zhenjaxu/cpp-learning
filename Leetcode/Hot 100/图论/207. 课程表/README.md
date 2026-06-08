# 207.课程表
## 理解
按照选课前置要求，查看是否能选完全部课程。

## 函数声明
```cpp
bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites);
```

## 尝试写
遍历每个课程，查看是否可以学习。通过栈存储前置的多个课程，进行递归。通过哈希表存放遍历过的路径，使用回溯思维自动管理路径。
```cpp
class Solution {
    std::unordered_set<int> visiteds;
    
    bool canStudy(std::vector<std::vector<int>>& prerequisites, int course, std::vector<int>& studieds){
        if(visiteds.count(course)) return false;
        visiteds.insert(course);

        std::stack<int> stk;
        for(int i=0;i<prerequisites.size();++i){
            if(prerequisites[i][0]==course){ 
                if(!studieds[prerequisites[i][1]]) stk.push(prerequisites[i][1]);
            }
        }

        while(!stk.empty()){
            int top=stk.top();stk.pop();
            if(!canStudy(prerequisites, top, studieds)) return false;
        }
        
        studieds[course]=true;
        visiteds.erase(course);

        return true;
    }
public:
    bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        std::vector<int> studieds(numCourses, false);

        for(int i=0;i<numCourses;++i){
            if(!canStudy(prerequisites, i, studieds)) return false;
        }

        return true;
    }
};
```
时间复杂度O(n*(m+n))，空间复杂度O(2n)。