#include<vector>
#include<string>

class Solution {
    bool backtrack(std::vector<std::vector<char>>& board, std::string& word, int i, int j, int k){
        if(board[i][j]!=word[k]) return false;
        if(k==word.length()-1) return true;

        int m=board.size();
        int n=board[0].size();

        char temp=board[i][j];
        board[i][j]='#';

        bool result=false;
        result=i>0&&backtrack(board, word, i-1, j, k+1)||
               i<m-1&&backtrack(board, word, i+1, j, k+1)||
               j>0&&backtrack(board, word, i, j-1, k+1)||
               j<n-1&&backtrack(board, word, i, j+1, k+1);
               
        board[i][j]=temp;
        return result;
    }

public:
    bool exist(std::vector<std::vector<char>>& board, std::string word) {
        int m=board.size();
        int n=board[0].size();
           
        for(int i=0;i<m;++i){
            for(int j=0;j<n;++j){
                if(backtrack(board, word, i, j, 0)) return true;
            }
        }
        return false;
    }
};