#include "SDCardManager.h"

#include "hw_config.h"
#include "ff.h"
#include "sd_card.h"
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
    // Get SD card handle (from sd_card_spi.c)
    sd_card_t *pSD = sd_get_by_num(0); // 0 = first/only SD card

    // Mount the filesystem (this triggers initialization automatically)
    FRESULT fr = f_mount(&fs, "", 1); // "" = default drive

    m_IsMounted = (fr == FR_OK);
}

SDCardManager::~SDCardManager()
{
    if (m_IsMounted) 
    {
        f_unmount("");
    }
}

File SDCardManager::Open(const char *path, BYTE mode)
{
    FIL fil;
    FRESULT fr = f_open(&fil, path, mode);

    return File(fil, fr == FR_OK);
}

bool SDCardManager::IsMounted() const
{
    return m_IsMounted;
}