# 線程池

## 線程池是什麼
線程池通過管理一定數量的線程（worker），來充當消費者，執行生產者生產的任務

## 線程池的結構
### 線程池，管理消費者
用vector，線程池支持任意數量，可以擴容
```cpp
std::vector<std::thread> workers;
```
### 任務隊列，獲取任務
這裏就很巧妙了，使用void()的函數去執行封裝好的任意傳參任意返回的packagedtask
既兼容各種函數模板
```cpp
std::queue<std::function<void()>> tasks;
```
又不影響構造函數的實現，妙哉
```cpp
explicit ThreadPool(size_t num):stop(false)
{
    for(size_t i=0;i<num;++i)
    {
        workers.emplace_back([this]{
            while(true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lk(mut);
                    cv.wait(lk, [this]{return stop||!tasks.empty();});
                    if(stop&&tasks.empty()) return;
                    task=std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
}
```
packagedtask不可拷貝，故而使用sharedptr指針，十分巧妙
```cpp
tasks.emplace([task_ptr]{(*task_ptr)();});
```
如是這樣，問題大了，task析構後直接完蛋
```cpp
tasks.emplace([&task]{task();});
```
如是這樣，taskptr析構後，引用計數爲0，故而同樣完蛋
```cpp
tasks.emplace([&task_ptr]{(*task_ptr)();});
```
只有sharedptr拷貝之後，哪怕原本的ptr析構了，引用計數還在，任務就還在
```cpp
tasks.emplace([=task_ptr]{(*task_ptr)();});
```
### 一把大鎖，保護tasks和stop
併發程度會低一些
```cpp
std::mutex mut;
```
### 條件變量
```cpp
std::condition_variable cv;
```
在submit（獲取任務）後，喚醒一個線程消費
```cpp
template<typename F, typename... Args>
auto submit(F&& f, Args... args)
    ->std::future<decltype(f(args...))>
{
    using return_type=decltype(f(args...));
    auto task_ptr=std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f),std::forward<Args>(args)...)
    );
    std::future<return_type> result=task_ptr->get_future();
    {
        std::unique_lock<std::mutex> lk(mut);
        if(stop) throw std::runtime_error("thread pool stopped!");
        tasks.emplace([task_ptr]{(*task_ptr)();});
    }
    cv.notify_one();
    return result;
}
```
在stop後，喚醒所有消費者，趕緊完工下班
```cpp
~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lk(mut);
        stop=true;
    }
    cv.notify_all();
    for(auto& worker:workers)
    {
        if(worker.joinable())
        {
            worker.join();
        }
    }
}
```
這裏一定要等所有worker全部聯結，否則程序會終止
## 功能添加
### 線程池線程數量限制
慾實現限制功能，肯定是從構造函數出發
比如說這個線程池的最大線程數量爲10，如果超過10我就拋出個異常或者斷言
```cpp
#include <assert.h>
size_t maxNum=10;
assert(num<=maxNum);
```
然後測試一下
```cpp
ThreadPool pool(10);
```
爲10的時候可以正常運行，且速度比4線程的更快
```cpp
ThreadPool pool(11);
```
爲11的時候觸發斷言
```shell
test: ThreadPool.hpp:23: ThreadPool::ThreadPool(size_t): Assertion `num<=maxNum' failed.
Aborted (core dumped)
```
這樣應該算成功了吧
嘿嘿，當然不是
assert在Release模式下會自動消失，比如加-02優化或加-DNDEBUG，就等於沒有限制了
故而嘗試把斷言換成拋出異常，表示參數不合理
```cpp
if(num>maxNum)
{
    throw std::invalid_argument("thread number exceeds maximum limit");
}
```
運行後結果如下
```shell
terminate called after throwing an instance of 'std::invalid_argument'
  what():  thread number exceeds maximum limit
Aborted (core dumped)
```        
或則更簡單的方法就是，直接不報錯，直接用最大限制，暗渡陳倉
```cpp
if(num>maxNum)
{
    num=maxNum;
}
```
## 編譯運行
使用g++編譯運行
```shell
g++ -std=c++11 -pthread main.cpp -o test
./test
```