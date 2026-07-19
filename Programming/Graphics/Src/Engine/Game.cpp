#include "Game.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"
#include <GL/glew.h>
#include <algorithm>

Game::Game()
: mWindow(nullptr)
, mSpriteShader(nullptr)
, mIsRunning(true)
, mUpdatingActors(false)
{}

bool Game::Initialize(){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // 窗口创建前，请求OpenGL属性
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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
    mWindow=SDL_CreateWindow("TravelInSpace(2D)", 400, 100, 1024, 768, SDL_WINDOW_OPENGL);
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

    
    if(!LoadShaders())      // 编译着色器
    {
        SDL_Log("Failed to load shaders");
        return false;
    }

    CreateSpriteVerts();
    LoadData();
    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop(){
    while(mIsRunning){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame(){
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();
    for(auto sprite:mSprites){
        sprite->Draw(mSpriteShader);
    }

    SDL_GL_SwapWindow(mWindow);
}

void Game::Shutdown()
{
    UnloadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

bool Game::LoadShaders(){
    mSpriteShader= new Shader();
    if(!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")){
        return false;
    }
    mSpriteShader->SetActive();
    
    // 设置 2D 的 VP 矩阵
    Matrix4 viewProj=Matrix4::CreateSimpleViewProj(1024.f, 768.f);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

    return true;
}

void Game::CreateSpriteVerts()
{
    // x, y, z, u, v 三角形顶点坐标和对应的纹理坐标
	float vertices[] = {  
		-0.5f,  0.5f, 0.f, 0.f, 0.f,
		 0.5f,  0.5f, 0.f, 1.f, 0.f,
		 0.5f, -0.5f, 0.f, 1.f, 1.f,
		-0.5f, -0.5f, 0.f, 0.f, 1.f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData()
{
	mShip = new Ship(this);
	mShip->SetRotation(Math::PiOver2);

	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
}

Texture* Game::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if(iter != mSprites.end()) mSprites.erase(iter);
}