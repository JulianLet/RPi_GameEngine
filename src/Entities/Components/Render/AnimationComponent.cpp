#include "AnimationComponent.h"
#include "Systems/Resource/ResourceManager.h"

Animation::Animation(AnimationMode mode, int frameAmount, int width, int height, const char *filePath)
    : frameAmount(frameAmount), mode(mode), width(width), height(height), file(ResourceManager::GetInstance().sdManager.Open(filePath, FA_READ))
{
    currentFrame = 0;
    UpdateFrame();
}

Animation::~Animation()
{
    file.Close();
}


void Animation::UpdateFrame()
{
    frameBuffer = ResourceManager::GetInstance().GetSprite(&file, width, height, currentFrame);
    // size_t frameSizeBytes = width * height * 2;
    // size_t offset = currentFrame * frameSizeBytes;

    // file.Seek(offset);
    // frameBuffer.resize(width * height);
    // std::vector<uint8_t> raw(frameSizeBytes);
    // file.Read(raw.data(), frameSizeBytes);

    // for (size_t i = 0; i < width * height; ++i)
    // {
    //     uint8_t lo = raw[i * 2];
    //     uint8_t hi = raw[i * 2 + 1];
    //     frameBuffer[i] = (hi << 8) | lo;
    // }
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
    animationList[ID] = std::make_unique<Animation>(mode, frameAmount, width, height, filePath);
}

void AnimationComponent::SwapAnimation(int ID)
{
    currentTime = 0;
    direction = 1;
    auto& current = animationList.at(currentAnimation)->currentFrame = 0;
    currentAnimation = ID;
}

const std::vector<uint8_t> &AnimationComponent::GetCurrentFrame()
{
    auto& current = animationList.at(currentAnimation);
    return current->frameBuffer;
}

void AnimationComponent::Reset()
{
    currentTime = 0;
    currentAnimation = 0;
    direction = 1;
}
