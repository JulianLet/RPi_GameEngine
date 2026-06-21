#pragma once

#include "Entities/Entity.h"
#include <memory>

enum EventType {START_GAME = 0, QUIT_GAME, BALL_OOB, UPDATE_SCORE, JUMP_EVENT, SPAWN_ENTITY};


struct Event
{
    virtual EventType GetEventType() const = 0;
};

struct EventStartGame : public Event
{
    EventStartGame() = default;
    EventType GetEventType() const override { return EventType::START_GAME; };
};

struct EventQuitGame : public Event
{
    EventQuitGame() = default;
    EventType GetEventType() const override { return EventType::QUIT_GAME; };
};

struct EventBallOOB : public Event
{
    int side;
    EventBallOOB(int side) : side(side) {};
    EventType GetEventType() const override { return EventType::BALL_OOB; };
};

struct EventUpdateScore : public Event
{
    int playerOne = 0;
    int playerTwo = 0;
    float endTime = 0;
    EventUpdateScore(int playerOne, int playerTwo) : playerOne(playerOne), playerTwo(playerTwo) {};
    EventUpdateScore(float endTime) : endTime(endTime) {};
    EventType GetEventType() const override { return EventType::UPDATE_SCORE; };
};

struct EventJump : public Event
{
    EventJump() = default;
    EventType GetEventType() const override { return EventType::JUMP_EVENT; };
};

struct EventSpawnEntity : public Event
{
    Entity* entity;
    EventSpawnEntity(Entity* entity) : entity(entity) {};
    EventType GetEventType() const override { return EventType::SPAWN_ENTITY; };
};