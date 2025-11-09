#include "AnimationComponent.h"
#include "Systems/Resource/ResourceManager.h"

Animation::Animation(AnimationMode mode, int frameAmount, int width, int height, const char *filePath)
    : frameAmount(frameAmount), mode(mode)
{

    File f = ResourceManager::GetInstance().sdManager.Open(filePath, FA_READ);
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

    for (int i = 0; i < frameAmount; i++)
    {
        std::vector<uint16_t> pixels;
        pixels.resize(width * height);
        
        int offset = i * width * height; 

        for (int j = 0; j < width * height; j++)
        {
            pixels[j] = sheetPixels[offset + j];
        }

        animationFrames.push_back(pixels);
    }
}

AnimationComponent::AnimationComponent(int spriteWidth, int spriteHeight)
    : width(spriteWidth), height(spriteHeight)
{
    currentTime = 0;
    currentAnimation = 0;
    direction = 1;
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::AddAnimation(int ID, AnimationMode mode, int frameAmount, int framesPerSecond, const char *filePath)
{
    FPS = framesPerSecond;
    animationList.emplace(ID, Animation(mode, frameAmount, width, height, filePath));
}

std::vector<uint16_t> &AnimationComponent::GetCurrentFrame()
{
    Animation& current = animationList.at(currentAnimation);
    return current.animationFrames[current.currentFrame];
}

void AnimationComponent::Reset()
{
    currentTime = 0;
    currentAnimation = 0;
    direction = 1;
}
