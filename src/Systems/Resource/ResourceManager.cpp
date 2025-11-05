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