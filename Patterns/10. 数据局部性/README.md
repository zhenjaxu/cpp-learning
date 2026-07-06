```cpp
for(int i=0;i<NUM_THINGS;++i){
    sleepFor500Cycles();        // 缓存不命中代价，CPU空转
    things[i].doStuff();
}
```
```cpp
class GameEntity{
public:
    GameEntity(AIComponent& ai,
               PhysicsComponent& physics,
               RenderComponent& render)
    : ai_(ai), 
      physics_(physics), 
      render_(render)
    {}

    AIComponent* ai(){return ai_;}
    PhysicsComponent* physics(){return physics_;}
    RenderComponent* render(){return render_;}

private:
    AIComponent* ai_;
    PhysicsComponent* physics_;
    RenderComponent* render_;
};
```
```cpp
class AIComponent{
public:
    void update(){
        // 处理并修改状态...
    }

private:
    // 目标，情绪，等等...
};

class PhysicsComponent{
public:
    void update(){
        // 处理并修改状态...
    }

private:
    // 刚体，速度，质量，等等...
};

class RenderComponent{
public:
    void render(){
        // 处理并修改状态...
    }

private:
    // 网格，纹理，着色器，等等...
};
```
```cpp
while(!gameOver){
    // 处理AI
    for(int i=0;i<numEntities;++i){
        entities[i]->ai()->update();
    }

    // 更新物理
    for(int i=0;i<numEntities;++i){
        entities[i]->physics()->update();
    }

    // 绘制屏幕
    for(int i=0;i<numEntities;++i){
        entities[i]->render()->render();
    }

    // 其他代码...
}
```
```cpp
AIComponent* aiComponents=
    new AIComponent[MAX_ENTITIES];
PhysicsComponent* physicsComponents=
    new PhysicsComponent[MAX_ENTITIES];
RenderComponent* renderComponent=
    new RenderComponent[MAX_ENTITIES];
```
```cpp
while(!gameOver){
    // 处理AI
    for(int i=0;i<numEntities;++i){
        aiComponents[i].update();
    }

    // 更新物理
    for(int i=0;i<numEntities;++i){
        physicsComponents[i].update();
    }

    // 绘制屏幕
    for(int i=0;i<numEntities;++i){
        renderComponents[i].update();
    }

    // 其他和时间有关的游戏循环机制...
}
```
```cpp
class Particle{
public:
    void update(){
        // 重力，等等...
    }

    // 位置，速度，等等...
};

class ParticleSystem{
public:
    ParticleSystem()
    : numParticles_(0)
    {}

    void update();

private:
    static const int MAX_PARTICLES=100000;

    int numParticles_;
    Particle particles_[MAX_PARTICLES];
};
```
```cpp
void ParticleSystem::update(){
    for(int i=0;i<numParticles_;++i){
        if(paritcles_[i].isActive()){
            particles_[i].update();
        }
    }
}
```
```cpp
void ParticleSystem::update(){
    for(int i=0;i<numActive_;++i){
        particles_[i].update();
    }
}
```
```cpp
void ParticleSystem::activateParticle(int index){
    // 不应该已被激活
    assert(index>=numActive_);

    Particle temp=particle_[numActive_];
    particles_[numActive_]=particles_[index];
    particles_[index]=temp;

    numActive_++;
}
```
```cpp
void ParticleSystem::deactivateParticle(int index){
    // 不应该没被激活
    assert(index<numAcitve_);

    numActive_--;

    Particle temp=particles_[numActive_];
    particles_[numAcitve_]=particles_[index];
    particles_[index]=temp;
}
```
```cpp
class AIComponent{
public:
    void update(){
        // 处理和更新...
    }

private:
    Animation* animation_;
    double energy_;
    Vector goalPos_;

    LootType drop_;
    int minDrops_;
    int maxDrops_;
    double chanceOfDrop_;
};
```
```cpp
class AIComponent{
public:
    void update(){
        // 处理和更新...
    }

private:
    Animation* animation_;
    double energy_;
    Vector goalPos_;

    LootDrop* loot_;
};
```
```cpp
class LootDrop{
    friend class AIComponent;
    LootType drop_;
    int minDrops_;
    int maxDrops_;
    double chanceOfDrop_;
};
```