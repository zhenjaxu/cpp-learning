#include<vector>

int coinChange(std::vector<int>& coins, int amount){
    int Max=amount+1;
    std::vector<int> dp(amount+1, Max);
    dp[0]=0;
    for(int i=1;i<=amount;++i){
        for(int j=0;j<(int)coins.size();++j){
            if(coins[j]<=i){
                dp[i]=std::min(dp[i], dp[i-coins[j]]+1);
            }
        }
    }
    return dp[amount]>amount?-1:dp[amount];
}