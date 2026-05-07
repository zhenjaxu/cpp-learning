#pragma once
#include <mutex>
#include <memory>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
private:
    struct node 
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::unique_ptr<node> head;
    std::mutex head_mutex;
    node* tail;
    std::mutex tail_mutex;
    std::condition_variable data_cond;

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::unique_ptr<node> old_head(std::move(head));
        head = std::move(old_head->next);
        return old_head;
    }

    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [this]{ return head.get() != get_tail(); });
        return head_lock;
    }

    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        // value 的赋值在 pop_head 之前，这很关键
        // 如果赋值抛出异常，但 pop_head 已经发生，那原来的数据就会丢失
        // 但如果在之前就抛出异常，则此时的数据还没有发生改变，故异常安全
        value = std::move(*head->data);
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return std::unique_ptr<node>();
        }
        return pop_head();
    }

    std::unique_ptr<node> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return std::unique_ptr<node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }

public:
    threadsafe_queue()
        : head(new node), tail(head.get())
    {}

    threadsafe_queue(const threadsafe_queue& other) = delete;
    threadsafe_queue& operator=(const threadsafe_queue& other) = delete;

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value))
        );
        std::unique_ptr<node> p(new node);
        node* const new_tail = p.get();
        {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            tail->next = std::move(p);
            tail = new_tail;
        }
        // 锁外唤醒，提高效率
        // 在该线程准备唤醒的时候，另一个线程已经拿到锁
        // 如果锁内唤醒，再解锁，此时另一个线程已经被唤醒，但锁还在原线程上，故会进入等待
        data_cond.notify_one();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_ptr<node> old_head(wait_pop_head());
        return old_head->data;
    }

    void wait_and_pop(T& value)
    {
        std::unique_ptr<node> old_head(wait_pop_head(value));
    }

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head(try_pop_head());
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    bool try_pop(T& value)
    {
        std::unique_ptr<node> old_head(try_pop_head(value));
        return old_head;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};


