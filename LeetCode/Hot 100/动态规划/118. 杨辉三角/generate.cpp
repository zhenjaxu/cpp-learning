#include<vector>

std::vector<std::vector<int>> generate(int numRows){
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
}