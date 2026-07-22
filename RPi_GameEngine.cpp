#include "Globals.h"

#include "Hardware/Input.h"
#include "Hardware/DisplayST7735.h"
#include "Hardware/Renderer.h"

#include "Managers/Managers.h"

#include "pico/stdlib.h"


const float TARGET_FPS = 60.0f;
const float FRAME_TIME = 1.0f / TARGET_FPS;

void HardwareSetUp();


int main()
{
    srand(time_us_64() & 0xFFFFFFFF); //use time to seed RNG
    stdio_init_all();
    HardwareSetUp();

    sleep_ms(1000);  //delay to open serial monitor
    printf("start programm\n");

    //Initialize 
    DebugManager::GetInstance().ClearLogs();
    Input myInput;
    ST7735 myDisplay(spi0, PIN_CS_DISPLAY, PIN_DC_DISPLAY, PIN_RST_DISPLAY);
    Renderer myRenderer(myDisplay);
    GameManager myGameManager;
    ResourceManager& resources = ResourceManager::GetInstance();

    sleep_ms(200);
    
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
        uint64_t afterUpdate = time_us_64();

        //Render
        myRenderer.Clear();
        myGameManager.Render(myRenderer);
        uint64_t afterRender = time_us_64();

        myRenderer.Display();
        uint64_t afterPresent = time_us_64();

        std::string msg = std::to_string(afterUpdate - frameStart);
        DebugManager::GetInstance().Log(msg.c_str());

        msg = std::to_string(afterRender - afterUpdate);
        DebugManager::GetInstance().Log(msg.c_str());

        msg = std::to_string(afterPresent - afterRender);
        DebugManager::GetInstance().Log(msg.c_str());

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

void HardwareSetUp()
{
    // --- Display CS + control pins ---
    gpio_init(PIN_CS_DISPLAY);
    gpio_set_dir(PIN_CS_DISPLAY, GPIO_OUT);
    gpio_put(PIN_CS_DISPLAY, 1); // deselect display

    gpio_init(PIN_DC_DISPLAY);
    gpio_set_dir(PIN_DC_DISPLAY, GPIO_OUT);
    gpio_put(PIN_DC_DISPLAY, 1);

    gpio_init(PIN_RST_DISPLAY);
    gpio_set_dir(PIN_RST_DISPLAY, GPIO_OUT);
    gpio_put(PIN_RST_DISPLAY, 1);

    // --- SD Card CS pin ---
    gpio_init(PIN_CS_SD_MODULE);
    gpio_set_dir(PIN_CS_SD_MODULE, GPIO_OUT);
    gpio_put(PIN_CS_SD_MODULE, 1); // deselect SD card

    // Optional: ensure MISO line has pull-up for stability
    gpio_pull_up(PIN_MISO0);

    // Allow the SD card to power up and settle
    sleep_ms(50);
}