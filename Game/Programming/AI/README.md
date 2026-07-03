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
### 贪婪最佳优先搜索
```cpp
struct GBFSScratch{
    const WeightEdge* mParentEdge=nullptr;
    float mHeuristic=0.0f;
    bool mInOpenSet=false;
    bool mInCloseSet=false;
};
```

```cpp
using GBFSMap=
    std::unordered_map<const WeightGraphNode*, GBFSScratch>;
```

```cpp
bool BGFS(const WeightedGraph& g, const WeightedGraphNode* start,
          const WeightedGraphNode* goal, GBFSMap& outMap)
{
    std::vector<const WeightedGraphNode*> openSet;    // 存放即将搜索的节点
    const WeightedGraphNode* current=start;
    outMap[current].mInClosedSet=true;      // 标记为已经搜索过的节点，放入闭集

    do{
        for(const WeightedEdge* edge:current->mEdges){
            // 通过outMap获取该点的相关信息
            GBFSScratch& data=outMap[edge->mTo];
            if(!data.mInClosedSet){
                data.mParentEdge=edge;      // 保存路径
                if(!data.mInOpenSet){
                    data.mHeuristic=ComputeHeuristic(edge->mTo, goal);
                    data.mInOpenSet=true;
                    openSet.emplace_back(edge->mTo);
                }
            }
        }

        if(openSet.empty()){
            break;
        }

        // 搜索开集中启发量最小的节点作为下一个当前节点
        auto iter=std::min_element(openSet.begin(), openSet.end(),
            [&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b)
        {
            return outMap[a].mHeuristic<outMap[b].mHeuristic;
        });
        current=*iter;
        openSet.erase(iter);
        outMap[current].mInOpenSet=false;
        outMap[current].mInClosedSet=true;
    }while(current!=goal);

    return current==goal;
}
```
### A*搜索
```cpp
for(const WeightedEdge* edge:current->mEdges){
    const WightedGraphNode* neighbor=edge->mTo;
    AStarScratch& data=outMap[neighbor];
    if(!data.mInClosedSet){
        if(!data.mInOpenSet){
            data.mParentEdge=edge;
            data.mHeuristic=ComputeHeuristic(neighbor, goal);
            data.mActualFromStart=outMap[current].mActualFromStart+edge->mWeight;
            data.mInOpenSet=true;
            openSet.emplace_back(neighbor);
        }else{
            // 如果已经在开集当中，重新计算该点的路径成本使它最小
            float newG=outMap[current].mActualFromStart+edge->mWeight;
            // 更新路径和成本
            if(newG<data.mActualFromStart){
                data.mParentEdge=edge;
                data.mActualFromStart=newG;
            }
        }
    }
}
```
### Dijkstra算法
将Astar代码转换成迪杰斯特拉代码，只需删除启发值h，相当于h=0的Astar算法。其次，还需删除目标节点，确保搜索完全部节点。该算法能计算出起点到每个节点的最短路径。
## 跟随路径
```cpp
void NavComponent::TurnTo(const Vector2& pos){
    Vector2 dir=pos-mOwner->GetPosition();
    float angle=Math::Atan2(-dir.y, dir.x);
    mOwner->SetRotation(angle);
}

void NavComponent::Update(float deltaTime){
    Vector2 diff=mOwner->GetPosition()-mNextPoint;
    // 到达后转向下个节点
    if(diff.LengthSq()<=2.0f*2.0f){     // LengthSq避免平方根的昂贵代价
        mNextPoint=GetNextPoint();
        TurnTo(mNextPoint);
    }

    MoveComponent::Update(deltaTime);
}
```