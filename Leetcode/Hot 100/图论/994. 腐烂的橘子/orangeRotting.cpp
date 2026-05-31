#include<vector>
#include<queue>

int orangesRotting(std::vector<std::vector<int>>& grid){
    std::queue<std::pair<int, int>> rottings;
    int m=grid.size();
    int n=grid[0].size();
    for(int i=0;i<m;++i){
        for(int j=0;j<n;++j){
            if(grid[i][j]==2){
                rottings.push({i, j});
            }
        }
    }

    int count=-1;
    while(!rottings.empty()){
        count++;

        int s=rottings.size();
        while(s--){
            std::pair<int, int> rot=rottings.front();
            rottings.pop();

            int x=rot.first;
            int y=rot.second;
            if(x>0&&grid[x-1][y]==1){
                grid[x-1][y]=2;
                rottings.push({x-1, y});
            }
            if(x<m-1&&grid[x+1][y]==1){
                grid[x+1][y]=2;
                rottings.push({x+1, y});
            }if(y>0&&grid[x][y-1]==1){
                grid[x][y-1]=2;
                rottings.push({x, y-1});
            }if(y<n-1&&grid[x][y+1]==1){
                grid[x][y+1]=2;
                rottings.push({x, y+1});
            }
        }
    }

    for(int i=0;i<m;++i){
        for(int j=0;j<n;++j){
            if(grid[i][j]==1){
                return -1;
            }
        }
    }
    // 等于-1说明既没有腐烂的也没有新鲜的，返回0
    return count==-1?0:count; 
}

