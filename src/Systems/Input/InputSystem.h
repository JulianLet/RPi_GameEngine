#pragma once

struct World;
class Input;

class InputSystem
{
    public:
    InputSystem() = default;
    ~InputSystem() = default;

    void Update(World& world, Input& input);
};