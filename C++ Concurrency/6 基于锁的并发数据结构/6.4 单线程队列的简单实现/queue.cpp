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

// 這是一個簡單的隊列
// 用head和tail分別指向頭節點和尾節點
// head以及next指針均用unqiueptr，以實現資源的自動釋放
// tail則單獨用裸指針，因爲unqiueptr獨佔資源，只用於從尾部添加節點
// 該隊列在配置精細粒度的鎖時存在一些問題：
// 1.在push的時候頭節點與尾節點的指針均要上鎖
// 2.在同時push和trypop的時候，可能同時訪問同一節點的next（head==tail），就會嘗試鎖住同一互斥，這樣的話就不算真正的併發了
// 所以想要做到真正的併發還是有難度的，那麼該如何實現才能避免上述問題呢？