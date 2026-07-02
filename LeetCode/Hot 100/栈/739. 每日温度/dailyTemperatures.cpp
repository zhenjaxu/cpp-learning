#include<vector>
#include<stack>

std::vector<int> dailyTemperatures(std::vector<int>& temperatures) {
    std::stack<int> stk;
    std::vector<int> result(temperatures.size(), 0);
    for(int i=0;i<temperatures.size();++i){
        while(!stk.empty()&&temperatures[i]>temperatures[stk.top()]){
            result[stk.top()]=i-stk.top();
            stk.pop();
        }

        stk.push(i);
    }
    return result;
}