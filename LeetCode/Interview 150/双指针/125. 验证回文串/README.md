# 125.验证回文串
## 问题描述
判断移除非字母数字后的字符串是否是回文串。
## 函数声明
```cpp
bool isPalindrome(std::string s);
```
## 尝试写
通过isalnum判断是否为字母或数字调整双指针至合适位置，然后判断是否为同一数字或字母，忽略大小写。按位异或可以对字母大小写翻转。但也要注意对数字的影响，数字区间在48-57，其4位上始终为1，故而异或运算不会导致与字母区间重叠。
```cpp
int left=0, right=s.length()-1;
while(left<right){
    if(!std::isalnum(static_cast<unsigned char>(s[left]))){
        left++;
    }else if(!std::isalnum(static_cast<unsigned char>(s[right]))){
        right--;
    }else{
        if(s[left]!=s[right]&&(s[left]^32)!=s[right]) return false;
        left++;
        right--;
    }
}
return true;
```
时间复杂度O(N)，空间复杂度O(1)。与题解解法一致。