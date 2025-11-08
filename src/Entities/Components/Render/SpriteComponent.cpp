#include "SpriteComponent.h"

#include "Systems/Resource/ResourceManager.h"

SpriteComponent::SpriteComponent(int width, int height, const char *path)
    : width(width), height(height)
{
    File f = ResourceManager::GetInstance().sdManager.Open(path, FA_READ);
    std::string text = f.Read();
    f.Close();

    pixels.resize(text.size() / 2);

    //combine two bytes from string to color
    for (size_t i = 0; i < pixels.size(); ++i)
    {
        uint8_t lo = static_cast<uint8_t>(text[i * 2]);
        uint8_t hi = static_cast<uint8_t>(text[i * 2 + 1]);

        pixels[i] = static_cast<uint16_t>(lo) | (static_cast<uint16_t>(hi) << 8);
    }

}

SpriteComponent::SpriteComponent(int width, int height, const char *path, int index)
    : width(width), height(height)
{
    File f = ResourceManager::GetInstance().sdManager.Open(path, FA_READ);
    std::string text = f.Read();
    f.Close();

    size_t totalPixels = text.size() / 2;

    //combine two bytes from string to color
    std::vector<uint16_t> sheetPixels(totalPixels);
    for (size_t i = 0; i < totalPixels; ++i)
    {
        uint8_t lo = static_cast<uint8_t>(text[i * 2]);
        uint8_t hi = static_cast<uint8_t>(text[i * 2 + 1]);
        sheetPixels[i] = static_cast<uint16_t>(lo) | (static_cast<uint16_t>(hi) << 8);
    }

    pixels.resize(width * height);

    int offset = index * width * height; 
    for (int i = 0; i < width * height; i++)
    {
        pixels[i] = sheetPixels[offset + i];
    }
}

SpriteComponent::SpriteComponent(int width, int height, const char* path, int posX, int posY, int sheetWidth)
    : width(width), height(height)
{
    File f = ResourceManager::GetInstance().sdManager.Open(path, FA_READ);
    std::string text = f.Read();
    f.Close();

    // total pixels in the sheet
    size_t totalPixels = text.size() / 2;

    // convert raw bytes to uint16_t
    std::vector<uint16_t> sheetPixels(totalPixels);
    for (size_t i = 0; i < totalPixels; ++i)
    {
        uint8_t lo = static_cast<uint8_t>(text[i * 2]);
        uint8_t hi = static_cast<uint8_t>(text[i * 2 + 1]);
        sheetPixels[i] = static_cast<uint16_t>(lo) | (static_cast<uint16_t>(hi) << 8);
    }

    // extract the rectangle
    pixels.resize(width * height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int sheetIndex = (posY + y) * sheetWidth + (posX + x);
            int spriteIndex = y * width + x;
        }
    }
}


SpriteComponent::~SpriteComponent() = default;

void SpriteComponent::Reset()
{

}
