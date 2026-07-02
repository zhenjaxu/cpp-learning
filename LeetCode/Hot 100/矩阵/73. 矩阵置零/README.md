# 73.矩阵置零
## 问题描述
将矩阵中出现的零的行和列全部置零。

## 函数声明
```cpp
void setZeroes(std::vector<std::vector<int>>& matrix);
```

## 尝试写
通过矩阵第一行与第一列作为记录需要置零的行和列。但操作前需要搜索第一行和第一列是否出现零，以便最后进行处理。
```cpp
int m=matrix.size();
int n=matrix[0].size();
bool rowZero=false;
bool columnZero=false;

for(int i=0;i<m;++i) if(matrix[i][0]==0) rowZero=true;
for(int j=0;j<n;++j) if(matrix[0][j]==0) columnZero=true;

for(int i=1;i<m;++i){
    for(int j=1;j<n;++j){
        if(matrix[i][j]==0){
            matrix[0][j]=0;
            matrix[i][0]=0;
        }
    }
}

for(int i=1;i<m;++i){
    for(int j=1;j<n;++j){
        if(!matrix[0][j]||!matrix[i][0]){
            matrix[i][j]=0;
        }
    }
}

if(rowZero) for(int i=0;i<m;++i) matrix[i][0]=0;
if(columnZero) for(int j=0;j<n;++j) matrix[0][j]=0;
```
时间复杂度O(MN)，空间复杂度O(1)。