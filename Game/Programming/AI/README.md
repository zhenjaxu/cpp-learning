# 人工智能
## 状态机
### 基本实现
```cpp
enum AIState{
    Patrol,
    Death,
    Attack
};
```

```cpp
void AIComponent::Update(float deltaTime){
    switch(mState){
        case Patrol:
            UpdatePatrol(deltaTime);
            break;
        case Death:
            UpdateDeath(deltaTime);
            break;
        case Attack:
            UpdateAttack(deltaTime);
            break;
        default:
            break;
    }
}
```
```cpp
void AIComponent::ChangeState(AIState newState){
    // Exit current state
    // ...

    mState=newState;

    // Enter current state
    // ...
}
```
### 状态类
```cpp
class AIState{
public:
    AIState(class AIComponent* owner)
    :mOwner(owner)
    {}

    virtual void Update(float deltaTime)=0;
    virtual void OnEnter()=0;
    virtual void OnExit()=0;
    virtual const char* GetName() const =0;
protected:
    std::unordered_map<std::string, class AIState*> mStateMap;
    class AIState* mCurrentState;
};
```
```cpp
void AIComponent::RegisterState(AIState* state){
    mStateMap.emplace(state->GetName(), state);
}
```
```cpp
void AIComponent::Update(float deltaTime){
    if(mCurrentState){
        mCurrentState->Update(deltaTime);
    }
}
```
```cpp
void AIComponent::ChangeState(const std::string& name){
    if(mCurrentState){
        mCurrentState->OnExit();
    }
    auto iter=mStateMap.find(name);
    if(iter!=mStateMap.end()){
        mCurrentState=iter->second;
        mCurrentState->OnEnter();
    }else{
        SDL_Log("Could not find AIState %s in state map", name.c_str());
        mCurrentState=nullptr;
    }
}
```

```cpp
class AIPatrol:public AIState{
public:
    AIPatrol(class AIComponent* owner);
    void Update(float deltaTime) override;
    void OnEnter() override;
    void OnExit() override;
    const char* GetName() const override {return "Patrol";}
};
```
```cpp
void AIPatrol::Update(float deltaTime){
    // Do some other updating...

    bool dead=IsDead();
    if(dead){
        mOwner->ChangeState("Death");
    }
}
```
```cpp
auto a=new Actor(this);
auto aic=new AIComponent(a);
aic->RegisterState(new AIPatrol(aic));
aic->RegisterState(new AIDeath(aic));
aic->RegisterState(new AIAttack(aic));
aic->ChangeState("Patrol");
```
## 寻路
### 图形
```cpp
struct GraphNode{
    std::vector<GraphNode*> mAdjacent;
};

struct Graph{
    std::vector<GraphNode*> mNodes;
};
```

```cpp
struct WeightedEdge{
    struct WeightGraphNode* mFrom;
    struct WeightGraphNode* mTo;
    float mWeight;
};

struct WeightedGraphNode{
    std::vector<WeightEdge*> mEdges;
};
```
### 广度优先搜索
```cpp
using NodeToParentMap=
    std::unordered_map<const GraphNode*, const GraphNode*>;
```
```cpp
bool BFS(const Graph& graph, const GraphNode* start,
         const GraphNode* goal, NodeToParentMap& outMap)
{
    bool pathFound=false;
    std::queue<const GraphNode*> q;
    q.emplace(start);

    while(!q.empty()){
        const GraphNode* current=q.front();
        q.pop();
        if(current=goal){
            pathFound=true;
            break;
        }

        for(const GraphNode* node:current->mAdjacent){
            const GraphNode* parent=outMap[node];
            if(parent==nullptr&&node!=start){
                outMap[node]=current;
                q.emplace(node);
            }
        }

        return pathFound;
    }
}
```

```cpp
NodeToParentMap map;
bool found=BFS(g, g.mNodes[0], g.mNodes[9], map);
```
### heuristics函数