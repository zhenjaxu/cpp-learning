#include<string>

class Solution {
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

public:
    std::string decodeString(std::string s) {
        src=s;
        ptr=0;
        return getString();
    }  
};