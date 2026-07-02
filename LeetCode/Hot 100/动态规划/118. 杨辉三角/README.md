# 118.杨辉三角
## 描述
给定行数，生成杨辉三角。

## 函数声明
```cpp
std::vector<std::vector<int>> generate(int numRows);
```
## 尝试写
当前行的结果由前一行保存的结果得出。
```cpp
std::vector<std::vector<int>> result;
for(int i=0;i<numRows;++i){
    std::vector<int> temp(i+1, 1);
    int count=i-1;
    while(count>0){
        temp[count]=result[i-1][count]+result[i-1][count-1];
        count--;
    }
    result.push_back(temp);
}
return result;
```
时间复杂度O(N2)，空间复杂度O(N2)。