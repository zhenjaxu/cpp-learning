#include<vector>
#include<string>

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

std::vector<std::vector<std::string>> solveNQueens(int n){
    size=n;
    queens.resize(n, -1);
    solve(0, 0, 0, 0);
    return solutions;
}