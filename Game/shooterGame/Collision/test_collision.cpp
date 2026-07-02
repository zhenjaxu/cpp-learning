#include "Collision.hpp"
#include <iostream>

int main(){
    AABB player={0,0,50,50};        // 玩家：50x50
    AABB enemy1={30,30,50,50};      // 敌机1；重叠，应该HIT
    AABB enemy2={100,100,50,50};    // 敌机2：不重叠，应该MISS

    std::cout<<"Test 1 (should HIT): "
             <<(player.intersects(enemy1)?"HIT!":"MISS")<<"\n";
    std::cout<<"Test 2 (should MISS): "
             <<(player.intersects(enemy2)?"HIT!":"MISS")<<"\n";

    return 0;
}