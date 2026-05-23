#include<climits>
#include<vector>

int maxProfit(std::vector<int>& prices){
    int minPrice=INT_MAX;
    int numPrices=prices.size();
    int result=0;
    for(int i=0;i<numPrices;++i){
        if(prices[i]<=minPrice) minPrice=prices[i];         
        result=std::max(result, prices[i]-minPrice);
    }
    return result;
}