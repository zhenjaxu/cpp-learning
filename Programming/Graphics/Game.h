#include<SDL2/SDL.h>
#include<GL/glew.h>
#include<string>
#include<unordered_map>
#include<vector>

class Game{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    class Texture* GetTexture(const std::string& fileName);

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    void InitSpriteVerts();
    bool LoadShaders();

    class VertexArray mSpriteVerts;
    class Shader* mSpriteShader;

    std::vector<class SpriteComponent*> mSprites;
    std::unordered_map<std::string, class Texture*> mTextures;

    SDL_Window* mWindow;
    SDL_GLContext mContext;     // OpenGL上下文
    bool mIsRunning;
    Uint32 mTicksCount;

    bool mUpdatingActors;
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;
};