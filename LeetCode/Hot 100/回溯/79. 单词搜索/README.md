# 79.单词搜索
## 理解
在二维网格中沿着某一路径能搜索到对应单词则返回true。

## 函数声明
```cpp
 bool exist(std::vector<std::vector<char>>& board, std::string word);
```

## 尝试写
找到第一个符合的单词开始深搜，找到符合路径。
```cpp
class Solution {
    bool result;
    std::unordered_set<int> path;
    void backtrack(std::vector<std::vector<char>>& board, std::string& word, int currBoard, int currWord){
        int m=board.size();
        int n=board[0].size();
        int i=currBoard/n;
        int j=currBoard%n;

        if(currWord==word.size()){
            result=true;
            return;
        }

        path.insert(currBoard);
        if(i>0&&board[i-1][j]==word[currWord]&&!path.count(currBoard-n)) backtrack(board, word, currBoard-n, currWord+1);
        if(i<m-1&&board[i+1][j]==word[currWord]&&!path.count(currBoard+n)) backtrack(board, word, currBoard+n, currWord+1);
        if(j>0&&board[i][j-1]==word[currWord]&&!path.count(currBoard-1)) backtrack(board, word, currBoard-1, currWord+1);
        if(j<n-1&&board[i][j+1]==word[currWord]&&!path.count(currBoard+1)) backtrack(board, word, currBoard+1, currWord+1);
        path.erase(currBoard);
    }
public:
    bool exist(std::vector<std::vector<char>>& board, std::string word) {
        result=false;
        int m=board.size();
        int n=board[0].size();
        
        for(int i=0;i<m;++i){
            for(int j=0;j<n;++j){
                if(!result&&board[i][j]==word[0]){
                    backtrack(board, word, i*n+j, 1);
                }
            }
        }

        return result;
    }
};
```
时间复杂度O(nx3^l)，空间复杂度O(l)。当重复字母过多导致哈希冲突严重，查找效率低，导致超时。

## 题解
通过visited数组或暂时修改网格中的字母，以提高检索效率。
```cpp
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
```
时间复杂度O(nx3^l)，空间复杂度O(l)。