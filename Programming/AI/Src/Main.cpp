#include"Game.h"
#undef main     // 阻止SDL2对main的宏定义

int main(int argc, char** argv){
    Game game;
    bool success=game.Initialize();
    if(success){
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}