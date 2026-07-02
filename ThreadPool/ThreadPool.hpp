#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    bool stop;
    std::mutex mut;
    std::condition_variable cv;
    size_t maxNum=10;
public:
    explicit ThreadPool(size_t num):stop(false)
    {
        if(num>maxNum)
        {
            throw std::invalid_argument("thread number exceeds maximum limit");
            // num=maxNum;
        }
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
};