template<typename T>
class threadsafe_queue
{
    private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;
    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);

        if(head.get()==get_tail())
        {
            return nullptr;
        }
        std::unqiue_ptr<node> old_head=std::move(head);
        head=std::move(old_head->next);
        return old_head;
    }
    public:
    threadsafe_queue():
        head(new node),tail(head.get())
    {}
    threadsafe_queue(const threadsafe_queue& other)=delete;
    threadsafe_queue& operator=(const threadsafe_queue& other)=delete;
    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head(pop_head());
        return old_head?old_head->data:std::shared_ptr<T>();
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value))
        );
        std::unique_ptr<node> p(new node);
        node* const new_tail=p.get();
        // 尽可能让资源准备工作在持锁外，提高并发程度
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data=new_data;
        tail->next=std::move(p);
        tail=new_tail;
    }
};

// 不同线程的解锁和加锁，能让受保护数据在线程间同步
// 如何判断是一个好的并发数据结构
// 1.在多线程并发调用成员函数时是否能保证不变量始终成立
// 不变量，即数据结构，比如gettail必须要在获取headmutex之后进行才能保证head不会外移，从而破坏结构
// 2.出现异常时，内存能否相机释放 
// 比如，push在前面构建两个实例的时候抛出异常，这些内存会因智能指针而自动释放
// 这两个成员函数都是异常安全，那并发程度呢
// 相比之前的一把大锁，这个平发程度就非常高了，因为我们没有直接用queue的内部函数实现
// 而是理解并将更精细的锁合理地融入结构当中，从而实现更高的并发程度
// 让尽可能多的不需要被锁保护的操作在多线程并发执行，比如内存分配、实例构造与析构等等
// 这段代码中有一个非常巧妙的地方，pophead返回oldhead实例让其在持锁之外析构，大大提高并发程度
// 而让需要锁保护的操作尽可能简单，比如指针赋值等等
// 不难发现，这段代码中持锁里的操作都非常简单，且异常安全
// 还要保证成员函数持尽可能少的锁，以避免死锁的发生
// 其中还有一个有趣的点，就是对于uniqueptr的空实例，返回的是nullptr
// 而对于sharedptr的空实例，返回的则是sharedptr，一个空的管理对象
// 这跟uniqueptr与sharedptr的内部结构有关，因为他们的内存占用不一样
// 那么，我们只是完成了trypop和push的编写，但queue并不止这些成员函数，还有waitandpop，这个又涉及等待和唤醒操作，又该如何实现呢
