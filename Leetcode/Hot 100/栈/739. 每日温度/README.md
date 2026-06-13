# 739.每日温度
## 描述
给定连续n天的温度数组，返回一个记录下次高温度在几天后的数组。如果温度不再升高记录为0。

## 函数声明
```cpp
std::vector<int> dailyTemperatures(std::vector<int>& temperatures);
```

## 尝试写
使用栈保存一时得不出结果的日期，在满足条件后，记录天数，并弹出。
```cpp
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
```
时间复杂度O(N)，空间复杂度O(N)。写法和题解一致。