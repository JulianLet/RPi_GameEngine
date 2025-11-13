#include "SpriteComponent.h"

#include "Systems/Resource/ResourceManager.h"
#include <cstring>

SpriteComponent::SpriteComponent(int width, int height, const char *path, int index)
    : width(width), height(height)
{
    File f = ResourceManager::GetInstance().sdManager.Open(path, FA_READ);
    pixels = ResourceManager::GetInstance().GetSprite(&f, width, height, index);
    f.Close();
}

SpriteComponent::~SpriteComponent() = default;

void SpriteComponent::Reset()
{

}
