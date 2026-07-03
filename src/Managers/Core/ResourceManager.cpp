#include "ResourceManager.h"

#include "Globals.h"
#include "Managers/Game/World.h"

ResourceManager::ResourceManager()
{

}

ResourceManager &ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::Initialize()
{
    return sdManager.Initialize();
}

std::vector<uint16_t> ResourceManager::GetSprite(File* file, int width, int height, int index)
{
    if (!file) return {};

    size_t pixelCount = width * height;
    size_t frameSizeBytes = pixelCount * sizeof(uint16_t);

    size_t offset = index * frameSizeBytes;

    file->Seek(offset);

    std::vector<uint16_t> frame(pixelCount);

    file->Read(
        reinterpret_cast<uint8_t*>(frame.data()),
        frameSizeBytes
    );

    return frame;
}