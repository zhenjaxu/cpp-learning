#include<vector>

int maxArea(std::vector<int>& height){
    int left=0, right=height.size()-1;
    int isLeft=-1;
    int preHeight=0;
    int result=0;
    while(left<right){
        if(isLeft!=-1){
            if(isLeft){
                if(height[++left]<=preHeight) continue;
            }else{
                if(height[--right]<=preHeight) continue;
            }
        }

        int nowHeight=std::min(height[left], height[right]);
        int nowWidth=right-left;
        if(height[left]==nowHeight) isLeft=1;            
        else isLeft=0;
        result=std::max(result, nowHeight*nowWidth);
        preHeight=nowHeight;
    }
    return result;
}