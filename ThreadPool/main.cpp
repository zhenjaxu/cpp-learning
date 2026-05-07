#include "ThreadPool.hpp"
#include <iostream>

int compute(int a, int b)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return a+b;
}

int main()
{
    ThreadPool pool(4);
    std::vector<std::thread> printers;
    std::mutex mut;
    for(int i=0;i<100;++i)
    {
        printers.emplace_back([i, &pool, &mut]{
            auto f=pool.submit(compute, i, i);
            while(!f.valid());
            std::lock_guard<std::mutex> lk(mut);
            std::cout << "f" << i << ": "
                      << f.get() << std::endl;
        });
    }
    for(int i=0;i<100;++i)
    {
        if(printers[i].joinable()) printers[i].join();
    }
    return 0; 
}