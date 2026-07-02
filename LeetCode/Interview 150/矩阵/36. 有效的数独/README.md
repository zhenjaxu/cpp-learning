# 36.有效的数独
## 问题描述
判断部分填写的数独是否有效。
## 函数声明
```cpp
bool isValidSudoku(std::vector<std::vector<char>>& board);
```
## 题解
使用三个数组分别判断行、列和九宫格内是否有效。再通过将数独中的数字作为数组索引，对其出现次数进行计数。
```cpp
int rows[9][9];
int columns[9][9];
int subboxes[3][3][9];
std::memset(rows, 0, sizeof(rows));
std::memset(columns, 0, sizeof(columns));
std::memset(subboxes, 0, sizeof(subboxes));
for(int i=0;i<9;++i){
    for(int j=0;j<9;++j){
        char c=board[i][j];
        if(c!='.'){
            int index=c-'0'-1;
            rows[i][index]++;
            columns[j][index]++;
            subboxes[i/3][j/3][index]++;
            if(rows[i][index]>1||
               columns[j][index]>1||
               subboxes[i/3][j/3][index]>1)
            {
                return false;
            }
        }
    }
}
return true;
```
时间复杂度O(1)，空间复杂度O(1)。