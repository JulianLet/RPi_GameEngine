#pragma once

#include "Hardware/SDCardModule.h"
#include "Hardware/SDCardManager.h"


class ResourceManager
{
    private:
    SDCardModule sdModule;

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