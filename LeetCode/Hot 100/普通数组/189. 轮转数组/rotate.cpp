#include<vector>
#include<stack>
#include<numeric>   // std::gcd()

class Solution {
public:
    void rotate(std::vector<int>& nums, int k) {
        // solution1(nums, k);     // 额外空间，时间复杂度O(N)，空间复杂度O(k%n)
        // solution2(nums, k);     // 环状替换，时间复杂度O(N)，空间复杂度O(1)
        solution3(nums, k);     // 翻转数组，时间复杂度O(N)，空间复杂度O(1)
    }

private:
    // 额外空间
    void solution1(std::vector<int>& nums, int k){
        k%=nums.size();     // 取模，避免超界
        std::stack<int> stk;
        for(int i=nums.size()-1;i>=0;--i){
            if(i>=nums.size()-k) stk.push(nums[i]);     // 先从取末尾k个数放入栈中
            else nums[i+k]=nums[i];                     // 之后每个数向后移动k位
        }

        // 就栈中k个数弹出放在前面
        for(int i=0;i<k;++i){
            nums[i]=stk.top();          
            stk.pop();
        }
    }

    // 环状替换
    void solution2(std::vector<int>& nums, int k){
        int n=nums.size();
        k%=n;
        int count=std::gcd(n, k);   // 最小公约数（greatest common divisor），形成几个闭环就需要环状替换多少次
        for(int start=0;start<count;++start){
            int current=start;
            do{
                int next=(current+k)%n;     
                std::swap(nums[next], nums[start]);     // 始终与start交换
                current=next;
            }while(start!=current);     // 直到闭环，然后开始替换下一个闭环
        }
    }

    // 翻转数组
    void solution3(std::vector<int>& nums, int k){
        k%=nums.size();
        reverse(nums, 0, nums.size()-1);        // 翻转所有元素
        reverse(nums, 0, k-1);                  // 翻转前k个元素
        reverse(nums, k, nums.size()-1);        // 翻转后面的元素
    }

    void reverse(std::vector<int>& nums, int start, int end){
        while(start<end){
            std::swap(nums[start], nums[end]);
            start+=1;
            end-=1;
        }
    }
};