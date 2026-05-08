#include "ECS.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    ComponentManager cm;
    cm.init(2);

    cm.positions[0] = {0, 0};
    cm.positions[1] = {1, 0.5f};

    cm.velocities[0] = {10, 10};
    cm.velocities[1] = {5, 0};

    MovementSystem sys;

    for (int frame = 0; frame < 5; ++frame)
    {
        std::cout << "frame " << frame << ":\n";
        for (int i = 0; i < 2; ++i)
        {
            std::cout << " Entity " << i << ":("
                      << cm.positions[i].x << ", "
                      << cm.positions[i].y << ")\n";
        }
        sys.update(cm, 2, 1.0f / 60.0f);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}