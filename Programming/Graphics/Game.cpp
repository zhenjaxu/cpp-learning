#include"Game.h"
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

    // 创建OpenGL窗口
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

    
}

void Game::RunLoop(){}

void Game::Shutdown(){}

void Game::InitSpriteVerts(){
    mSpriteVerts=new VertexArray(vertexBuffer, 4, indexBuffer, 6);      // 正方形，2三角形，4顶点，6索引
}