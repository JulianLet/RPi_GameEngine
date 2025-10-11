#pragma once

class Entity;

struct Component
{
    Entity* owner;

    virtual ~Component() {};
    virtual void Reset() {};
};