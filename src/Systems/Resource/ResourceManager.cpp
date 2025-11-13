#include "ResourceManager.h"
#include "Globals.h"

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

std::vector<uint8_t> ResourceManager::GetSprite(File* file, int width, int height, int index)
{
    if (!file) return {};

    size_t frameSizeBytes = width * height;      // 1 byte per pixel
    size_t offset = index * frameSizeBytes;

    file->Seek(offset);

    std::vector<uint8_t> frame(frameSizeBytes);
    file->Read(frame.data(), frameSizeBytes);

    return frame;
}