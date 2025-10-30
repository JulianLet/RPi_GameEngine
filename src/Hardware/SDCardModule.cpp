#include "SDCardModule.h"
#include "Globals.h"
#include <string>
#include <cstring>

#include "Systems/Debug/DebugManager.h"

#define CMD0  (0x40 | 0)
#define CMD1  (0x40 | 1)
#define CMD8  (0x40 | 8)
#define CMD16 (0x40 | 16)
#define CMD17 (0x40 | 17)
#define CMD24 (0x40 | 24)
#define CMD55 (0x40 | 55)
#define CMD58 (0x40 | 58)
#define ACMD41 (0x40 | 41)


// --- Full initialization sequence ---
bool SDCardModule::Initialize() 
{
    // --- SPI init ---
    spi_init(spiPort, 16*1000*1000);
    spi_set_format(spiPort, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    // --- GPIO setup ---
    gpio_init(myCsPin); gpio_set_dir(myCsPin, GPIO_OUT); gpio_put(myCsPin, 1);

    return true;
}