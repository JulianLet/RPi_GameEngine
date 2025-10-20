#pragma once
#include "hardware/spi.h"
#include <cstdlib>   // for rand() and srand()


//layout of button pins
constexpr int PIN_START     = 1;
constexpr int PIN_SELECT    = 2;
constexpr int PIN_UP        = 3;
constexpr int PIN_DOWN      = 4;
constexpr int PIN_LEFT      = 5;
constexpr int PIN_RIGHT     = 6;
constexpr int PIN_A         = 7;
constexpr int PIN_B         = 8;


//SPIO PINS
constexpr int PIN_SCK           = 18; //SCL, SPI0 SCK
constexpr int PIN_MOSI          = 19; //SDA, SPI0 TX
constexpr int PIN_MISO          = 16; //MISO, SPI0 RX

//layout display pins
constexpr int PIN_CS_DISPLAY    = 17; //SPI0 CSN //chip select pin -> on low the device is selected it reads spi traffic
constexpr int PIN_DC_DISPLAY    = 21; //checks if its data or a command incomming (data/command pin)
constexpr int PIN_RST_DISPLAY   = 20; //if LOW it resets, if HIGH its active

//layout sd reader pins
constexpr int PIN_CS_SD_MODULE  = 22;

namespace Color
{
    // Your display = RBG (5/6/5 bits)
    inline uint16_t Color(uint8_t r, uint8_t g, uint8_t b)
    {
        uint16_t red   = (r >> 3) << 11;  // 5 bits
        uint16_t blue  = (b >> 2) << 5;   // 6 bits, but stored in middle!
        uint16_t green = (g >> 3);        // 5 bits at bottom
        return red | blue | green;
    }

    const uint16_t WHITE    = Color(255, 255, 255);
    const uint16_t LIGHTGRAY= Color( 50,  50,  50);
    const uint16_t GRAY     = Color(128, 128, 128);
    const uint16_t BLACK    = Color(  0,   0,   0);

    const uint16_t RED      = Color(255,   0,   0);
    const uint16_t GREEN    = Color(  0, 255,   0);
    const uint16_t BLUE     = Color(  0,   0, 255);

    const uint16_t YELLOW   = Color(255, 255,   0);
    const uint16_t CYAN     = Color(  0, 255, 255);
    const uint16_t MAGENTA  = Color(255,   0, 255);
    
    const uint16_t ORANGE   = Color(255, 165,   0);
    const uint16_t PURPLE   = Color(128,   0, 128);

    inline uint16_t RANDOM() // must seed rand() before use
    {
        uint8_t r = rand() % 32; // 0–31
        uint8_t b = rand() % 64; // 0–63
        uint8_t g = rand() % 32; // 0–31

        return (r << 11) | (b << 5) | g;
    }
};

