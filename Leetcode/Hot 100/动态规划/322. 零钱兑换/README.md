# 322.零钱兑换
## 理解
返回凑齐总金额最少硬币数，其中硬币可以重复无限次。

## 函数声明
```cpp
int coinChange(std::vector<int>& coins, int amount);
```

## 尝试写
不断遍历数组，将组合值记录到dp数组和队列中。当组合值等于目标值时，此时的计数器会对应着最少硬币数。
```cpp
if(amount==0) return 0;

std::vector<int> dp(amount+1, 0);
std::queue<int> que;
int count=1;

que.push(0);
while(!que.empty()){
    int s=que.size();

    while(s--){
        int n=que.front();
        que.pop();

        for(int i=0;i<coins.size();++i){
            int m=coins[i]+n;
            if(m>amount) continue;
            if(m==amount) return count;

            if(dp[m]==0){       // 如果组合值没出现过，当前计数即为最少硬币数
                dp[m]=count;
                que.push(m);    // 加入队列，进入下次组合
            }  
        }
    }

    count++;    // 该层组合结束时，计数加一
}
// 队列为空后，表示当前组合值全部超过总金额，且没有找到结果
return -1;      
```