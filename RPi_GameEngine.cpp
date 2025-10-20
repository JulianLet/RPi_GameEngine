#include "Globals.h"

#include "Hardware/Input.h"
#include "Hardware/DisplayST7735.h"
#include "Hardware/Renderer.h"

#include "Systems/Core/GameManager.h"
#include "Systems/Events/EventSystem.h"
#include "Systems/Resource/ResourceManager.h"
#include "Systems/Debug/DebugManager.h"

#include "pico/stdlib.h"

const float TARGET_FPS = 60.0f;
const float FRAME_TIME = 1.0f / TARGET_FPS;


int main()
{
    srand(time_us_64() & 0xFFFFFFFF); //use time to seed RNG
    stdio_init_all();

    //Initialize 
    DebugManager::GetInstance().ClearLogs();
    Input myInput;
    ST7735 myDisplay(spi0, PIN_CS_DISPLAY, PIN_DC_DISPLAY, PIN_RST_DISPLAY);
    Renderer myRenderer(myDisplay);
    GameManager myGameManager;
    ResourceManager& resources = ResourceManager::GetInstance();
    
    resources.Initialize();
    myDisplay.Initialize();

    uint64_t lastFrameStart = time_us_64(); //get the time since boot of the pico

    while (true)
    {
        //update timing
        uint64_t frameStart = time_us_64();
        float deltaTime = (frameStart - lastFrameStart) / 1'000'000.0f; // in seconds
        lastFrameStart = frameStart;

        //Update
        myInput.Update();
        myGameManager.Update(myInput, deltaTime);

        //Render
        myRenderer.Clear();
        myGameManager.Render(myRenderer);
        myRenderer.Display();

        //wait for FPS
        uint64_t frameEnd = time_us_64();
        float elapsed = (frameEnd - frameStart) / 1'000'000.0f;

        if (elapsed < FRAME_TIME) //if light calculation frame sleep
        {
            sleep_us((FRAME_TIME - elapsed) * 1'000'000.0f);
        }
    }
    return 0;
}

