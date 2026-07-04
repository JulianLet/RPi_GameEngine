#include "AssetManager.h"

#include "Managers/Managers.h"

uint8_t AssetManager::AddSprite(const char *path, uint16_t w, uint16_t h, uint8_t sheetIndex)
{  
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        if (!mySprites.filled[i])
        {
            mySprites.sprites[i] = {
                .path = path,
                .width = w,
                .height = h,
                .indexOnSpritesheet = sheetIndex
            };

            mySprites.filled[i] = true;
            return i;
        }
    }

    return INVALID_SPRITE; // database is full
}

Sprite &AssetManager::GetOrLoadSprite(int spriteID)
{
    if (spriteID == INVALID_SPRITE || spriteID >= MAX_SPRITES || !mySprites.filled[spriteID])
    {
        return myCache.sprites[0]; // fallback sprite
    }

    auto& def = mySprites.sprites[spriteID];
    auto& cached = myCache.sprites[spriteID];
    auto& loaded = myCache.loaded[spriteID];

    if (loaded) return cached;

    File file = ResourceManager::GetInstance().sdManager.Open(def.path, FA_READ);

    auto pixels = ResourceManager::GetInstance().GetSprite(&file, def.width, def.height, def.indexOnSpritesheet);

    cached.width = def.width;
    cached.height = def.height;

    // allocate / assign pixels (depends on your memory model)
    size_t copyCount = std::min(pixels.size(), (size_t)MAX_SPRITE_PIXELS);
    memcpy(cached.pixels, pixels.data(), copyCount * sizeof(uint16_t));

    loaded = true;

    file.Close();

    return cached;
}

uint8_t AssetManager::AddAnimation(const char *path, uint16_t w, uint16_t h, uint8_t frames, uint8_t fps, AnimationMode mode)
{
    uint8_t firstSpriteID = INVALID_SPRITE;

    for (int i = 0; i < frames; i++)
    {
        uint8_t id = AddSprite(path, w, h, i);

        if (id == INVALID_SPRITE)
            return INVALID_SPRITE;

        if (i == 0)
            firstSpriteID = id;
    }

    for (int i = 0; i < MAX_ANIMATIONS; i++)
    {
        if (!myAnimations.filled[i])
        {
            myAnimations.clips[i] = 
            {
                .frameStart = firstSpriteID,
                .frameCount = frames,
                .fps = fps,
                .mode = mode,
            };

            myAnimations.filled[i] = true;
            return i;
        }
    }

    return INVALID_SPRITE; // database is full
}

void AssetManager::CreateTileset(const char *path, uint16_t w, uint16_t h, uint8_t tileCount)
{
    myTileset.tileWidth = w;
    myTileset.tileHeight = h;

    for (int i = 0; i < tileCount; i++)
    {
        myTileset.sprites[i] = AddSprite(path, w, h, i);
        myTileset.solidTiles[i] = false;
    }
}