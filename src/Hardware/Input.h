#pragma once
#include "Globals.h"


enum KEYCODE {START = 0, SELECT, UP, DOWN, LEFT, RIGHT, A, B, COUNT}; //add count as last to have the number available everywhere

struct KeyState
{
    bool pressed = false;
    bool held = false;
    bool released = false;
};

class Input
{
    private:
    KeyState myKeys[KEYCODE::COUNT]; //safe state

    int gpioPins[KEYCODE::COUNT] //save pins
    {
        PIN_START, PIN_SELECT,
        PIN_UP, PIN_DOWN, PIN_LEFT, PIN_RIGHT,
        PIN_A, PIN_B 
    };

    public:
    Input();
    void Update();
    KeyState& GetKey(KEYCODE key);
};