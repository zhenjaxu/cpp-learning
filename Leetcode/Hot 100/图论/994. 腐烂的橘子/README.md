# 994.腐烂的橘子
## 理解
每分钟腐烂会将四个方向相邻的新橘子腐烂，返回所有橘子腐烂的时间。
搜索出所有腐烂的橘子并进行广搜和计数。

## 函数声明
```cpp
int orangesRotting(std::vector<std::vector<int>>& grid);
```

## 尝试写
队列存放腐烂橘子，搜索前记录大小，保证只更新一分钟。
当队列为空后判断是否还有新鲜橘子。有则说明永远不会腐烂。
```cpp
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
```
时间复杂度O(3n)，空间复杂度O(n)。