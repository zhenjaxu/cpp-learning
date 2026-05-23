# 121.买股票最佳时机
## 理解
低价买入，高价卖出，返回最大利润。
### 迭代
遍历的同时找到最低价格，并更新结果。

## 尝试写
### 函数声明
```cpp
int maxProfit(std::vector<int>& prices);
```
### 迭代
```cpp
int minIdx=0;
int result=0;
for(int i=0;i<prices.size();++i){
    if(prices[i]<=prices[minIdx]){
        minIdx=i;
        for(int j=i;j<prices.size();++j){
            result=std::max(result, prices[j]-prices[i]);
        }
    }
}
return result;
```
时间复杂度O（n2），空间复杂度O（1）。运行超时。

## 题解
### 一次遍历
略微修改一下尝试写即可。其原因在于第二层循环的非必要性。最低价格**很可能**发生改变的，导致第二层遍历的结果**全部**推翻。
```cpp
int minPrice=INT_MAX;
int numPrices=prices.size();
int result=0;
for(int i=0;i<numPrices;++i){
    if(prices[i]<=minPrice) minPrice=prices[i];         
    result=std::max(result, prices[i]-minPrice);
}
return result;
```
时间复杂度O（n），空间复杂度O（1）。