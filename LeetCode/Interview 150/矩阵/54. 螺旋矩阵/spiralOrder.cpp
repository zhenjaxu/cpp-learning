#include<vector>

std::vector<int> spiralOrder(std::vector<std::vector<int>>& matrix){
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
}