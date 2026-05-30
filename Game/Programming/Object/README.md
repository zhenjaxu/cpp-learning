# 游戏对象与2D图形

```cpp
class Actor{
public:
    // Called every frame to update the Actor
    virtual void update(float deltaTime);
    // Called every frame to draw the Actor
    virtual void Draw(); 
};
```
```cpp
class PacMan:public Actor{
public:
    void Update(float deltaTime) override;
    void Draw() override;
};
```

```cpp
class GameObject{
public:
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
private:
    std::unordered_set<Component*> components_;
};
```

```cpp
class Actor{
public:
    // Used to track state of actor
    enum State{
        EActive,
        EPaused,
        EDead
    };
    // Constructor/destructor
    Actor(class Game* game);
    virtual ~Actor();

    // Update function called from Game (not overridable)
    void Update(float deltaTime);
    // Updates all the components attached to the actor (not overridable)
    void UpdateComponents(float deltaTime);
    // Any actor-specific update code (overridable)
    virtual void UpdateActor(float deltaTime);
    // Getters/setters
    // ...

    // Add/remove components
    void addComponent(class Component* component);
    void RemoveComponent(class Component* component);

private:
    // Actor's state
    State state_;
    // Transform
    Vector2 position_;      // Center position of actor
    float scale_;           // Uniforms scale of actor (1.0f for 100%)
    float rotation;         // Rotation angle (in radians)
    // Components held by this actor
    std::vector<class Component*> components_;
    class Game* game;
};
```

```cpp
class Component{
public:
    // Constructor
    // (the lower the update order, the earlier the component updates)
    Component(class Actor* owner, int updateOrder=100);
    // Destructor
    virtual ~Component();
    // Update this component by delta time
    virtual void Update(float deltaTime);
    int GetUpdateOrder() const { return updateOrder_; }

protected:
    // Owning actor
    class Actor* actor;
    // Update order of component
    int updateOrder_;
};
```

```cpp
void Game::AddActor(Actor* actor){
    // If updating actors, neet to add to pending
    if(mUpdatingActors){
        mPendingActors.emplace_back(actor);
    }
    else{
        mActors.emplace_back(actor);
    }
}
```

```cpp
void Game::UpdateGame(){
    // Compute delta time (as in Chapter 1)
    float deltaTime;

    mUpdatingActors=true;
    for(auto actor:mActors){
        actor->Update(deltaTime);
    }
    mUpdatingActors=false;

    // Move any pending actors to mActors
    for(auto pending:mPendingActors){
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // Add any dead actors to a temp vector
    std::vector<Actor*> deadActors;

    for(auto actor:mActors){
        if(actor->GetState==Actor::EDead){
            deadActors.emplace_back(actor);
        }
    }

    // Delete dead actors (which removes them from mActors)
    for(auto actor:deadActors){
        delete actor;
    }
}
```

```cpp
// Because ~Actor calls RemoveActor, use a different style loop
while(!mActors.empty()){
    delete mActors.back();
}
```

```cpp
IMG_Init(IMG_INIT_PNG);
```
```cpp
// Loads an image from a file
// Returns a pointer to a SDL_Surface if successful, otherwise nullptr
SDL_Surface* IMG_Load(
    const char* file        // Image file name
);
```
```cpp
// Converts an SDL_Surface to an SDL_Eexture
// Returns a pointer to an SDL_Texture if successful, otherwise nullptr
SDL_Texture* SDL_CreateTextureFromSurface(
    SDL_Renderer* renderer,     // Renderer used
    SDL_Surface* surface        // Surface to convert
);
```
```cpp
SDL_Texture* LoadTexture(const char* fileName){
    // Load from file
    SDL_Surface* surface=IMG_Load(fileName);
    if(!surface){
        SDL_Log("Faild to load texture file %s", fileName);
        return nullptr;
    }
    // Create texture from surface
    SDL_Texture* texture=SDL_CreateTextureFromSurface(mRenderer, surface);
    if(!texture){
        SDL_Log("Faild to convert surface to texture for %s", fileName);
        return nullptr;
    }
    return texture;
}
```

```cpp
class SpriteComponent:public Component{
public:
    // (Lower draw order corresponds with further back)
    SpriteComponent(class Actor* owner, int drawOrder=100);
    ~SpriteComponent();
    virtual void Draw(SDL_Renderer* renderer);
    virtual void SetTexture(SDL_Texture* texture);

    int GetDrawOrder() const {return mDrawOrder;}
    int GetTextureHeight() const {return mTextureHeight;}
    int GetTextureWidth() const {return mTextureWidth;}

protected:
    // Texture to draw
    SDL_Texture* mTexture;
    // Draw order used for painter's algorithm
    int mDrawOrder;
    // Width/height of texture
    int mTextureHeight;
    int mTextureWidth;
};
```

```cpp
void Game::AddSprite(SpriteComponent* sprite){
    // Find the insertion point in the sorted vector
    // (The first element with a higher draw order then me)
    int myDrawOrder=sprite->GetDrawOrder();
    auto iter=mSprites.begin();
    for(;iter!=mSprites.end();++iter){
        if(myDrawOrder<(*iter)->GetDrawOrder()){
            break;
        }
    }

    mSprites.insert(iter, sprite);
}
```

```cpp
void SpriteComponent::SetTexture(SDL_Texture* texture){
    mTexture=texture;
    // Get width/height of texture
    SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
}
```

```cpp
// Renders a texture to the rendering target
// Returns 0 on success, negative value on failure
int SDL_RenderCopy(
    SDL_Renderer* renderer,     // Render target to draw to
    SDL_Texture* texture,       // Texture to draw
    const SDL_Rect* srcrect,    // Part of texture to draw (null if whole)
    const SDL_Rect* dstrect     // Rectangle to draw onto the target
);
```
```cpp
int SDL_RenderCopyEx(
    SDL_Renderer* renderer,
    SDL_Texture* texture,
    const SDL_Rect* srcrect,
    const SDL_Rect* dstrect,
    double angle,                   // Rotation angle (in degrees, clockwise)
    const SDL_Point* center,        // Point to rotate about (nullptr for center)
    SDL_RenderFlip flip             // How to flip texture (usually SDL_FLIP_NONE)
);
```

```cpp
void SpriteComponent::Draw(SDL_Renderer* renderer){
    if(mTexture){
        SDL_Rect r;
        // Scale the width/height by owner's scale
        r.w=static_cast<int>(mTextureWidth*mOwner->GetScale());
        r.h=static_cast<int>(mTextureHeight*mOwner->GetScale());
        // Center the rectangle around th position of the owner
        r.x=static_cast<int>(mOwner->GetPosition().x-r.w/2);
        r.y=static_cast<int>(mOwner->GetPosition().y-r.h/2);

        // Draw
        SDL_RenderCopyEx(
            renderer,
            mTexture,
            nullptr,        // Source rectangle
            &r,             // Destination rectangle
            -Math::ToDegrees(mOwner->GetRotation()),    // (Convert angle)
            nullptr,        // Point of rotation
            SDL_FLIP_NONE   // Flip behavior
        );
    }
}
```

```cpp
class AnimSpriteComponent:public SpriteComponent{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder=100);
    // Update animation every frame (overriden from component)
    void Update(float deltaTime) override;
    // Set the textures used for animation
    void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
    // Set/get the animation FPS
    float GetAnimFPS() const {return mAnimFPSl;}
    void SetAnimFPS(float fps){mAnimFPS=fps;}

private:
    // All textures in the animation
    std::vector<SDL_Texture*> mAnimTextures;
    // Current frame displayed
    float mCurrFrame;
    // Animation frame rate
    float mAnimFPS;
};
```

```cpp
void AnimSpriteComponent::Update(float deltaTime){
    SpriteComponent::Update(deltaTime);

    if(mAnimTextures.size()>0){
        // Update the current frame based on frame rate
        // and delta time
        mCurrFrame+=mAnimFPS*deltaTime;

        // Wrap current fram if needed
        while(mCurrentFrame>=mAnimTextures.size()){
            mCurrFrame-=mAnimTextures.size();
        }

        // Set the current texture
        SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
    }
}
```

```cpp
class BGSpriteComponent:public SpriteComponent{
public:
    // Set draw order to default to lower (so it's in the background)
    BGSpriteComponent(class Actor* owner, int drawOrder=10);
    // Update/draw overriden from parent
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;
    // Set the textures used for the background
    void SetBGTextures(const std::vector<SDL_Texture*>& textures);
    // Get/set screen size and scroll speed
    void SetScreenSize(const Vector2& size){mScreenSize=size;}
    void SetScrollSpeed(float speed){mScrollSpeed=speed;}
    float GetScrollSpeed() const {return mScrollSpeed;}

private:
    // Struct to encapsulate each BG image and its offset
    struct BGTexture{
        SDL_Texture* mTexture;
        Vector2 mOffset;
    };
    std::vector<BGTexture> mBGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};
```

```cpp
void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures){
    int count=0;
    for(auto tex:textures){
        BGTexture temp;
        temp.mTexture=tex;
        // Each textures is screen width in offset
        temp.mOffset.x=count*mScreenSize.x;
        temp.mOffset.y=0;
        mBGTextures.emplace_back(temp);
        count++;
    }
}
```

```cpp
class Ship:public Actor{
public:
    Ship(class Game* game);
    void UpdateActor(float deltaTime) override;
    void ProcessKeyBoard(const uint8_t* state);
    float GetRightSpeed() const {return mRightSpeed;}
    float GetDownSpeed() const {return mDownSpeed;}

private:
    float mRightSpeed;
    float mDownSpeed;
};
```

```cpp
AnimSpriteComponent* asc=new AnimSpriteComponent(this);
std::vector<SDL_Texture*> anims={
    game->GetTexture("Asserts/Ship/Ship01.png"),
    game->GetTexture("Asserts/Ship/Ship02.png"),
    game->GetTexture("Asserts/Ship/Ship03.png"),
    game->GetTexture("Asserts/Ship/Ship04.png")
};
asc->SetAnimTextures(anims);
```

```cpp
void Ship::UpdateActor(float deltaTime){
    Actor::UpdateActor(deltaTime);
    // Update position based on speeds and delta time
    Vector2 pos=GetPosition();
    pos.x+=mRightSpeed*deltaTime;
    pos.y+=mDownSpeed*deltaTime;
    // Restrict position to left half of screen
    // ...
    SetPostion(pos);
}
```