#pragma once

#include "Entities/Component.h"
#include "Hardware/SDCardManager.h"


#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>


enum AnimationMode {SINGLE = 0, BOUNCE, LOOP};

struct Animation
{
    File file;
    AnimationMode mode;
    int frameAmount;
    int currentFrame = 0;

    std::vector<uint8_t> frameBuffer;

    int width;
    int height;

    Animation(AnimationMode mode, int frameAmount, int width, int height, const char* filePath);
    ~Animation();

    void UpdateFrame();
};

struct AnimationComponent : public Component
{
    std::unordered_map<int, std::unique_ptr<Animation>> animationList;
    int currentAnimation = 0;

    int width;
    int height;
    
    float currentTime = 0;    //to track when to switch frame
    int FPS = 0;
    int direction = 1;      //play forward or backwards

    AnimationComponent(int spriteWidth, int spriteHeight);
    ~AnimationComponent() override;
    void AddAnimation(int ID, AnimationMode mode, int frameAmount, int framesPerSecond, const char* filePath);
    const std::vector<uint8_t>& GetCurrentFrame();

    void Reset() override;
};