#include<string>
#include<cctype>

bool isPalindrome(std::string s){
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
}