#pragma once
#include "hardware/spi.h"

// Entites
constexpr uint8_t INVALID_ENTITY = 255;
constexpr int MAX_ENTITIES = 20;

// Layout of button pins
constexpr int PIN_START     = 1;
constexpr int PIN_SELECT    = 2;
constexpr int PIN_UP        = 3;
constexpr int PIN_DOWN      = 4;
constexpr int PIN_LEFT      = 5;
constexpr int PIN_RIGHT     = 6;
constexpr int PIN_A         = 7;
constexpr int PIN_B         = 8;

// SPI0 PINS
constexpr int PIN_SCK         = 18;  // SCL, SPI0 SCK
constexpr int PIN_MOSI        = 19;  // SDA, SPI0 TX
constexpr int PIN_MISO        = 16;  // MISO, SPI0 RX

// Layout display pins
constexpr int PIN_CS_DISPLAY  = 17;  // SPI0 CSN
constexpr int PIN_DC_DISPLAY  = 21;  // Data/command select pin
constexpr int PIN_RST_DISPLAY = 20;  // Reset pin (LOW = reset)

// Layout SD reader pins
constexpr int PIN_CS_SD_MODULE = 22;