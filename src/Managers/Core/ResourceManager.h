#pragma once

#include "Hardware/SDCardManager.h"

#include <vector>

class ResourceManager
{
    private:
    ResourceManager();
    ~ResourceManager() = default;

    // Delete copy/move
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    public:
    SDCardManager sdManager;

    static ResourceManager& GetInstance();
    bool Initialize();
    std::vector<uint16_t> GetSprite(File* file, int width, int height, int index);
};