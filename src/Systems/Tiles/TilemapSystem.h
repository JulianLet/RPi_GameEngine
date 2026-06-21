#pragma once

class World;
class Renderer;

class TilemapSystem
{
public:
    void InitColliders(World& world);
    // void Update(World& world, float dt);
    void Render(World& world, Renderer& renderer, float screenX, float screenY, float zoom);
};