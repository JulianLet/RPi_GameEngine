#pragma once

#include "Entities/Component.h"

#include <unordered_map>
#include <vector>
#include <cstdint>

enum AnimationMode {SINGLE = 0, BOUNCE, LOOP};

struct Animation
{
    std::vector<std::vector<uint16_t>> animationFrames;
    AnimationMode mode;
    int frameAmount;
    int currentFrame = 0;

    Animation(AnimationMode mode, int frameAmount, int width, int height, const char* filePath);
};

struct AnimationComponent : public Component
{
    std::unordered_map<int, Animation> animationList;
    int currentAnimation = 0;

    int width;
    int height;
    
    float currentTime = 0;    //to track when to switch frame
    int FPS = 0;
    int direction = 1;      //play forward or backwards

    AnimationComponent(int spriteWidth, int spriteHeight);
    ~AnimationComponent() override;
    void AddAnimation(int ID, AnimationMode mode, int frameAmount, int framesPerSecond, const char* filePath);
    std::vector<uint16_t>& GetCurrentFrame();

    void Reset() override;
};