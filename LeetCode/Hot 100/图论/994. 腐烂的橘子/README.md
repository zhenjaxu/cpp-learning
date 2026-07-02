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
        }if(x<m-1&&grid[x+1][y]==1){
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

## 题解
题解与尝试写的思路相似，不过题解在一开始记录新鲜橘子数量，在腐烂过程中减去，时间复杂度更低。此外还通过数组来记录腐烂时间以及改变方向。
```cpp
int cnt;
int dis[10][10];
int dir_x[4] = {0, 1, 0, -1};
int dir_y[4] = {1, 0, -1, 0};
```
```cpp
std::queue<std::pair<int, int>>Q;
std::memset(dis, -1, sizeof(dis));
cnt = 0;
int n = (int)grid.size(), m = (int)grid[0].size(), ans = 0;
for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
        if (grid[i][j] == 2) {
            Q.emplace(i, j);
            dis[i][j] = 0;
        }
        else if (grid[i][j] == 1) {
            cnt += 1;
        }
    }
}

while (!Q.empty()){
    auto [r, c] = Q.front();
    Q.pop();
    for (int i = 0; i < 4; ++i) {
        int tx = r + dir_x[i];
        int ty = c + dir_y[i];
        if (tx < 0|| tx >= n || ty < 0|| ty >= m || ~dis[tx][ty] || !grid[tx][ty]) {
            continue;
        }
        dis[tx][ty] = dis[r][c] + 1;
        Q.emplace(tx, ty);
        if (grid[tx][ty] == 1) {
            cnt -= 1;
            ans = dis[tx][ty];
            if (!cnt) {
                break;
            }
        }
    }
}
return cnt ? -1 : ans;
```
时间复杂度O(2n)，空间复杂度O(2n)。