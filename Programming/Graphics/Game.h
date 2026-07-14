#include<SDL2/SDL.h>
#include<GL/glew.h>

class Game{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    void InitSpriteVerts();
    class VertexArray mSpriteVerts;

    SDL_Window* mWindow;
    SDL_GLContext mContext;     // OpenGL上下文
    bool mIsRunning;
    Uint32 mTicksCount;
};