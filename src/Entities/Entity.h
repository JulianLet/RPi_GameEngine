#pragma once
#include <cstdint>

enum class EntityTag : uint8_t
{
    Default = 0,
    UI,
    Player,
    Ground,
    Ball,

};

struct Entity
{
    uint32_t mask;
    EntityTag tag = EntityTag::Default;

    bool isAlive = true;
    //bool shouldDelete = false;
};