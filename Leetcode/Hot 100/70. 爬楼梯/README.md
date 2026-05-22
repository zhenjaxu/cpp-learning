# 70.爬楼梯
## 理解
爬到n阶楼梯一共有多少种方法。
### 递归
结果为爬到n-1阶与n-2阶的方法数之和。
### 数组
通过数组保存结果，避免重复计算。

## 尝试写
### 函数声明
```cpp
int climbStairs(int n);
```
### 递归
```cpp
if(n==0) return 0;
if(n==1) return 1;
if(n==2) return 2;
return climbStairs(n-1)+climbStairs(n-2);
```
时间复杂度O（2^n），空间复杂度O（n）。很容易实现，**重复**计算严重，运行超时。
### 数组
```cpp
if(n==0) return 0;
int array[45];
array[0]=1;
array[1]=2;
for(int i=2;i<n;++i){
    array[i]=array[i-1]+array[i-2];
}
return array[n-1];
```
时间复杂度O（n），空间复杂度O（n）。

## 题解
### 动态规划
数组虽避免了重复计算，但多了**不必要**的内存开销。计算是**连续**的，只要保存前两个的值即可。
```cpp
if(n<=2) return n;
int prev2=1;
int prev1=2;
for(int i=2;i<n;++i){
    int curr=prev1+prev2;
    prev2=prev1;
    prev1=curr;
}
return prev1;
```
时间复杂度O（n），空间复杂度O（1）。
### 矩阵快速幂
通过推导式得出矩阵，通过矩阵n次幂（使用**二进制**思路实现**快速幂**）后得结果（矩阵左上角）。
```cpp
using Matrix=std::vector<std::vector<long long>>;   // 2x2矩阵

Matrix multiply(Matrix& a, Matrix& b){
    Matrix c(2, std::vector<long long>(2));

    for(int i=0;i<2;++i){
        for(int j=0;j<2;++j){
            c[i][j]=a[i][0]*b[0][j]+a[i][1]*b[1][j];
        }
    }
    return c;
}

Matrix matrixPow(Matrix a, int n){
    Matrix ret={{1, 0}, {0, 1}};

    while(n>0){
        // 二进制位为1，记录结果
        if((n&1)==1){
            ret=multiply(ret, a);
        }
        n>>=1;
        a=multiply(a, a);   // 与二进制位同步（持续平方）
    }
    return ret;
}
```
```cpp
Matrix ret={{1, 1}, {1, 0}};
Matrix res=matrixPow(ret, n);
return res[0][0];       // 计算后结果与矩阵左上角一致
```
时间复杂度O（logn），空间复杂度O（1）。
### 通项公式
通过数学方法得**通项公式**直接求解。
```cpp
double sqrt5=std::sqrt(5);
double fibn=std::pow((1+sqrt5)/2, n+1)-std::pow((1-sqrt5)/2, n+1);
return (int)std::round(fibn/sqrt5);
```
时空复杂度主要来源pow，与CPU支持得指令集相关。