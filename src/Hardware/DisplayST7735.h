#pragma once
#include "hardware/spi.h"
#include "Globals.h"


class ST7735
{
    private:
    spi_inst_t* spiPort;
    int myCsPin;
    int myDcPin;
    int myRstPin;

    void WriteCommand(uint8_t command);
    void WriteData(uint8_t data);
    void WriteData16(uint16_t data);

    public:
    static const int WIDTH = 130; //128
    static const int HEIGHT = 163; //160
 

    ST7735(spi_inst_t* spi, int csPin, int dcPin, int rstPin) 
        : spiPort(spi), myCsPin(csPin), myDcPin(dcPin), myRstPin(rstPin) {};
    ~ST7735() = default;

    void Initialize();
    void SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void Present(uint8_t* buffer); //send full framebuffer to display
};