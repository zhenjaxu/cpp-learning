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

## 题解
### 集合
题解通过四个容器分别管理行列和两条斜线。行通过递归控制，而列和斜线直接通过哈希表管理，实现O(1)的查找。位于某条斜线上的点横纵坐标之差或之和固定且唯一，很适合哈希表。
```cpp
std::vector<std::vector<std::string>> solutions;
std::vector<int> queens;
std::unordered_set<int> columns;
std::unordered_set<int> diagonals1;
std::unordered_set<int> diagonals2;
int size;

void backtrack(int row){
    if(row==size){
        std::vector<std::string> board=generateBoard();
        solutions.push_back(board);
    }else{
        for(int i=0;i<size;++i){
            if(columns.count(i)){
                continue;
            }
            int diagonal1=row-i;
            if(diagonals1.count(diagonal1)){
                continue;
            }
            int diagonal2=row+i;
            if(diagonals2.count(diagonal2)){
                continue;
            }

            queens[row]=i;
            columns.insert(i);
            diagonals1.insert(diagonal1);
            diagonals2.insert(diagonal2);

            backtrack(row+1);

            queens[row]=-1;
            columns.erase(i);
            diagonals1.erase(diagonal1);
            diagonals2.erase(diagonal2);
        }
    }
}

std::vector<std::string> generateBoard(){
    auto board=std::vector<std::string>();
    for(int i=0;i<size;++i){
        std::string row=std::string(size, '.');
        row[queens[i]]='Q';
        board.push_back(row);
    }
    return board;
}
```
```cpp
size=n;
queens.resize(n, -1);
backtrack(0);
return solutions;
```
时间复杂度O(N!)，空间复杂度O(N)。
### 位运算
通过位运算思维，使用整型代替列和斜线集合，高速运算。通过不断左移和右移实现斜向占领。
```cpp
std::vector<std::vector<std::string>> solutions;
std::vector<int> queens;
int size;

void solve(int row, int columns, int diagonal1, int diagonal2){
    if(row==size){
        auto board=generateBoard();
        solutions.push_back(board);
        return;
    }

    int availablePositions=((1<<size)-1)&(~(columns|diagonal1|diagonal2));      // int32位，通过按位与截取n位有效长度
    while(availablePositions!=0){
        int position=availablePositions&(-availablePositions);           // 补码，取反后加一，position自动选取最右边第一个可用位
        availablePositions=availablePositions&(availablePositions-1);   //  迭代，更新可用位
        int column=__builtin_ctz(position);     // 内置尾随零计数器，无需作用域

        queens[row]=column;
        solve(row+1, columns|position, (diagonal1|position)>>1, (diagonal2|position)<<1);
        queens[row]=-1;
    }
}

std::vector<std::string> generateBoard(){
    auto board=std::vector<std::string>();
    for(int i=0;i<size;++i){
        std::string row=std::string(size, '.');
        row[queens[i]]='Q';
        board.push_back(row);
    }
    return board;
}
```
```cpp
size=n;
queens.resize(n, -1);
solve(0, 0, 0, 0);
return solutions;
```
时间复杂度O(N!)，空间复杂度O(N)。