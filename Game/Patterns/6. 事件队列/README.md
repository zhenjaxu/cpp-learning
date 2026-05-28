# 事件队列
## 目的
解耦发出消息或事件的时间和处理它们的时间。
## 问题
例如一个“声音引擎”的实现可能是这样的。
```cpp
class Audio{
public:
    static void playSound(SoundId id, int volume);
};
```
```cpp
void Audio::playSound(SoundId id, int volume){
    ResourceId resource=loadSound(id);
    int channel=findOpenChannel();
    if(channel==-1) return;
    startSound(resource, channel, volume);
}
```
在菜单上使用声音引擎。
```cpp
class Menu{
public:
    // 打开菜单时，“扑通”一下...
    void onSelect(int index){
        Audio::playSound(SOUND_BLOOP, VOL_MAX);
        // 其他代码...
    }
};
```
于是，出现了以下三个问题：
1. API先进行对声音请求的处理，**堵塞**调用者。假设声音处理了有一会，那菜单界面就会卡住。
2. 请求无法**合并**处理。比如，同一时间对两个相同声音进行处理，声音会变成两倍。
3. 请求在要求该线程**立即**执行。
## 模式
需要将**接收**请求和**处理**请求解耦。
### 推迟
将处理改为请求，**推迟**处理时间。此时，需要一个**存储**容器来存放这些请求，**队列**就是不错的选择。不过这里的队列是由**数组**和**指针**实现的**环形**缓存，性能更优。
```cpp
struct PlayMessage{
    SoundId id,
    int volume;
};
```
```cpp
class Audio{
public:
    static void init(){
        head_=0;
        tail_=0;
    }
    static void playSound(SoundId id, int volume);        // 请求
    static void update();                                   // 处理

private:
    static const int MAX_PENDING=16;
    
    static PlayMessage pending_[MAX_PENDING];
    static int head_;
    static int tail_;
};
```
```cpp
void Audio::playSound(SoundId id, int volume){
    assert((tail_+1)%MAX_PENDING!=head_);

    pending_[tail_].id=id;
    pending_[tail_].volume=volume;

    tail_=(tail_+1)%MAX_PENDING;
}
void Audio::update(){
    if(tail_==head_) return;

    ResourceId resource=loadSound(pending_[head_].id);
    int channel=findOpenChannel();
    if(channel==-1) return;
    startSound(resource, channel, pending_[head_].volume);

    head_=(head_+1)%MAX_PENDING;
}
```
### 合并
**合并**同一音频的请求，解决音量叠加的问题。在申请的时候，查找相同请求并合并。当然，也可以使用哈希表来降低查找的时间复杂度。
```cpp
void Audio::playSound(SoundId id, Volume volume){
    // 合并请求
    for(int i=head_;i!=tail_;
        i=(i+1)%MAX_PENDING){
        if(id==pending_[i].id){
            // 选择最大的声音
            pending_[i].volum=
                std::max(pending_[i].volume, volume);       
            // 无需入队
            return;     
        }
    }

    assert((tail_+1)%MAX_PENDING!=head_);

    pending_[tail_].id=id;
    pending_[tail_].volume=volume;

    tail_=(tail_+1)%MAX_PENDING;
}
```
### 多线程
使用**互斥锁**保证队列（临界区）不被同时修改，update可使用**条件变量**等待请求。
```cpp
class Audio{
public:
    static void init(){
        head_=0;
        tail_=0;
    }
    static void playSound(SoundId id, Volume volume);
    static void update();

private:
    static const int MAX_PENDING=16;
    static int head_;
    static int tail_;
    mutable std::mutex mutex;       // 互斥量
    std::condition_variable cv;    // 条件变量    
};
```
```cpp
void Audio::playSound(SoundId id, Volume volume){
    {
        std::lock_guard<std::mutex> lock(mutex);
        for(int i=head_;i!=tail_;
            i=(i+1)%MAX_PENDING){
            if(id==pending_[i].id){
                pending_[i].volume=
                    std::max(pending_[i].volume, volume);
                return;
            }
        }
        assert((tail_+1)%MAX_PENDING!=head_);
        pending_[tail_].id=id;
        pending_[tail_].volume=volume;
        tail_=(tail_+1)%MAX_PENDING;
    }
    cv.notify_one();            // 唤醒
}
void Audio::update(){
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [&this]{return tail_!=head_;});       // 等待
    int channel=findOpenChannel();
    if(channel==-1) return;
    ResourceId resource=loadSound(pending_[head_].id);
    startSound(resource, channel, pending_[head_].volume);
    head_=(head_+1)%MAX_PENDING;
}
```
## 决策
### 队列元素
若队列中存储**事件**或**通知**，传递**已经**发生的事件，需要其他对象作出**回应**。此时，队列一般全局可见，允许**多个**监听者。
若队列中存储**消息**或**请求**，传递**想要**发生在**未来**的事件。此时，队列一般只有**一个**监听者，提供**异步**服务。
### 队列类型
**单播队列**无需担心监听者之间的竞争，队列变成读取者的实现细节；**广播队列**需要过滤事件，从监听者中筛选出合适的，而且事件也可能因没有监听者在意而消失；**工作队列**会将每个事件投给监听者其中的一个，比如将工作打包给线程池。