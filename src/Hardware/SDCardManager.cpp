#include "SDCardManager.h"

#include "Systems/Debug/DebugManager.h"

#include "hw_config.h"
#include "ff.h"
#include "f_util.h"

File::File(FIL &fil, bool success)
 : fil(fil), isValid(success)
{

}

File::~File()
{
    Close();
}

bool File::IsValid()
{
    return isValid;
}

std::string File::Read()
{
    std::string content;
    char buffer[256];
    UINT bytesRead;

    while (true) //read until there is nothing to read anymore
    {
        FRESULT res = f_read(&fil, buffer, sizeof(buffer), &bytesRead);

        if (res != FR_OK || bytesRead == 0)
            break;

        content.append(buffer, bytesRead);
    }

    return content;
}

bool File::Write(std::string text)
{
    FRESULT result;
    UINT bytesWritten;

    result = f_write(&fil, text.c_str(), text.length(), &bytesWritten);

    return (result == FR_OK) && (bytesWritten == text.length());
}

void File::Close()
{
    f_close(&fil);
}



SDCardManager::SDCardManager()
{

}

SDCardManager::~SDCardManager()
{
    if (m_IsMounted) 
    {
        f_unmount("");
    }
}

bool SDCardManager::Initialize()
{    
    spi_init(spi0, 400000); // 400 kHz for init

    // Initialize all cards defined in hw_config.c
    if (!sd_init_driver()) 
    {
        DebugManager::GetInstance().Log("sd init failed");
        fr = FR_NOT_READY;
        return false;
    }
    
    // Get handle to first card
    pSD = sd_get_by_num(0);

    // Try to detect and initialize the card manually
    if (!sd_card_detect(pSD)) {
        DebugManager::GetInstance().Log("No SD card detected on bus");
        fr = FR_NOT_READY;
        return false;
    }

    DSTATUS stat = pSD->init(pSD);
    DebugManager::GetInstance().Log("stat: " + std::to_string((int)stat));
    if (stat & STA_NOINIT) {
        DebugManager::GetInstance().Log("sd card init failed");
        fr = FR_NOT_READY;
        return false;
    }
    DebugManager::GetInstance().Log("sd card init succeeded");

    
    // Mount filesystem (FatFS automatically calls disk_initialize)
    fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) 
    {
        DebugManager::GetInstance().Log("f_mount failed");
        return false;
    }

    m_IsMounted = true;
    return true;
}

File SDCardManager::Open(const char *path, BYTE mode)
{
    FIL fil;
    fr = f_open(&fil, path, mode);

    return File(fil, fr == FR_OK);
}

bool SDCardManager::IsMounted() const
{
    return m_IsMounted;
}

int SDCardManager::Result() const
{
    return (int)fr;
}
