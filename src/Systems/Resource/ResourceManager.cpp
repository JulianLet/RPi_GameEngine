#include "ResourceManager.h"
#include "Globals.h"

ResourceManager::ResourceManager()
    : sdModule(spi0, PIN_CS_SD_MODULE), sdManager()
{

}

ResourceManager &ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

bool ResourceManager::Initialize()
{
    return sdModule.Initialize();
}
