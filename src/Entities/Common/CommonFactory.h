#pragma once

#include "Common.hpp"
#include <cstdint>

struct World;
struct TransformComponent;

struct CommonFactory
{
    uint8_t CreateTopDownPlayer(World& world, Vector2 startPos, Vector2 size, float moveSpeed, uint16_t color);
    uint8_t CreateSideScrollerPlayer(World& world, Vector2 startPos, Vector2 size, float moveSpeed, uint16_t color, float jumpPower);
    uint8_t CreateStaticCamera(World& world, Vector2 position, float initZoom, float targetZoom);
    uint8_t CrateFollowCammera(World& world, TransformComponent* target, float initZoom, float targetZoom, float speed);
    uint8_t CreateStaticWall(World& world, Vector2 pos, Vector2 size, uint16_t color);
};