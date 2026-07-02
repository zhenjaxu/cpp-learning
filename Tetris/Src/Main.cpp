#include"Game/Game.h"
#undef main

int main(int argc, char** argv){
    Game game;
    bool success=game.Initialize();
    if(success){
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}