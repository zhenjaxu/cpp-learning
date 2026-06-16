# 11.盛最多水的容器
## 问题描述
选出两条高作为容器边缘，使得该容器盛水最多，返回最大水量。

## 函数声明
```cpp
int maxArea(std::vector<int>& height);
```

## 尝试写
遍历所有情况，但做了剪枝。对right、left和width做了限制，只对有可能产生更大值的参数进行计算。
```cpp
int left;
int right=height.size();
int preWidth=0;
int preHeight=0;
int max=0;
while(right--){
    if(height[right]<preHeight) continue;
    int width=right;
    for(left=0;left<right;++left,--width){
        if(height[left]<preHeight&&width<preWidth) continue;
        int newMax=std::max(max, std::min(height[left], height[right])*width);
        if(newMax!=max){
            max=newMax;
            preWidth=width;
            preHeight=newMax/preWidth;
        }
        
    }
}
return max;
```
时间复杂度O(N2)，空间复杂度O(1)。

## 题解
双指针分别从两端开始向中间遍历，使得容器尽可能的宽。移动较矮一端的指针，使得容器尽可能的高。通过isLeft和preHeight对搜索剪枝，对可能产生更大值的参数进行计算。
```cpp
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
```
时间复杂度O(N)，空间复杂度O(1)。