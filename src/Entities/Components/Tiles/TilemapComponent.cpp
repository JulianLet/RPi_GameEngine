#include "TilemapComponent.h"

#include "Hardware/Renderer.h"

#include "Systems/Events/EventSystem.h"
#include "Systems/Events/Event.h"

#include "Entities/Components/Render/RenderableComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"
#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/Physics/ColliderComponent.h"
#include "Entities/Components/Physics/PhysicsComponent.h"


Tileset::Tileset(int w, int h, int tileCount, const char *filePath)
    : tileWidth(w), tileHeight(h), spritesheet(ResourceManager::GetInstance().sdManager.Open(filePath, FA_READ))
{
    for (int i = 0; i < tileCount; i++)
    {
        tiles.emplace_back(ResourceManager::GetInstance().GetSprite(&spritesheet, tileWidth, tileHeight, i));
        solidTiles.emplace_back(false); //init all as false
    }

    printf("Tileset init done");
}

Tileset::~Tileset()
{
    spritesheet.Close();
}

bool Tileset::IsSolid(int index)
{
    return solidTiles.at(index);
}

std::vector<uint8_t>& Tileset::GetSprite(int index)
{
    
    if (index > tiles.size() - 1) return tiles.at(0);

    return tiles.at(index);
    //return ResourceManager::GetInstance().GetSprite(&spritesheet, tileWidth, tileHeight, index);
}

TilemapComponent::TilemapComponent(int width, int height, const char *filePath, Tileset& tileset)
    : width(width), height(height), tileset(tileset)
{
    File tilemap = ResourceManager::GetInstance().sdManager.Open(filePath, FA_READ);
    std::string text = tilemap.Read();
    tilemap.Close();

    for (char c : text)
    {
        if (c == '\n') continue;

        int index = c - '0'; //get index with ASCII

        tileIndices.emplace_back(index); //fill with indeces
    }

    //create colliders of solid tiles
    for (size_t idx = 0; idx < tileIndices.size(); ++idx)
    {
        int tileID = tileIndices[idx];
        if (!tileset.solidTiles[tileID]) continue;

        int xPos = idx % width; // tile X position in map
        int yPos = idx / width; // tile Y position in map

        Entity* e = new Entity();
        e->AddComponent<TransformComponent>(
            Vector2(xPos * tileset.tileWidth, yPos * tileset.tileHeight),
            Vector2(tileset.tileWidth, tileset.tileHeight)
        );
        e->AddComponent<ColliderComponent>(Vector2(0,0), Vector2(tileset.tileWidth, tileset.tileHeight), SOLID);
        e->AddComponent<PhysicsComponent>(NO_GRAVITY, Vector2(0,0), PhysicsType::STATIC);
        EventSystem::GetInstance().DispatchEvent(EventSpawnEntity(e));
    }

    printf("Tilemap init done");
}

TilemapComponent::~TilemapComponent() = default;

void TilemapComponent::Render(Renderer &renderer, int screenX, int screenY, float zoom)
{
    // Number of tiles that fit on screen (plus one extra to handle partial tiles)
    int tilesXOnScreen = (int)((ST7735::WIDTH  / (tileset.tileWidth  * zoom)) + 2);
    int tilesYOnScreen = (int)((ST7735::HEIGHT / (tileset.tileHeight * zoom)) + 2);

    // Starting tile index (tilemap coordinates)
    int startTileX = std::max(0, (int)(-screenX / (tileset.tileWidth * zoom)));
    int startTileY = std::max(0, (int)(-screenY / (tileset.tileHeight * zoom)));

    // Clamp to tilemap size
    int endTileX = std::min(width, startTileX + tilesXOnScreen);
    int endTileY = std::min(height, startTileY + tilesYOnScreen);

    for (int y = startTileY; y < endTileY; y++)
    {
        for (int x = startTileX; x < endTileX; x++)
        {
            int tileIndex = tileIndices[x + y * width];
            std::vector<uint8_t>& spritePixels = tileset.GetSprite(tileIndex);

            if (spritePixels.empty()) continue;

            int tileScreenX = (int)(screenX + x * tileset.tileWidth * zoom);
            int tileScreenY = (int)(screenY + y * tileset.tileHeight * zoom);

            renderer.DrawSprite(
                tileScreenX,
                tileScreenY,
                tileset.tileWidth,
                tileset.tileHeight,
                spritePixels,
                zoom,
                false
            );
        }
    }
}





void TilemapComponent::Reset()
{

}


// int c = 'c' - '0' with ASCII