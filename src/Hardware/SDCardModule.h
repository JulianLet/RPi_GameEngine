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
    bool isHighCapacity;

    void WriteCommand(uint8_t cmd);
    void WriteData(const uint8_t* data, size_t len);
    uint8_t ReadByte();
    void ReadData(uint8_t* buffer, size_t len);

public:
    SDCardModule(spi_inst_t* spi, int csPin)
        : spiPort(spi), myCsPin(csPin), isHighCapacity(false)
    {}
    ~SDCardModule() = default;

    bool Initialize();
    bool SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t* response = nullptr);
    bool ReadBlock(uint8_t* buffer, uint32_t blockAddress);
    bool WriteBlock(const uint8_t* buffer, uint32_t blockAddress);
    bool IsHighCapacity() const { return isHighCapacity; }
};
