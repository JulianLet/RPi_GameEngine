#pragma once
#include "hardware/spi.h"


// Entities
#define INVALID_ENTITY 255
#define MAX_ENTITIES   30

// Layout of button pins
#define PIN_START      1
#define PIN_SELECT     2
#define PIN_UP         3
#define PIN_DOWN       4
#define PIN_LEFT       5
#define PIN_RIGHT      6
#define PIN_A          7
#define PIN_B          8

// SPI0 pins
#define PIN_SCK        18  // SCL, SPI0 SCK
#define PIN_MOSI       19  // SDA, SPI0 TX
#define PIN_MISO       16  // MISO, SPI0 RX

// Display pins
#define PIN_CS_DISPLAY  17 // SPI0 CSN
#define PIN_DC_DISPLAY  21 // Data/command select pin
#define PIN_RST_DISPLAY 20 // Reset pin (LOW = reset)

// SD reader pins
#define PIN_CS_SD_MODULE 22