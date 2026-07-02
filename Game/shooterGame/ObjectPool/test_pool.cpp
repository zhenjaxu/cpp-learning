#include "ObjectPool.hpp"

struct Bullet{
    int id=0;
    float x=0, y=0;
};

int main(){
    ObjectPool<Bullet> pool(3);

    Bullet* b1=pool.acquire(); b1->id=1; b1->x=100;
    Bullet* b2=pool.acquire(); b2->id=2; b2->x=200;
    Bullet* b3=pool.acquire(); b3->id=3; b3->x=300;

    // 第四个应该失败，超出容量
    Bullet* b4=pool.acquire();  // 预期输出：Pool empty！

    // 回收b2，在申请应该复用同一块内存
    pool.release(b2);
    Bullet* b5=pool.acquire();  // 复用了b2的内存
    std::cout<<"Reused bullet at address: "<<b5<<", old was: "<<b2<<"\n";
    std::cout<<"b5 id: "<<b5->id<<"(should be 2, unless you reset it)\n";

    return 0;
}