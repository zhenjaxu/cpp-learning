# 51.N皇后
## 问题
nxn的棋盘中摆放n个皇后，使他们不相互攻击（皇后会攻击同一行、同一列以及同一斜线上的旗子）。

## 函数声明
```cpp
std::vector<std::vector<std::string>> solveNQueens(int n);
```

## 尝试写
使用回溯的方法一层层递归。放置皇后后更新visited，回溯时将其还原，遍历下一个格子。当递归完最后层，row等于size，记录放置结果。
```cpp
int size;                                           // 等于n
std::vector<std::string> chessboard;             // 棋盘，摆放皇后
std::vector<std::vector<std::string>> result;   // 结果，记录摆放方案
std::vector<std::vector<int>> visited;           // 数组，记录被占领情况，大于0则当前位置不能摆放皇后
```
```cpp
void backtrack(int row){
    if(row==size){
        result.push_back(chessboard);
        return;
    }

    for(int i=0;i<size;++i){
        if(visited[row][i]==0){
            chessboard[row][i]='Q';
            occupy(row, i);

            backtrack(row+1);

            release(row, i);
            chessboard[row][i]='.';
        }
    }
}

void occupy(int row, int column){
    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            if(std::abs(i-row)==std::abs(j-column)&&
                i!=row&&j!=column||
                i!=row&&j==column||
                i==row&&j!=column)
            {
                visited[i][j]++;
            }
        }
    }
}

void release(int row, int column){
    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            if(std::abs(i-row)==std::abs(j-column)&&
                i!=row&&j!=column||
                i!=row&&j==column||
                i==row&&j!=column)
            {
                visited[i][j]--;
            }
        }
    }
}
```
```cpp
size=n;

chessboard.resize(n);
for(auto& row:chessboard){
    row.assign(n, '.');
}

visited.resize(n, {});
for(auto& row:visited){
    row.resize(n, 0);
}

backtrack(0);

return result;
```
时间复杂度O(n!xn2)，空间复杂度O(n2)。