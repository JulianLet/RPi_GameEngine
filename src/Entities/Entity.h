#pragma once

struct Entity
{
    uint8_t id;
    uint32_t mask;

    bool isAlive = true;
    bool shouldDelete = false;
};