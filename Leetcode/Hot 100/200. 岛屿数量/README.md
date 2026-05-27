# 200.岛屿数量
## 理解
给定二维数组，判断岛屿数量。
遍历从左向右，从上到下。如果当前为1且左边或者右边有1，就说明该岛屿与之前的相连，否则就记录新岛屿。

## 函数声明
```cpp
int numIslands(std::vector<std::vector<char>>& grid);
```

## 尝试写
遍历整个数组一次即可。
```cpp
int result=0;
for(int i=0;i<grid.size();++i){
    for(int j=0;j<grid[0].size();++j){
        if(grid[i][j]=='1'){
            if(i>0&&grid[i-1][j]=='1'){
                continue;
            }else if(j>0&&grid[i][j-1]=='1'){
                continue;
            }

            result++;
        }
    }
}
return result;
```
但是存在一种极端的情况（工字型）无法判断，导致结果偏多。

## 题解
### 深度优先搜索
通过深搜将整片岛全部搜完后，置为零。
```cpp
void dfs(std::vector<std::vector<char>>& grid, int r, int c){
    int nr=grid.size();
    int nc=grid[0].size();

    grid[r][c]='0';
    if(r>0&&grid[r-1][c]=='1') dfs(grid, r-1, c);
    if(c>0&&grid[r][c-1]=='1') dfs(grid, r, c-1);
    if(r<nr-1&&grid[r+1][c]=='1') dfs(grid, r+1, c);
    if(c<nc-1&&grid[r][c+1]=='1') dfs(grid, r, c+1);
}
```
```cpp
int nr=grid.size();
if(!nr) return 0;
int nc=grid[0].size();

int num_islands=0;
for(int r=0;r<nr;++r){
    for(int c=0;c<nc;++c){
        if(grid[r][c]=='1'){
            ++num_islands;
            dfs(grid, r, c);
        }
    }
}
return num_islands;
```
时间复杂度O(mn)，空间复杂度O(mn)。
### 广度优先搜索
每碰到一块陆地置零，然后将周围的岛屿加进队列。
```cpp
int nr=grid.size();
if(!nr) return 0;
int nc=grid[0].size();

int num_islands=0;
for(int r=0;r<nr;++r){
    for(int c=0;c<nc;++c){
        if(grid[r][c]=='1'){
            ++num_islands;
            grid[r][c]='0';
            std::queue<std::pair<int, int>> neighbors;
            neighbors.push({r, c});
            while(!neighbors.empty()){
                auto rc=neighbors.front();
                neighbors.pop();
                int row=rc.first, col=rc.second;
                if(row>0&&grid[row-1][col]=='1'){
                    neighbors.push({row-1, col});
                    grid[row-1][col]='0';
                }
                if(col>0&&grid[row][col-1]=='1'){
                    neighbors.push({row, col-1});
                    grid[row][col-1]='0';
                }
                if(row<nr-1&&grid[row+1][col]=='1'){
                    neighbors.push({row+1, col});
                    grid[row+1][col]='0';
                }
                if(col<nc-1&&grid[row][col+1]=='1'){
                    neighbors.push({row, col+1});
                    grid[row][col+1]='0';
                }
            }
        }
    }
}
return num_islands;
```
时间复杂度O(mn)，空间复杂度O(min(m,n))。
### 并查集
```cpp
class UnionFind{
public:
    UnionFind(std::vector<std::vector<char>>& grid){
        count=0;
        int m=grid.size();
        int n= grid[0].size();
        for(int i=0;i<m;++i){
            for(int j=0;j<n;++j){
                if(grid[i][j]=='1'){
                    parent.push_back(i*n+j);
                    ++count;
                }
                else{
                    parent.push_back(-1);
                }
                rank.push_back(0);
            }
        }
    }

    int find(int i){
        if(parent[i]!=i){
            parent[i]=find(parent[i]);
        }
        return parent[i];
    }

    void unite(int x, int y){
        int rootx=find(x);
        int rooty=find(y);
        if(rootx!=rooty){
            if(rank[rootx]<rank[rooty]){
                swap(rootx, rooty);
            }
            parent[rooty]=rootx;
            if(rank[rootx]==rank[rooty]) rank[rootx]+=1;
            --count;
        }
    }

    int getCount() const{
        return count;
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
    int count;
};
```
```cpp
int nr=grid.size();
if(!nr) return 0;
int nc=grid[0].size();

UnionFind uf(grid);
int num_islands=0;
for(int r=0;r<nr;++r){
    for(int c=0;c<nc;++c){
        if(grid[r][c]=='1'){
            grid[r][c]='0';
            if(r>0&&grid[r-1][c]=='1') uf.unite(r*nc+c, (r-1)*nc+c);
            if(c>0&&grid[r][c-1]=='1') uf.unite(r*nc+c, r*nc+c-1);
            if(r<nr-1&&grid[r+1][c]=='1') uf.unite(r*nc+c, (r+1)*nc+c);
            if(c<nc-1&&grid[r][c+1]=='1') uf.unite(r*nc+c, r*nc+c+1);
        }
    }
}
return uf.getCount();
```
时间复杂度O(mnxa(mn))，空间复杂度O(mn)。