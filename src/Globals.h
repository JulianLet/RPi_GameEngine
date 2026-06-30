#pragma once

enum 
{
    INVALID_ENTITY   = 255,
    MAX_ENTITIES     = 60,
    MAX_LAYERS       = 10,
 
    MAX_TILES        = 15,
    INVALID_SPRITE   = 255,
    MAX_SPRITES      = 32,
    MAX_SPRITE_PIXELS = 1024, // 32 * 32
    MAX_ANIMATIONS   = 32,
 
    PIN_START        = 1,
    PIN_SELECT       = 2,
    PIN_UP           = 3,
    PIN_DOWN         = 4,
    PIN_LEFT         = 5,
    PIN_RIGHT        = 6,
    PIN_A            = 7,
    PIN_B            = 8,
 
    PIN_SCK          = 18,
    PIN_MOSI         = 19,
    PIN_MISO         = 16,
 
    PIN_CS_DISPLAY   = 17,
    PIN_DC_DISPLAY   = 21,
    PIN_RST_DISPLAY  = 20,

    PIN_CS_SD_MODULE = 22
};