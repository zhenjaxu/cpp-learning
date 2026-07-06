# 服务定位器
## 目的
提供服务的全局接入点，避免使用者和提供服务的具体类耦合。
## 问题
对于整个游戏都需要的**服务**，例如音频系统，它可能出现在游戏的各个板块（如物理、AI、用户界面）。全局可通过静态类或单例调用音频系统。服务的**实现**与**提供**服务的类产生耦合，不仅**暴露**接口，而且一旦**更改**提供服务的类，**所有**实现都需要更改。
```cpp
// 使用静态类？
AudioSystem::playSound(VERY_LOUD_BANG);
// 使用单例？
AudioSystem::instance()->playSound(VERY_LOUD_BANG);
```
```cpp
class Audio{
public:
    virtual ~Audio(){}
    virtual void playSound(int soundId)=0;
    virtual void stopSound(int soundId)=0;
    virtual void stopAllSound()=0;
};
```
```cpp
class ConsoleAudio:public Audio{
public:
    virtual void playSound(int soundId){
        // 使用主机音频API播放声音...
    }

    virtual void stopSound(int soundId){
        // 使用主机音频API停止声音...
    }

    virtual void stopAllSound(){
        // 使用主机音频API停止所有声音...
    }
};
```
## 模式
通过调用一个**固定**的服务定位器里的**静态**函数，定位到提供服务的类。即使更改服务，只需调用**一次**静态函数provide更新服务即可。
```cpp
class Locator{
public:
    static Audio* getAudio(){return service_;}

    static void provide(Audio* service){
        service_=service;
    }

private:
    static Audio* service_;
};
```
```cpp
Audio* audio=new Locator::getAudio();   // 定位服务
audio->playSound(VERY_LOUD_BANG);
```
```cpp
ConsoleAudio* audio=new ConsoleAudio();
Locator::provide(audio);    // 更改服务
```
需对返回的值进行**检查**，查看是否为空。如若不然，游戏就崩溃了。也可通过返回**空服务**，保证对象始终有效，调用不会使游戏崩溃。
```cpp
class NullAudio:public Audio{
public:
    // 什么也不干
    virtual void playSound(int soundId){}
    virtual void stopSound(int soundId){}
    virtual void stopAllSound(){}
};
```
```cpp
class Locator{
public:
    static void initialize(){service_=&nullService_;}

    static Audio& getAudio(){return *service_;}

    static void provide(Audio* service){
        if(service==nullptr) service_=&nullService_;
        else service_=service;
    }

private:
    static Audio* service_;
    static NullAudio nullService_;      // 空服务对象
};
```
## 日志装饰器
将原本的音频服务**升级**为有日志的音频服务，而服务定位器启动它十分容易。
```cpp
class LoggedAudio:public Audio{
public:
    LoggedAudio(Audio& wrapped)
    : wrapped_(wrapped)
    {}

    virtual void playSound(int soundId){
        log("play sound");
        wrapped_.playSound(soundId);
    }

    virtual void stopSound(int soundId){
        log("stop sound");
        wrapped_.stopSound(soundId);
    }

    virtual void stopAllSound(){
        log("stop all sound");
        wrapped_.stopAllSound();
    }

private:
    void log(const char* message){
        // 记录日志...
    }

    Audio* wrapped_;
};
```
```cpp
void enableAudioLogging(){
    Audio* service=new LoggedAudio(Locator::getAudio());      // 升级
    Locator::provide(service);                                  // 启动服务
}
```
```cpp
class Locator{
public:
    static Audio& getAudio(){
        Audio* service=nullptr;

        // 配置服务...

        assert(service!=nullptr);       // 为空则挂起
        return *service;
    }
};
```