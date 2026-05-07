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
        // 儘可能讓資源準備工作在持鎖外，提高併發程度
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data=new_data;
        tail->next=std::move(p);
        tail=new_tail;
    }
};

// 不同線程的解鎖和加鎖，能讓受保護數據在線程間同步
// 如何判斷是一個好的併發數據結構
// 1.在多線程併發調用成員函數時是否能保證不變量始終成立
// 不變量，即數據結構，比如gettail必須要在獲取headmutex之後進行才能保證head不會外移，從而破壞結構
// 2.出現異常時，內存能否相機釋放 
// 比如，push在前面構建兩個實例的時候拋出異常，這些內存會因智能指針而自動釋放
// 這兩個成員函數都是異常安全，那併發程度呢
// 相比之前的一把大鎖，這個平發程度就非常高了，因爲我們沒有直接用queue的內部函數實現
// 而是理解並將更精細的鎖合理地融入結構當中，從而實現更高的併發程度
// 讓儘可能多的不需要被鎖保護的操作在多線程併發執行，比如內存分配、實例構造與析構等等
// 這段代碼中有一個非常巧妙的地方，pophead返回oldhead實例讓其在持鎖之外析構，大大提高併發程度
// 而讓需要鎖保護的操作儘可能簡單，比如指針賦值等等
// 不難發現，這段代碼中持鎖裏的操作都非常簡單，且異常安全
// 還要保證成員函數持儘可能少的鎖，以避免死鎖的發生
// 其中還有一個有趣的點，就是對於uniqueptr的空實例，返回的是nullptr
// 而對於sharedptr的空實例，返回的則是sharedptr，一個空的管理對象
// 這跟uniqueptr與sharedptr的內部結構有關，因爲他們的內存佔用不一樣
// 那麼，我們只是完成了trypop和push的編寫，但queue並不止這些成員函數，還有waitandpop，這個又涉及等待和喚醒操作，又該如何實現呢
