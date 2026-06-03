# 394.字符串解码
## 理解
对给定字符串进行解码，将k[str]转为重复k次的str。

## 函数声明
```cpp
std::string decodeString(std::string s);
```

## 尝试写
遍历传入的数组，通过递归获取所有子串，最后合到一起返回。
```cpp
std::vector<std::pair<std::string, int>> subs;

std::string sub="";
for(int i=0;i<s.length();){
    // 处理特殊类型
    if(s[i]>='0'&&s[i]<='9'){
        // 保存普通类型
        if(!sub.empty()){
            subs.emplace_back(sub, 1);
            sub="";
        }

        // 计算重复次数
        int k=0;
        while(s[i]!='['){
            k=10*k+(s[i]-'0');
            i++;
        }

        // 通过栈判断括号结束位置，获取完整子串
        std::stack<char> stack;
        stack.push(s[i++]);
        while(!stack.empty()){
            if(s[i]=='[') stack.push(s[i]);
            if(s[i]==']') stack.pop();
            sub+=s[i++];    // 全部记录，最后交给递归
        }
        sub.pop_back();     // 弹出多余的']'

        subs.emplace_back(sub, k);
        sub="";
    }
    
    else sub+=s[i++];       // 普通类型直接获取
}

// 合并子串
std::string result="";
for(auto iter:subs){
    sub=decodeString(iter.first);
    for(int i=0;i<iter.second;++i){
        result+=sub;
    }
}

// 避免纯普通类型进入subs，导致无限递归
// 所以末尾存在普通类型，则在最后添加
// 也可在开头处查找是否为纯普通类型，直接返回整个字符串
result+=sub;    
return result;
```
时间复杂度O(n+s)，空间复杂度O(s)。

## 题解
### 栈操作
题解通过栈存放遍历的字母和数字。在遇到']'时，弹出字母，合并并翻转。弹出数字，重复子串并记录。遍历完后，栈中存放所有解码完成的子串，合并并返回结果。通过vector模拟栈行为，同时也能在最后的合并过程中，从栈底开始遍历，减少一次翻转操作。
```cpp
std::vector<std::string> stk;
size_t ptr=0;

while(ptr<s.size()){
    char cur=s[ptr];
    if(std::isdigit(cur)){
        std::string digits="";
        while(isdigit(s[ptr])){
            digits+=s[ptr++];
        }

        stk.push_back(digits);
    }
    
    else if(std::isalpha(cur)||cur=='['){
        stk.push_back(std::string(1, s[ptr++]));
    }
    
    else{
        ++ptr;
        std::vector<std::string> sub;
        while(stk.back()!="["){
            sub.push_back(stk.back());
            stk.pop_back();
        }

        std::reverse(sub.begin(), sub.end());

        stk.pop_back();

        int repTime=std::stoi(stk.back());
        stk.pop_back();
        
        std::string o="";
        for(const auto& s:sub){
            o+=s;
        }

        std::string t="";
        while(repTime--) t+=o;
        stk.push_back(t);
    }
}

std::string result="";
for(const auto& sub:stk){
    result+=sub;
}
return result;
```
时间复杂度O(n+s)，空间复杂度O(s)。
### 递归
递归的关键就是层次拆解，最终会落实到纯字母。而返回的时候继续调用getString，完成后续子串的拼接。
```cpp
std::string src;
size_t ptr;

int getDigits(){
    int ret=0;
    while(ptr<src.size()&&std::isdigit(src[ptr])){
        ret=ret*10+src[ptr++]-'0';
    }
    return ret;
}

std::string getString(){
    if(ptr==src.size()||src[ptr]==']'){
        return "";
    }

    char cur=src[ptr];
    int repTime=1;
    std::string ret;

    if(isdigit(cur)){
        repTime=getDigits();
        ++ptr;

        std::string str=getString();
        ++ptr;

        while(repTime--) ret+=str;
    }

    else if(std::isalpha(cur)){
        ret=std::string(1, src[ptr++]);
    }

    return ret+getString();
}
```
时间复杂度O(n+s)，空间复杂度O(s)。