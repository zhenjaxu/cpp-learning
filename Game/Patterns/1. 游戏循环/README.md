# 游戏循环
## 意图
将游戏的进行与玩家输入解耦，与处理器速度解耦。
## 事件循环
不同于大多数软件**堵塞**等待用户输入事件，真实的游戏循环是：**它处理用户输入，但不等待**。
```cpp
// 等待输入
while(true){
    Event* event=waitForEvent();
    dispatchEvent(event);
}
```
```cpp
// 游戏循环：处理输入，但不等待
while(true){
    processInput();     // 上次调用到现在的任何输入
    update();           // 运行物理和AI
    render();           // 渲染
}
```
## 时间模式
游戏循环运行的速度对应现实的**帧率**。游戏循环越快，帧率越高。其取决于**每帧要处理的工作量**（游戏内容），以及**底层平台的速度**（硬件水平）。开发者难以要求用户会在固定硬件条件下运行游戏，所以游戏循环的另一个关键任务：**不管潜在的硬件条件，以固定速度运行游戏**。
### 尽可能快地运行
你无法控制游戏运行得有多快。在高性能机器上，循环太快，玩家都看不清发生了什么；在低性能机器上，游戏慢得跟爬一样。如果游戏内容太多，游戏也会慢一些。而且机器会不停地处理，耗电和发热难以想象。
```cpp
while(true){
    processInput();
    update();
    render();
}
```
### 固定时间步长
游戏以固定帧数运行，当处理完当前一帧的游戏内容后**等待**休息。MS_PER_FRAME为帧间隔时间。sleep可以保证游戏不会运行太**快**，可运行太**慢**就无济于事了，休眠时间为**负**。
```cpp
while(true){
    double start=getCurrentTime();
    processInput();
    update();
    render();
    
    sleep(MS_PER_FRAME-(getCurrentTime()-start));       // 处理完后休眠
}
```
### 动态时间步长
通过**动态**调整更新频率，使游戏与时间同步。基于上帧到现在过去的**真实时间**（elapsed）来决定**游戏时间的跨度**（交由update实现）。游戏终于在不同的硬件上以**固定速度**运行了，高端机的游戏体验更**流畅**了。但悲剧的是，游戏变得**不确定**和**不稳定**，比如浮点数的舍入误差在高端机上放大数十倍。**同样**的子弹最终在高端和低端机器上的**最终位置不同**。
```cpp
double lastTime=getCurrentTime();
while(true){
    double current=getCurrentTime();
    double elapsed=current-last;        // 真实时间跨度
    last=current;

    processInput();
    update(elapsed);                    // 游戏时间跨度
    render();
}
```
### 固定更新时间步长
通过固定**更新的频率**来解决高端机与低端机**更新频率不同**带来的**不稳定**。同时，将**渲染拉出更新循环**，释放一大块CPU时间。MS_PER_UPDATE为**更新**时间间隔，不是与**视觉上**的帧率不同。间隔越短，更新次数越多；间隔越长，游戏抖动越厉害。其确定更为**复杂**，需尽可能地小来适应高端机，同时不至于低端机太慢（更新不过来）。
```cpp
double previous=getCurrentTime();
double lag=0.0;         // 真实时间累积量
while(true){
    double current=getCurrentTime();
    double elapsed=current-previous;
    previous=current;
    lag+=elapsed;

    processInput();
    // 追赶玩家的时间
    while(lag>=MS_PER_UPDATE){
        update();       // 单位游戏时间跨度一致
        lag-=MS_PER_UPDATE;
    }
    render();       // 将渲染拉出更新循环
}
```
## 动态渲染
如果渲染发生在两次更新之间，我们期待子弹出现在屏幕中间，而现在的实现是它在左边（前面一次更新的位置）。很巧的是，两次更新的时间间隔存储在**lag**中。通过其与MS_PER_UPDATE的比值来确定大致渲染的**提前量**。这种修正使图形运行更加**平滑**，虽有偏差，但通常感知不到。
```cpp
render(lag/MS_PER_UPDATE);      // 平滑渲染
```