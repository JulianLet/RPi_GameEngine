#pragma once

enum AnimState {Idle, Walk, Jump, Sprint, STATE_COUNT};

struct AnimationSetComponent
{
    uint8_t states[STATE_COUNT];
};