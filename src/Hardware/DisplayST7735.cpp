#include "Hardware/DisplayST7735.h"
#include "pico/stdlib.h"

void ST7735::WriteCommand(uint8_t command)
{
    gpio_put(myDcPin, 0); //command mode
    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, &command, 1);
    gpio_put(myCsPin, 1);
}

void ST7735::WriteData(uint8_t data)
{
    gpio_put(myDcPin, 1); //data mode
    gpio_put(myCsPin, 0); //active -> listen to spi
    spi_write_blocking(spiPort, &data, 1);
    gpio_put(myCsPin, 1);
}

void ST7735::WriteData16(uint16_t data)
{
    uint8_t bytes[2] = { (uint8_t)(data >> 8), (uint8_t)(data & 0xFF) }; //split to two separate 8 bits
    gpio_put(myDcPin, 1);
    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, bytes, 2);
    gpio_put(myCsPin, 1);
}

void ST7735::Initialize()
{
    // --- SPI init ---
    spi_init(spiPort, 16*1000*1000);
    spi_set_format(spiPort, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // --- GPIO setup ---
    gpio_init(myCsPin); gpio_set_dir(myCsPin, GPIO_OUT); gpio_put(myCsPin, 1);
    gpio_init(myDcPin); gpio_set_dir(myDcPin, GPIO_OUT);
    gpio_init(myRstPin); gpio_set_dir(myRstPin, GPIO_OUT);

    // --- Hardware reset ---
    gpio_put(myRstPin, 0); sleep_ms(300);
    gpio_put(myRstPin, 1); sleep_ms(200);

    // --- Minimal init sequence using helper functions ---
    WriteCommand(0x11);      // Sleep Out
    sleep_ms(120);

    // --- After Sleep Out (0x11) and delay ---
    WriteCommand(0xB1); // Frame Rate Control (Normal Mode)
    WriteData(0x01);
    WriteData(0x2C);
    WriteData(0x2D);

    WriteCommand(0xB2); // Frame Rate Control (Idle Mode)
    WriteData(0x01);
    WriteData(0x2C);
    WriteData(0x2D);

    WriteCommand(0xB3); // Frame Rate Control (Partial Mode)
    WriteData(0x01);
    WriteData(0x2C);
    WriteData(0x2D);
    WriteData(0x01);
    WriteData(0x2C);
    WriteData(0x2D);

    WriteCommand(0xB4); // Display Inversion Control
    WriteData(0x07);

    WriteCommand(0xC0); // Power Control 1
    WriteData(0xA2);
    WriteData(0x02);
    WriteData(0x84);

    WriteCommand(0xC1); // Power Control 2
    WriteData(0xC5);

    WriteCommand(0xC2); // Power Control 3
    WriteData(0x0A);
    WriteData(0x00);

    WriteCommand(0xC3); // Power Control 4
    WriteData(0x8A);
    WriteData(0x2A);

    WriteCommand(0xC4); // Power Control 5
    WriteData(0x8A);
    WriteData(0xEE);

    WriteCommand(0xC5); // VCOM Control
    WriteData(0x0E);

    WriteCommand(0x36); // MADCTL
    WriteData(0xC8); // BGR order, upside down orientation
    //0x08 would be for normal orientation
    //0xC8 upside down?

    WriteCommand(0x3A); // COLMOD
    WriteData(0x05); // 16-bit color

    // Gamma correction
    WriteCommand(0xE0);
    WriteData(0x0F); WriteData(0x1A); WriteData(0x0F); WriteData(0x18);
    WriteData(0x2F); WriteData(0x28); WriteData(0x20); WriteData(0x22);
    WriteData(0x1F); WriteData(0x1B); WriteData(0x23); WriteData(0x37);
    WriteData(0x00); WriteData(0x07); WriteData(0x02); WriteData(0x10);

    WriteCommand(0xE1);
    WriteData(0x0F); WriteData(0x1B); WriteData(0x0F); WriteData(0x17);
    WriteData(0x33); WriteData(0x2C); WriteData(0x29); WriteData(0x2E);
    WriteData(0x30); WriteData(0x30); WriteData(0x39); WriteData(0x3F);
    WriteData(0x00); WriteData(0x07); WriteData(0x03); WriteData(0x10);

    // Finally
    WriteCommand(0x29); // Display ON

    sleep_ms(100);

    // --- Clear display to black ---
    uint16_t black[WIDTH * HEIGHT] = {0};
    Present(black);
}


void ST7735::SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    WriteCommand(0x2A); //set start x and end x coords which will be written next
    WriteData16(x0);
    WriteData16(x1);

    WriteCommand(0x2B); //set start y and end y coords
    WriteData16(y0);
    WriteData16(y1);

    WriteCommand(0x2C); //sets that next bytes will be pixel data
}

void ST7735::Present(uint16_t *buffer)
{
    SetAddressWindow(0,0, WIDTH - 1, HEIGHT - 1);
    gpio_put(myDcPin, 1);
    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, (uint8_t*)buffer, WIDTH * HEIGHT * 2);
    gpio_put(myCsPin, 1);
}






// ST7735 Command Cheat Sheet
// Command	Hex	    Name	                    Description / Typical Use
// 0x01	SWRESET	    Software Reset	            Resets internal registers; after power-up or errors. Requires ~150ms delay.
// 0x11	SLPOUT	    Sleep Out	                Wakes the display from sleep mode. Requires ~500ms delay before sending other commands.
// 0x3A	COLMOD	    Interface Pixel Format	    Sets color depth: 0x05 = 16-bit RGB565 (5R-6G-5B).
// 0x36	MADCTL	    Memory Access Control	    Controls rotation, row/column order, RGB/BGR order. Bitmask sets orientation.
// 0x2A	CASET	    Column Address Set	        Define horizontal start (x0) and end (x1) of the pixel window.
// 0x2B	RASET	    Row Address Set	            Define vertical start (y0) and end (y1) of the pixel window.
// 0x2C	RAMWR	    Memory Write	            Send pixel data to the selected address window.
// 0x29	DISPON	    Display ON	                Turns on display output.
// 0x28	DISPOFF	    Display OFF	                Turns off display output.
// 0x13	NORON	    Normal Display Mode On	    Used after sleep out in some init sequences.
// 0x20	INVOFF	    Display Inversion Off	    Optional; controls pixel inversion.
// 0x21	INVON	    Display Inversion On	    Optional; invert colors for effect.
// 0xB1	FRMCTR1	    Frame Rate Control	        Controls refresh rate / front porch / back porch timing.
// 0xB4	DISSET5	    Display Setting	            Enables idle mode / color sequence options.
// 0xC0	PWCTR1	    Power Control 1	            Adjust voltage and contrast.
// 0xC1	PWCTR2	    Power Control 2	            Additional power settings.
// 0xE0	GAMMASET	Positive Gamma Correction	Sets gamma curve for colors.
// 0xE1	GAMMANEG	Negative Gamma Correction	Sets negative gamma curve.