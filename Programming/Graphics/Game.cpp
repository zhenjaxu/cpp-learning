#include"Game.h"
#include"Actor.h"
#include"VertexArray.h"

Game::Game(){}

bool Game::Initialize(){
    if(SDL_Init(SDL_INTI_VIDEO|SDL_INIT_AUDIO)){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // 窗口创建前，请求OpenGL属性
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    // 设置GL版本3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // 设置RGBA颜色深度
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // 请求双缓冲
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // 请求硬件(GPU)加速
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // 创建 OpenGL 窗口
    mWindow=SDL_CreatWindow("TravelInSpace(2D)", 400, 100, 1024, 768, SDL_WINDOW_OPENGL);
    if(!mWindow){
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // 创建窗口后，创建上下文
    mContext=SDL_GL_CreateContext(mWindow);

    // 初始化GLEW
    glewExperimental=GL_TRUE;
    if(glewInit()!=GLEW_OK){
        SDL_Log("Failed to initialize GLEW");
        return false;
    }

    // 清理无害错误
    glGetError();

    LoadShaders();      // 编译着色器


}

void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::UpdateGame(){
    if(SDL_TICKS_PASSED(mTicksCount+16, SDL_GetTicks()+2)){
        SDL_Delay(mTicksCount+16-SDL_GetTicks()-2);
    }

    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));
    float deltaTime=(SDL_GetTicks()-mTicksCount)/1000.0f;
    mTicksCount=SDL_GetTicks();
    if(deltaTime>0.05f) deltaTime=0.05f;

    mUpdatingActors=true;
    for(auto actor:mActors){
        actor->Update(deltaTime);
    }
    mUpdatingActors=false;

    for(auto pending:mPendingActors){
        pending->ComputeWorldTransform();       // 加入前，计算世界变换矩阵
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for(auto actor:mActors){
        if(actor->GetState()==Actor::EDead){
            deadActors.emplace_back(actor);
        }
    }

    for(auto actor:deadActors){
        delete actor;
    }
}

void Game::GenerateOutput(){
    glClearColor(0.86f, 0.86f, 0.86f, 1.0f);    // 灰色
    glClear(GL_COLOR_BUFFER_BIT);

    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();

    for(auto sprite:mSprites){
        sprite->Draw(mSpriteShader);
    }

    SDL_GL_SwapWindow(mWindow);
}

void Game::Shutdown(){}

void Game::InitSpriteVerts(){
    mSpriteVerts=new VertexArray(vertexBuffer, 4, indexBuffer, 6);      // 正方形，2三角形，4顶点，6索引
}

bool Game::LoadShaders(){
    mSpriteShader= new Shader();
    if(!mSpriteShader->Load("Shaders/Transform.vert", "Shaders/Basic.frag")){
        return false;
    }
    mSpriteShader->SetActive();

    // 设置 2D 的 VP 矩阵
    Matrix4 viewProj=Matrix4::CreateSimpleViewProj(1024.f, 768.f);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

    return true;
}