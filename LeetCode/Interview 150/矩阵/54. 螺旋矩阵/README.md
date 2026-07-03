# 54.螺旋矩阵
## 问题描述
按顺时针顺序记录矩阵元素。
## 函数声明
```cpp
std::vector<int> spiralOrder(std::vector<std::vector<int>>& matrix);
```
## 尝试写
一次转半圈，之后计数器翻转，移动次数也会减少，直至m或n为0，即螺旋结束。
```cpp
std::vector<int> result;
int m=matrix.size();
int n=matrix[0].size();
int row=0;
int column=-1;
int reverse=1;
while(m&&n){
    for(int i=0;i<n;++i){
        column+=reverse;
        result.push_back(matrix[row][column]);
    }
    m--;
    for(int j=0;j<m;++j){
        row+=reverse;
        result.push_back(matrix[row][column]);
    }
    n--;
    reverse*=-1;
}
return result;
```
时间复杂度O(MN)，空间复杂度O(1)。与题解思路一致。