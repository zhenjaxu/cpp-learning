template<typename T>
class queue
{
private:
    struct node
    {
        T data;
        std::unique_ptr<node> next;
        node(T data_):
        data(std::move(data_))
        {}
    };
    std::unique_ptr<node> head;
    node* tail;
public:
    queue():tail(nullptr)
    {}
    queue(const queue& other)=delete;
    queue& operator=(const queue& other)=delete;
    std::shared_ptr<T> try_pop()
    {
        if(!head)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(head->data))
        );
        std::unique_ptr<node> const old_head=std::move(head);
        head=std::move(old_head->next);
        if(!head)
        {
            tail=nullptr;
        }
        return res;
    }
    void push(T new_value)
    {
        std::unique_ptr<node> p(new node(std::move(new_data)));
        node* const new_tail=p.get();
        if(tail)
        {
            tail->next=std::move(p);
        }
        else
        {
            head=std::move(p);
        }
        tail=new_tail;
    } 
};

// 这是一个简单的队列
// 用head和tail分别指向头节点和尾节点
// head以及next指针均用unqiueptr，以实现资源的自动释放
// tail则单独用裸指针，因为unqiueptr独占资源，只用于从尾部添加节点
// 该队列在配置精细粒度的锁时存在一些问题：
// 1.在push的时候头节点与尾节点的指针均要上锁
// 2.在同时push和trypop的时候，可能同时访问同一节点的next（head==tail），就会尝试锁住同一互斥，这样的话就不算真正的并发了
// 所以想要做到真正的并发还是有难度的，那么该如何实现才能避免上述问题呢？