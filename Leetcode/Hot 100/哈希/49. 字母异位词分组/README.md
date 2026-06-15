# 49. 字母异位词分组
## 描述
将互为字母异位词的字符串分为一组。

## 函数声明
```cpp
std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs);
```

## 尝试写
字母异位词通过排序后是否相等判断得出。通过unordered_map存放多组字母异位词，而其索引为该组字母异位词的排序结果。最后遍历map导出结果。
```cpp
std::vector<std::vector<std::string>> result;
std::unordered_map<std::string, std::vector<std::string>> map;
for(auto& str:strs){
    std::string key=str;
    std::sort(key.begin(), key.end());
    map[key].push_back(str);
}
for(auto& [k, v]:map){
    result.push_back(v);
}
return result;
```
时间复杂度O(NMlogM)，空间复杂度O(NM)。与题解实现一致。