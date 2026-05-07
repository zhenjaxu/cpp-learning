template<typename T>
class queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::unique_ptr<node> head;
    node* tail;
public:
    queue()
    :head(new node),tail(head.get())
    {}
    queue(const queue& other)=delete;
    queue& operator=(const queue& other)=delete;
    std::shared_ptr<T> try_pop()
    {
        if(head.get()==tail)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(head->data);
        std::unique_ptr<node> old_head=std::move(head);
        head=std::move(old_head->next);
        return res;
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared(std::move(new_value))
        );
        std::unique_ptr<node> p(new node);
        tail->data=new_data;
        node* const new_tail=p.get();
        tail->next=std::move(p);
        tail=new_tail;
    }
};

// head.get返回节点地址与tail比较
// 不难发现，在加入dummynode（tail所指的）后，tail不再指向尾节点，失去back的能力，以提高并发程度
// makeshared一次性完成内存和控制块的分配，比ptr（new T()）（两次内存分配）更快更安全，减少内存碎片化
// 那headmutex和tailmutex要在什么时候上锁解锁才能使并发程度最高?