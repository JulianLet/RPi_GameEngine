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

void SDCardModule::WriteCommand(uint8_t cmd)
{
    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, &cmd, 1);
    gpio_put(myCsPin, 1);
}

void SDCardModule::WriteData(const uint8_t* data, size_t len)
{
    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, data, len);
    gpio_put(myCsPin, 1);
}

uint8_t SDCardModule::ReadByte()
{
    uint8_t val = 0xFF;
    spi_write_read_blocking(spiPort, &val, &val, 1);
    return val;
}

void SDCardModule::ReadData(uint8_t* buffer, size_t len)
{
    memset(buffer, 0xFF, len);
    spi_write_read_blocking(spiPort, buffer, buffer, len);
}

// --- Full initialization sequence ---
bool SDCardModule::Initialize() {
    // --- SPI setup for initialization ---
    spi_init(spiPort, 400 * 1000); // 400 kHz for init
    spi_set_format(spiPort, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    gpio_init(myCsPin);
    gpio_set_dir(myCsPin, true);
    gpio_put(myCsPin, 1); // CS high

    sleep_ms(100);

    // --- 80 dummy clocks ---
    for (int i = 0; i < 10; i++) ReadByte();

    uint8_t r1;
    uint8_t r7[4];

    // --- CMD0: reset ---
    DebugManager::GetInstance().Log("Sending CMD0");
    gpio_put(myCsPin, 0);
    uint8_t cmd0Packet[6] = { CMD0, 0, 0, 0, 0, 0x95 };
    spi_write_blocking(spiPort, cmd0Packet, 6);

    r1 = 0xFF;
    for (int i = 0; i < 100; i++) {  
        r1 = ReadByte();
        if (r1 != 0xFF) break;
    }
    gpio_put(myCsPin, 1);
    ReadByte(); // extra clock

    if (r1 != 0x01) {
        DebugManager::GetInstance().Log("CMD0 failed");
        return false;
    }
    DebugManager::GetInstance().Log("CMD0 OK");

    // --- CMD8: voltage check ---
    DebugManager::GetInstance().Log("Sending CMD8");
    uint8_t cmd8Packet[6] = { CMD8, 0, 0, 0x01, 0xAA, 0x87 };
    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, cmd8Packet, 6);

    r1 = 0xFF;
    for (int i = 0; i < 100; i++) {
        r1 = ReadByte();
        if (r1 != 0xFF) break;
    }
    for (int i = 0; i < 4; i++) r7[i] = ReadByte();
    gpio_put(myCsPin, 1);
    ReadByte(); // extra clock

    bool v2Card = false;
    if (r1 == 0x01 && r7[2] == 0x01 && r7[3] == 0xAA) {
        v2Card = true;
        DebugManager::GetInstance().Log("v2 card detected");
    } else {
        DebugManager::GetInstance().Log("CMD8 failed, treating as v1 card");
    }

    // --- ACMD41 loop ---
    int timeout = 10000;
    r1 = 0xFF;
    uint32_t arg = v2Card ? 0x40000000 : 0;
    int printCounter = 0;

    while (r1 != 0x00 && timeout-- > 0) {
        // CMD55
        SendCommand(CMD55, 0, 0x01, &r1);

        // ACMD41
        SendCommand(ACMD41, arg, 0x01, &r1);

        if (printCounter < 10 && (timeout % (1000) == 0)) {
            DebugManager::GetInstance().Log("ACMD41 R1: " + std::to_string(r1) +
                                            " timeout left: " + std::to_string(timeout));
            printCounter++;
        }

        sleep_ms(1);
    }

    if (r1 != 0x00) {
        DebugManager::GetInstance().Log("ACMD41 failed after timeout");
        return false;
    }

    // --- CMD58: read OCR ---
    DebugManager::GetInstance().Log("Sending CMD58");
    uint8_t cmd58Packet[6] = { CMD58, 0, 0, 0, 0, 0x01 };
    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, cmd58Packet, 6);

    r1 = 0xFF;
    for (int i = 0; i < 100; i++) {
        r1 = ReadByte();
        if (r1 != 0xFF) break;
    }
    for (int i = 0; i < 4; i++) r7[i] = ReadByte();
    gpio_put(myCsPin, 1);
    for (int i = 0; i < 2; i++) ReadByte(); // extra clocks

    isHighCapacity = (r7[0] & 0x40) != 0;
    DebugManager::GetInstance().Log(
        "Initialization complete. High capacity: " + std::string(isHighCapacity ? "yes" : "no")
    );

    // --- Increase SPI speed for normal operation ---
    spi_init(spiPort, 16 * 1000 * 1000);

    return true;
}



// --- SendCommand with response ---
bool SDCardModule::SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t* response)
{
    uint8_t packet[6];
    packet[0] = cmd;
    packet[1] = (arg >> 24) & 0xFF;
    packet[2] = (arg >> 16) & 0xFF;
    packet[3] = (arg >> 8) & 0xFF;
    packet[4] = arg & 0xFF;
    packet[5] = crc;

    gpio_put(myCsPin, 0);
    spi_write_blocking(spiPort, packet, 6);

    uint8_t r = 0xFF;
    for (int i = 0; i < 8; i++) {
        r = ReadByte();
        if (r != 0xFF) break;
    }

    if (response) response[0] = r;

    gpio_put(myCsPin, 1);
    ReadByte(); // final clock
    return (r != 0xFF);
}

bool SDCardModule::ReadBlock(uint8_t* buffer, uint32_t blockAddress)
{
    if (!isHighCapacity)
        blockAddress *= 512;

    if (!SendCommand(CMD17, blockAddress, 0x01))
        return false;

    gpio_put(myCsPin, 0);

    uint8_t token = 0xFF;
    int timeout = 10000;
    do {
        token = ReadByte();
    } while (token == 0xFF && --timeout > 0);

    if (token != 0xFE) {
        gpio_put(myCsPin, 1);
        return false;
    }

    ReadData(buffer, 512);
    ReadByte(); ReadByte(); // discard CRC
    gpio_put(myCsPin, 1);
    return true;
}

bool SDCardModule::WriteBlock(const uint8_t* buffer, uint32_t blockAddress)
{
    if (!isHighCapacity)
        blockAddress *= 512;

    if (!SendCommand(CMD24, blockAddress, 0x01))
        return false;

    gpio_put(myCsPin, 0);
    uint8_t token = 0xFE;
    spi_write_blocking(spiPort, &token, 1);
    spi_write_blocking(spiPort, buffer, 512);

    uint8_t dummyCRC[2] = {0xFF, 0xFF};
    spi_write_blocking(spiPort, dummyCRC, 2);

    uint8_t response = ReadByte();
    if ((response & 0x1F) != 0x05) {
        gpio_put(myCsPin, 1);
        return false;
    }

    int timeout = 50000;
    while (ReadByte() == 0x00 && --timeout > 0);
    gpio_put(myCsPin, 1);
    return (timeout > 0);
}
