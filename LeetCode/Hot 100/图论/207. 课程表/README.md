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

## 题解
### 深搜
题解使用edges存放后续可学习的课程，这种拓扑的思维直接避免了每次去遍历prerequisites找对应的课程。通过visited设置课程的三种状态，同时进行环的检查和已学过课程的标记，降低时间复杂度和空间复杂度。
```cpp
std::vector<std::vector<int>> edges;
std::vector<int> visited;
bool valid=true;

void dfs(int u){
    visited[u]=1;
    for(int v:edges[u]){
        if(visited[v]==0){
            dfs(v);
            if(!valid){
                return;
            }
        }else if(visited[v]==1){
            valid=false;
            return;
        }
    }
    visited[u]=2;
}
```
```cpp
edges.resize(numCourses);
visited.resize(numCourses);
for(const auto& info:prerequisites){
    edges[info[1]].push_back(info[0]);
}

for(int i=0;i<numCourses&&valid;++i){
    if(!visited[i]){
        dfs(i);
    }
}
return valid;
```
时间复杂度O(n+m)，空间复杂度O(n+m)。
### 广搜
使用同样的方法使用edges存放后续课程。广搜通过入度判断当前课程是否可以学习，入度会随着前置课程的学习而减少。如后续课程可学，放入队列中进入下一次学习。最后需要一个计数器visited在学习的过程中记录学习课程的数目，避免再次遍历。
```cpp
std::vector<std::vector<int>> edges;
std::vector<int> indeg;
```
```cpp
edges.resize(numCourses);
indeg.resize(numCourses);

for(const auto& info:prerequisites){
    edges[info[1]].push_back(info[0]);
    ++indeg[info[0]];
}

std::queue<int> q;
for(int i=0;i<numCourses;++i){
    if(indeg[i]==0){
        q.push(i);
    }
}

int visited=0;
while(!q.empty()){
    ++visited;
    int u=q.front();q.pop();

    for(int v:edges[u]){
        --indeg[v];
        if(indeg[v]==0){
            q.push(v);
        }
    }
}

return visited==numCourses;
```
时间复杂度O(n+m)，空间复杂度O(m+n)。