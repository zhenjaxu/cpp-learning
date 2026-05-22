#include<vector>

class Solution{
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
public:
    int climbStairs(int n){
        Matrix ret={{1, 1}, {1, 0}};
        Matrix res=matrixPow(ret, n);
        return res[0][0];       // 计算后结果与矩阵左上角一致
    }
};