// 对于多线程并发，使用标准库容器的接口并不合适，其设计存在固有的条件竞争
// iterator本质是指向内部节点的指针，而map的接口几乎都有所涉及
// 一旦返回iterator，其就可以在锁外肆无忌惮
// 想要管理好迭代器，相对复杂，故而剔除，将其替换掉也不失为一种好方法
#include <functional>
#include <list>
#include <shared_mutex>
#include <mutex>
#include <memory>
template<typename Key, typename Value, typename Hash=std::hash<Key>>
class threadsafe_lookup_table
{
private:
    class bucket_type
    {
    private:
        typedef std::pair<Key, Value> bucket_value;
        typedef std::list<bucket_value> bucket_data;
        typedef typename bucket_data::iterator bucket_iterator;
        bucket_data data;
        mutable std::shared_mutex mutex;
        // 通过私有成员函数返回迭代器是安全的，只为寻找到对应数据
        bucket_iterator find_entry_for(Key const& key) const
        {
            return std::find_if(data.begin(),data.end(),
                                [&](bucket_value const& item)
                                {return item.first==key;});
        }
    public:
        Value value_for(Key const& key, Value const& default_value) const
        {
            std::shared_lock<std::shared_mutex> lock(mutex);    // 读操作用sharedlock，提高并发程度
            bucket_iterator const found_entry=find_entry_for(key);
            return (found_entry==data.end())?
                default_value:found_entry->second;
        }
        void add_or_update_mapping(Key const& key, Value const& value)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);    // 改写操作用uniquelock，保证并发安全
            bucket_iterator const found_entry=find_entry_for(key);
            if(found_entry==data.end())
            {
                data.push_back(bucket_value(key,value));
            }
            else
            {
                found_entry->second=value;
            }
        }
        void remove_mapping(Key const& key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);    // 删操作同样用uniquelock
            bucket_iterator const found_entry=find_entry_for(key);
            if(found_entry!=data.end())
            {
                data.erase(found_entry);
            }
        }
    };
    std::vector<std::unique_ptr<bucket_type>> buckets;
    Hash hasher;
    bucket_type& get_bucket(Key const& key) const
    {
        // Key作为模板参数，哈希后可能非常大，通过取模，在保证索引不越界的同时，也能给key进行分配一个指定的桶。
        std::size_t const bucket_index=hasher(key)%buckets.size();
        return *buckets[bucket_index];
    }
public:
    typedef Key key_type;
    typedef Value mapped_value;
    typedef Hash hash_type;
    threadsafe_lookup_table(
        unsigned num_buckets=19, Hash const& hasher_=Hash()):
        buckets(num_buckets),hasher(hasher_)
    {
        for(unsigned i=0;i<num_buckets;++i)
        {
            buckets[i].reset(new bucket_type);
        }
    }
    threadsafe_lookup_table(threadsafe_lookup_table const& other)=delete;
    threadsafe_lookup_table& operator=(
        threadsafe_lookup_table const& other)=delete;
    // 因为bucket的数量是固定的，故而get_bucket不需加锁，多线程可以并发运行，尽可能提高并发程度。
    // 有由于每个通都有自己的锁，故而不同的桶可以并发改写，同样能够实现高并发。
    Value value_for(Key const& key,
                    Value const& default_value=Value()) const
    {
        return get_bucket(key).value_for(key,default_value);
    }
    void add_or_update_mapping(Key const& key,Value const& value)
    {
        get_bucket(key).add_or_update_mapping(key,value);
    }
    void remove_mapping(Key const& key)
    {
        get_bucket(key).remove_mapping(key);
    }
};

// 异常安全：
// value_for不进行任何改动，即使抛出异常，也不会影响到数据结构。
// remove_mapping通过erase（不抛出异常）调用改动链表，故而异常安全。
// add_or_update_mapping含if语句，两个分支都会抛出异常。
// if条件成立运行push_back，是异常安全操作，即便抛出异常，链表也会保持原样。
// if条件不成立，会用新值替换原值，此过程抛出异常就只能期望原值没有发生改动。
// 但整个数据结构并未受到影响，而且值的类型由查找表的使用者提供，所以我们大可将异常留待使用者善后，这是安全的处理方式。