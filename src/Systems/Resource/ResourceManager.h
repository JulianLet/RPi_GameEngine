#pragma once

#include "Hardware/SDCardManager.h"


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
};