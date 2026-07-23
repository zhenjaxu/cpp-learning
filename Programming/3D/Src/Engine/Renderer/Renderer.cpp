#include "Renderer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include <GL/glew.h>
#include <algorithm>

Renderer::Renderer(Game* game)
:mGame(game)
,mSpriteShader(nullptr)
,mMeshShader(nullptr)
{}

Renderer::~Renderer()
{}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
    mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

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
	// 申请深度缓冲
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	

    // 请求双缓冲
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    // 请求硬件(GPU)加速
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // 创建 OpenGL 窗口
    mWindow=SDL_CreateWindow("Pong(3D)", 400, 100, 
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
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

    return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    mMeshShader->SetActive();
    mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
    SetLightUniforms(mMeshShader);
    for(auto mc : mMeshComps)
    {
        mc->Draw(mMeshShader);
    }

    glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();
    for(auto sprite:mSprites){
        sprite->Draw(mSpriteShader);
    }

    SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
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

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if(iter != mSprites.end()) mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	if(iter != mMeshComps.end()) mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
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

Mesh* Renderer::GetMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

void Renderer::SetLightUniforms(Shader* shader)
{
    Matrix4 invView = mView;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());   // 反转后解析相机世界空间位置
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}

void Renderer::CreateSpriteVerts()
{
    // x, y, z, nx, ny, nz, u, v 顶点坐标、法线和纹理坐标
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
		 0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f,
		 0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

bool Renderer::LoadShaders()
{
    // 网格着色器
    mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

    mMeshShader->SetActive();
    mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 25.0f, 10000.0f);
    mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

    // 2D精灵着色器
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
