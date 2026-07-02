#include<vector>

void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n){
    int p1=m-1, p2=m+n-1;
    while(p1<p2){
        if(p1!=-1&&nums1[p1]>nums2[p2-p1-1]){
            nums1[p2]=nums1[p1];
            p1--;
        }else{
            nums1[p2]=nums2[p2-p1-1];
        }
        p2--;
    }
}