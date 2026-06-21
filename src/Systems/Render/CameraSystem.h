#pragma once

struct World;

class CameraSystem
{
    public:
    CameraSystem() = default;
    ~CameraSystem() = default;

    void Update(World& world, float deltaTime);
};