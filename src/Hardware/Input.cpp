#include "Hardware/Input.h"
#include "hardware/gpio.h"

Input::Input()
{
    for (int i = 0; i < KEYCODE::COUNT; i++)
    {
        gpio_init(gpioPins[i]);
        gpio_set_dir(gpioPins[i], GPIO_IN);
    }
}

void Input::Update()
{
    for (int i = 0; i < KEYCODE::COUNT; i++)
    {
        bool isPressed = gpio_get(gpioPins[i]);
        KeyState& ks = myKeys[i];

        ks.pressed = !ks.held && isPressed;
        ks.released = ks.held && !isPressed;
        ks.held = isPressed;
    }
}

KeyState &Input::GetKey(KEYCODE key)
{
    return myKeys[key];
}
