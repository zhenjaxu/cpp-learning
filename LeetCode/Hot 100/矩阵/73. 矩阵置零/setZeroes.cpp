#include<vector>

void setZeroes(std::vector<std::vector<int>>& matrix){
    int m=matrix.size();
    int n=matrix[0].size();
    bool rowZero=false;
    bool columnZero=false;

    for(int i=0;i<m;++i) if(!matrix[i][0]) rowZero=true;
    for(int j=0;j<n;++j) if(!matrix[0][j]) columnZero=true;

    for(int i=1;i<m;++i){
        for(int j=1;j<n;++j){
            if(!matrix[i][j]){
                matrix[0][j]=matrix[i][0]=0;
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
}