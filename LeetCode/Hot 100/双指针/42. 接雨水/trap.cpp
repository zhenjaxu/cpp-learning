#include<vector>

class Solution {
public:
    int trap(std::vector<int>& height) {
        // return solution1(height);     // 暴力解法，时间复杂度O(N^2)，空间复杂度O(1) 
        // return solution2(height);       // 数组预处理，时间复杂度O(N)，空间复杂度O(1)
        return solution3(height);       // 双指针，时间复杂度O(N)，空间复杂度O(1)
    }

    // 暴力解法，运行超时
    int solution1(std::vector<int>& height){
        int water=0;
        for(int i=0;i<height.size();++i){
            int leftMax=0, rightMax=0;

            for(int left=i;left>=0;--left){
                leftMax=std::max(leftMax, height[left]);
            }

            for(int right=i;right<height.size();++right){
                rightMax=std::max(rightMax, height[right]);
            }

            water+=std::min(leftMax, rightMax)-height[i];
        }
        return water;
    }

    // 数组预处理
    int solution2(std::vector<int>& height){
        std::vector<int> leftMax(height.size(), 0);
        std::vector<int> rightMax(height.size(), 0);
        int maxLeft=0;
        int maxRight=0;
        int water=0;
        
        // 预处理i位置处左右两边最大值
        for(int i=0;i<height.size();++i){
            maxLeft=std::max(maxLeft, height[i]);
            leftMax[i]=maxLeft;
        }
        
        for(int i=height.size()-1;i>=0;--i){
            maxRight=std::max(maxRight, height[i]);
            rightMax[i]=maxRight;
        }

        // 最后根据预处理数组计算积雨量
        for(int i=0;i<height.size();++i){
            water+=std::min(leftMax[i], rightMax[i])-height[i];
        }

        return water;
    }

    // 双指针
    int solution3(std::vector<int>& height){
        int left=0, right=height.size()-1;
        int leftMax=0, rightMax=0;
        int water=0;

        while(left<=right){
            // 左侧为瓶颈
            if(height[left]<height[right]){
                if(height[left]>leftMax){
                    leftMax=height[left];   // 如果大于最大值，则说明该处无法装水，更新最大值
                }else{
                    water+=leftMax-height[left];    // 如果小于，则说明可以装，且多少由瓶颈决定
                    left+=1;        // 指针向内夹逼
                }
            }

            // 右侧为瓶颈
            else{ 
                if(height[right]>rightMax){
                    rightMax=height[right];
                }else{
                    water+=rightMax-height[right];
                    right-=1;
                }
            }
        }

        return water;
    }
};