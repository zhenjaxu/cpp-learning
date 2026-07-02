#include <atomic>
#include <thread>
#include <assert.h>

int x;
std::atomic<bool> y;
std::atomic<int> z;
void write_x_then_y()
{
    x=true;
    // 非原子类型会在同步线程读取它时，将缓存传递过去，与原子类型的立即同步不同。但依旧能保证断言不会触发
    std::atomic_thread_fence(std::memory_order_release); 
    y.store(true, std::memory_order_relaxed);   
}
void read_y_then_x()
{
    while(!y.load(std::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order_acquire); 
    if(x)
        ++z;
}
int main()
{
    x=false;
    y=false;
    z=0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load()!=0);
}