# 線程池

## 線程池是什麼
線程池通過管理一定數量的線程（worker），來充當消費者，執行生產者生產的任務

## 線程池的結構
### 線程池，管理消費者
std::vector<std::thread> workers;
### 任務隊列，獲取任務
這裏就很巧妙了，使用void()的函數去執行封裝好的任意傳參任意返回的packagetask
既兼容各種函數模板
std::queue<std::function<void()>> tasks;
又不影響構造函數的實現，妙哉
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
packagetask不可拷貝，故而使用sharedptr指針，十分巧妙
tasks.emplace([task_ptr]{(*task_ptr)();});
如是這樣，問題大了，task析構後直接完蛋
tasks.emplace([&task]{task();});
如是這樣，taskptr析構後，引用計數爲0，故而同樣完蛋
tasks.emplace([&task_ptr]{(*task_ptr)();});
只有sharedptr拷貝之後，哪怕原本的ptr析構了，引用計數還在，任務就還在
tasks.emplace([=task_ptr]{(*task_ptr)();});
### 一把大鎖，保護tasks和stop
併發程度會低一些
std::mutex mut;
### 條件變量
std::condition_variable cv;
在submit（獲取任務）後，喚醒一個線程消費
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
在stop後，喚醒所有消費者，趕緊完工下班
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
這裏一定要等所有worker全部聯結，否則程序會終止