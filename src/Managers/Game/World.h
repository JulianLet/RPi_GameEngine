#pragma once
#include "Common.hpp"

#include "Entities/Entity.h"
#include "Entities/Components.h"

#include "Managers/Core/AssetManager.h"

#include <cstdint>

struct World
{
    Entity entities[MAX_ENTITIES];
    uint8_t entityCount = 0;
    uint8_t activeCamera = INVALID_ENTITY;

    AssetManager assets;

    // AnimationDatabase animationDB;
    // SpriteDatabase spriteDB;
    // SpriteCache spriteCache;
    
    // ===== Core =====
    JumpComponent jumps[MAX_ENTITIES];
    TransformComponent transforms[MAX_ENTITIES];
    MovementComponent movements[MAX_ENTITIES];
    TimerComponent timers[MAX_ENTITIES];
    
    // ===== AI =====
    AIComponent ai[MAX_ENTITIES];
    
    // ===== Input =====
    InputIntendComponent inputIntends[MAX_ENTITIES];
    InputMappingComponent inputMappings[MAX_ENTITIES];
    
    // ===== Physics =====
    PhysicsComponent physics[MAX_ENTITIES];
    ColliderComponent colliders[MAX_ENTITIES];
    CollisionResponseComponent collisionResponses[MAX_ENTITIES];
    PhysicsMaterialComponent physicsMaterials[MAX_ENTITIES];

    // ===== Events =====
    OnEventComponent events[MAX_ENTITIES];
    
    // ===== Render =====
    SpriteComponent sprites[MAX_ENTITIES];
    RenderableComponent renderables[MAX_ENTITIES];
    RectangleComponent rectangles[MAX_ENTITIES];
    
    // ===== Animation =====
    AnimationComponent animations[MAX_ENTITIES];
    AnimationSetComponent animSets[MAX_ENTITIES];
    
    // ===== Camera / Game logic =====
    CameraComponent cameras[MAX_ENTITIES];
    FollowTargetComponent followTargets[MAX_ENTITIES];
    
    // ===== Tilemap (likely sparse) =====
    TilemapComponent tilemaps[MAX_ENTITIES];
    
    // ===== UI =====
    UIButtonComponent uiButtons[MAX_ENTITIES];
    UITextComponent uiTexts[MAX_ENTITIES];

    uint8_t CreateEntity()
    {
        for (uint8_t i = 0; i < MAX_ENTITIES; i++)
        {
            if (!entities[i].isAlive)
            {
                entities[i].isAlive = true;
                entities[i].mask = 0;

                return i;
            }
        }

        return INVALID_ENTITY;
    }
};