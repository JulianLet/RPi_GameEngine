#include "TilemapSystem.h"

#include "Managers/Game/World.h"
#include "Hardware/Renderer.h"

// Components
#include "Entities/Components/Tiles/TilemapComponent.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/PhysicsComponent.h"

// Bitmask
#include "Globals.h"
#include "Common.hpp"

void TilemapSystem::InitColliders(World& world)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!(world.entities[e].mask & TilemapBit)) continue;

        auto& tilemap = world.tilemaps[e];
        auto& tileset = world.tilesets[tilemap.tilesetId];

        for (size_t idx = 0; idx < tilemap.tileIndices.size(); ++idx)
        {
            int tileID = tilemap.tileIndices[idx];

            if (!tileset.solidTiles[tileID])
                continue;

            int x = idx % tilemap.width;
            int y = idx / tilemap.width;

            uint8_t id = world.CreateEntity();

            world.transforms[id] =
            {
                { x * tileset.tileWidth, y * tileset.tileHeight },
                { tileset.tileWidth, tileset.tileHeight }
            };

            world.colliders[id] =
            {
                {0, 0},
                {tileset.tileWidth, tileset.tileHeight},
                SOLID
            };

            world.physics[id] =
            {
                NO_GRAVITY,
                PhysicsType::STATIC,
                {0, 0}
            };

            world.entities[id].mask =
                TransformBit |
                ColliderBit |
                PhysicsBit;
        }
    }
}

void TilemapSystem::Render(World& world, Renderer& renderer, float screenX, float screenY, float zoom)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!(world.entities[e].mask & TilemapBit)) continue;

        auto& tilemap = world.tilemaps[e];
        auto& tileset = world.tilesets[tilemap.tilesetId];

        int tilesX = (int)((ST7735::WIDTH  / (tileset.tileWidth  * zoom)) + 2);
        int tilesY = (int)((ST7735::HEIGHT / (tileset.tileHeight * zoom)) + 2);

        int startX = std::max(0, (int)(-screenX / (tileset.tileWidth * zoom)));
        int startY = std::max(0, (int)(-screenY / (tileset.tileHeight * zoom)));

        int endX = std::min(tilemap.width, startX + tilesX);
        int endY = std::min(tilemap.height, startY + tilesY);

        for (int y = startY; y < endY; y++)
        {
            for (int x = startX; x < endX; x++)
            {
                int tileIndex = tilemap.tileIndices[x + y * tilemap.width];

                const auto& sprite = tileset.tiles[tileIndex];

                int drawX = (int)(screenX + x * tileset.tileWidth * zoom);
                int drawY = (int)(screenY + y * tileset.tileHeight * zoom);

                renderer.DrawSprite(
                    drawX,
                    drawY,
                    tileset.tileWidth,
                    tileset.tileHeight,
                    sprite,
                    zoom,
                    false
                );
            }
        }
    }
}