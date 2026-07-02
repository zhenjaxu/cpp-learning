# 1.两数之和
## 理解
返回的是不值本身，而是索引

## 尝试写
### 方法一
首先想到的就是双指针
一个快一个慢，快的从慢的后面开始遍历，遍历到末尾后进入下一次遍历
快的每遍历完一次，就向后移动一步，知道找到答案或遍历到最后一个数
```cpp
// 一快一慢
int fast, slow;
// 开始遍历
// 两层循环，一层遍历slow，一层遍历fast
// fast每次从slow后面开始
// fast遍历完后slow向后面移动
// 然后再最里面判断，如果找到就返回
slow=0;
while(slow<nums.size()-1)
{
    fast=slow+1;
    while(fast<nums.size())
    {
        if(nums[slow]+nums[fast]==target)
        {
            return std::vector<int>{slow, fast};
        }
        fast++;
    }
    slow++;
}
return std::vector<int>();
```
时间复杂度O（N^2），至多slow遍历完整个数组，fast每次从slow后面出发，每循环一次遍历一层的时间就少一点
空间复杂度O（1），只有几个指针，常数的内存足以

### 方法二
尝试递归，最终形态肯定就是数组为空，或者找到答案
```cpp
首先就是判断数组是否为空
if(nums.empty())
{
    return std::vector<int>();
}
// 然后就是判断是否满足条件
// 但是这里要判断就得遍历
for(int i=1;i<nums.size();++i)
{
    if(nums[0]+nums[i]==target)
    {
        // 这里就有问题了，返回什么，不可能是0和i，所以递归是不可行的
        return std::vector<int>{};
        // 欲解决此问题，可以用一个辅助函数，在参数里多加一个层数offset
        // 那这里就可以这样写
        // return std::vector<int>{offset, offset+i};
    }
}
// 没有找到就进入下一层递归
return twoSum(std::vector<int>(nums.begin()+1, nums.end()), target);
```
故而想要实现递归，肯定就不能用twoSum进行递归了
使用twoSum_offset参数中加个偏移量，就可以知道递归的层数
```cpp
return twoSum_offset(nums, target, 0);
```
函数细节如下（类的私有成员函数）
```cpp
vector<int> twoSum_offset(vector<int> nums, int target, int offset)
{
    if(nums.empty())
    {
        return std::vector<int>();
    }
    for(int i=1;i<nums.size();++i)
    {
        if(nums[0]+nums[i]==target)
        {
            return std::vector<int>{offset, offset+i};
        }
    }
    return twoSum_offset(std::vector<int>(nums.begin()+1, nums.end()), target, offset+1);
}
```
不难发现，每次递归都会返回一个新创的数组，作为临时变量，那么参数里面就不能用引用
这样递归下来，每层一个数组，递归N次，每次一个N长度的数组，最后空间复杂度度O（N^2）
运行下来内存直接250，大部分都是15以下
所以参数用上引用，用offset去代替数组的更替
```cpp
std::vector<int> twoSum_offset(std::vector<int>& nums, int target, int offset)
{
    if(offset>=nums.size()-1)
    {
        return std::vector<int>();
    }
    for(int i=offset+1;i<nums.size();++i)
    {
        if(nums[offset]+nums[i]==target)
        {
            return std::vector<int>{offset, i};
        }
    }
    return twoSum_offset(nums, target, offset+1);
}
```
这么一改，不仅不用进行开辟内存的耗时操作，又节省了内存空间
内存压倒15以下，耗时缩短了一半

## 题解
### 暴力枚举
两层循环，与方法一相似
这么看来方法一写复杂了，官方这个简洁明了
```cpp
int n = nums.size();
// 既然while里面又++操作的应该直接用for
for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
        if (nums[i] + nums[j] == target) {
            // vector的返回也不用太复杂，直接返回大括号
            return {i, j};
        }
    }
}
return {};
```
时间复杂度O（N^2），n为数组元素个数，虽说里面一层的遍历数在逐渐减少，但还是在N这个级别，所以N^2
空间复杂度O（1），自然不用多说，几个索引

### 哈希表
两层循环之所以慢，主要是第二层需要target-x太慢了
如果能在第一层遍历的时候，第二层随之进行自动匹配
那么只需要一次遍历，就能既做到记录数组，又能看是否有满足条件的解
就相当于自己与以前的自己进行匹配
这样，时间复杂度度就能极限压缩，用空间换时间
```cpp
std::unordered_map<int, int> hash;
for(int i=0;i<nums.size();++i)
{
    // 用值作为索引的好处在这里体现
    auto it=hash.find(target-nums[i]);
    if(it!=hash.end())
    {
        return {it->second, i};
    }
    hash[nums[i]]=i;
}
return {};
```
时间复杂度O（N），只遍历了一次数组，unordered_map的底层实现是哈希表，其find的时间复杂度为O（1）
空间复杂度O（N），将数组的元素存入哈希表的开销

## 总结
这么看来unordered_map的潜力确实很高，追求高速度，用空间换时间，很适合查表
递归在这个问题里就显得有点吃力不讨好，一定要注意递归时的内存开销，最好用引用或指针
写的时候一定要注意代码的简洁与可读性，养成一个好素养