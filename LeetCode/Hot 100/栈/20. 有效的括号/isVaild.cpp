#include <string>
#include <unordered_map>
#include <stack>

bool isValid(std::string s){
    if(s.size()%2==1) return false;
        std::unordered_map<char, char> pairs{
            {')', '('},
            {']', '['},
            {'}', '{'}
        };
        std::stack<char> leftBrackets;
        for(char ch:s){
            if(pairs.count(ch)){
                if(leftBrackets.empty()||
                   pairs[ch]!=leftBrackets.top()) return false;
                leftBrackets.pop();
            }
            else leftBrackets.push(ch);   
        }
        return leftBrackets.empty()?true:false;
}