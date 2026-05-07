# 线程池

## 线程池是什么
线程池通过管理一定数量的线程（worker），来充当消费者，执行生产者生产的任务

## 线程池的结构
### 线程池，管理消费者
用vector，线程池支持任意数量，可以扩容
```cpp
std::vector<std::thread> workers;
```
### 任务队列，获取任务
这里就很巧妙了，使用void()的函数去执行封装好的任意传参任意返回的packagedtask
既兼容各种函数模板
```cpp
std::queue<std::function<void()>> tasks;
```
又不影响构造函数的实现，妙哉
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
packagedtask不可拷贝，故而使用sharedptr指针，十分巧妙
```cpp
tasks.emplace([task_ptr]{(*task_ptr)();});
```
如是这样，问题大了，task析构后直接完蛋
```cpp
tasks.emplace([&task]{task();});
```
如是这样，taskptr析构后，引用计数为0，故而同样完蛋
```cpp
tasks.emplace([&task_ptr]{(*task_ptr)();});
```
只有sharedptr拷贝之后，哪怕原本的ptr析构了，引用计数还在，任务就还在
```cpp
tasks.emplace([=task_ptr]{(*task_ptr)();});
```
### 一把大锁，保护tasks和stop
并发程度会低一些
```cpp
std::mutex mut;
```
### 条件变量
```cpp
std::condition_variable cv;
```
在submit（获取任务）后，唤醒一个线程消费
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
在stop后，唤醒所有消费者，赶紧完工下班
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
这里一定要等所有worker全部联结，否则程序会终止
## 功能添加
### 线程池线程数量限制
欲实现限制功能，肯定是从构造函数出发
比如说这个线程池的最大线程数量为10，如果超过10我就抛出个异常或者断言
```cpp
#include <assert.h>
size_t maxNum=10;
assert(num<=maxNum);
```
然后测试一下
```cpp
ThreadPool pool(10);
```
为10的时候可以正常运行，且速度比4线程的更快
```cpp
ThreadPool pool(11);
```
为11的时候触发断言
```shell
test: ThreadPool.hpp:23: ThreadPool::ThreadPool(size_t): Assertion `num<=maxNum' failed.
Aborted (core dumped)
```
这样应该算成功了吧
嘿嘿，当然不是
assert在Release模式下会自动消失，比如加-02优化或加-DNDEBUG，就等于没有限制了
故而尝试把断言换成抛出异常，表示参数不合理
```cpp
if(num>maxNum)
{
    throw std::invalid_argument("thread number exceeds maximum limit");
}
```
运行后结果如下
```shell
terminate called after throwing an instance of 'std::invalid_argument'
  what():  thread number exceeds maximum limit
Aborted (core dumped)
```        
或则更简单的方法就是，直接不报错，直接用最大限制，暗渡陈仓
```cpp
if(num>maxNum)
{
    num=maxNum;
}
```
## 编译运行
使用g++编译运行
```shell
g++ -std=c++11 -pthread main.cpp -o test
./test
```