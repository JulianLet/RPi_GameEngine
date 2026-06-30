#include "TilemapSystem.h"

#include "Managers/Game/World.h"
#include "Hardware/Renderer.h"

// Components
#include "Entities/Components.h"

// Bitmask
#include "Globals.h"
#include "Common.hpp"

void TilemapSystem::InitColliders(World& world)
{
    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!(world.entities[e].mask & TilemapBit)) continue;

        auto& tilemap = world.tilemaps[e];
        auto& tileset = world.assets.myTileset;

        Vector2 size = Vector2(tileset.tileWidth, tileset.tileHeight);

        for (size_t idx = 0; idx < tilemap.tileIndices.size(); ++idx)
        {
            int tileID = tilemap.tileIndices[idx];

            if (!tileset.solidTiles[tileID])
                continue;

            int x = idx % tilemap.width;
            int y = idx / tilemap.width;

            uint8_t id = world.CreateEntity();

            world.entities[id].mask = TransformBit | ColliderBit | PhysicsBit;

            world.transforms[id] =
            {
                .currentPosition = Vector2(x * size.x, y * size.y),
                .lastPosition = Vector2(x * size.x, y * size.y),
                .currentSize = size
            };

            world.colliders[id] =
            {
                .offset = Vector2(0,0),
                .size = size,
                .isTrigger = SOLID
            };

            world.physics[id] =
            {
                . useGravity = NO_GRAVITY,
                .physicsType = PhysicsType::STATIC,
                .currentVelocity = Vector2(0,0)
            };
        }
    }
}

void TilemapSystem::Render(World& world, Renderer& renderer)
{
    Vector2 cameraPos = Vector2(0,0);
    float currentZoom = 1;

    if (world.activeCamera != INVALID_ENTITY)
    {
        auto& camTransform = world.transforms[world.activeCamera];
        cameraPos = camTransform.currentPosition;

        currentZoom = world.cameras[world.activeCamera].currentZoom;

        cameraPos.x -= ST7735::WIDTH  / (2 * currentZoom);
        cameraPos.y -= ST7735::HEIGHT / (2 * currentZoom);
    }

    for (uint8_t e = 0; e < MAX_ENTITIES; e++)
    {
        if (!world.entities[e].isAlive) continue;
        if (!(world.entities[e].mask & TilemapBit)) continue;

        auto& tilemap = world.tilemaps[e];
        auto& tileset = world.assets.myTileset;//[tilemap.tilesetId];

        int tileW = tileset.tileWidth;
        int tileH = tileset.tileHeight;

        int tilesX = (int)((ST7735::WIDTH  / (tileW * currentZoom)) + 2);
        int tilesY = (int)((ST7735::HEIGHT / (tileH * currentZoom)) + 2);

        int startX = std::max(0, (int)(cameraPos.x / tileW));
        int startY = std::max(0, (int)(cameraPos.y / tileH));

        int endX = std::min(tilemap.width, startX + tilesX);
        int endY = std::min(tilemap.height, startY + tilesY);

        for (int y = startY; y < endY; y++)
        {
            for (int x = startX; x < endX; x++)
            {
                int tileIndex = tilemap.tileIndices[x + y * tilemap.width];

                auto& sprite = world.assets.GetOrLoadSprite(tileset.sprites[tileIndex]);

                float worldX = x * tileW;
                float worldY = y * tileH;

                float screenX = (worldX - cameraPos.x) * currentZoom;
                float screenY = (worldY - cameraPos.y) * currentZoom;

                if (screenX + tileW * currentZoom < 0 || screenX > ST7735::WIDTH ||
                    screenY + tileH * currentZoom < 0 || screenY > ST7735::HEIGHT)
                {
                    continue;
                }

                renderer.DrawSprite(
                    (int)screenX,
                    (int)screenY,
                    sprite,
                    currentZoom,
                    false
                );
            }
        }
    }
}