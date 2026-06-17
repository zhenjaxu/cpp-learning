# 3.无重复字符的最长子串
## 问题描述
返回无重复字符最长子串的长度。

## 函数声明
```cpp
int lengthOfLongestSubstring(std::string s);
```

## 尝试写
通过哈希表判断字母是否出现过。在通过哈希表中存放的索引值调整左边界，使得窗口内字符串保证无重复字符，并更新哈希表。
```cpp
if(s.empty()) return 0;

int left=0, right=0;
std::unordered_map<char, int> map;
int result=0;
while(right<s.size()){
    if(map.count(s[right])){
        while(left<map[s[right]]){
            map.erase(s[left]);
            left++;
        }
        left=map[s[right]]+1;
    }
    
    map[s[right]]=right;
    result=std::max(result, right-left+1);
    right++;
}
return result;
```
时间复杂度O(N)，空间复杂度O(N)。