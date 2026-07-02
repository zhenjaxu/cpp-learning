#include<vector>
#include<queue>

std::vector<std::vector<int>> edges;
std::vector<int> indeg;

bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites){
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
}