#pragma once
#include <vector>
#include <iostream>

struct Position
{
    float x, y;
};
struct Velocity
{
    float vx, vy;
};
class ComponentManager
{
public:
    std::vector<Position> positions;
    std::vector<Velocity> velocities;

    void init(size_t n)
    {
        positions.resize(n);
        velocities.resize(n);
    }
};
class MovementSystem
{
public:
    void update(ComponentManager &cm, size_t count, float dt)
    {
        for (size_t i = 0; i < count; ++i)
        {
            cm.positions[i].x += cm.velocities[i].vx * dt;
            cm.positions[i].y += cm.velocities[i].vy * dt;
        }
    }
};