#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdint.h>
#include <stddef.h>

class SDCardModule
{
private:
    spi_inst_t* spiPort;
    int myCsPin;

public:
    SDCardModule(spi_inst_t* spi, int csPin)
        : spiPort(spi), myCsPin(csPin)
    {}
    ~SDCardModule() = default;

    bool Initialize();
};
