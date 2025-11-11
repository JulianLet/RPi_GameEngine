#pragma once

#include "ff.h"
#include "sd_card.h"
#include <string>

class File
{
    FIL fil;      // FatF File object
    bool isValid;

    public:
    File(FIL& fil, bool success);
    ~File();

    bool IsValid();
    std::string Read();
    UINT Read(uint8_t* buffer, UINT length);
    bool Seek(int offset);
    bool Write(std::string text);
    void Close();
};

class SDCardManager
{
    sd_card_t *pSD;
    FATFS fs;     // FatF Filesystem object
    FRESULT fr = FR_NO_FILE;
    bool m_IsMounted = false;

    public:
    SDCardManager();
    ~SDCardManager();

    bool Initialize();
    File Open(const char* path, BYTE mode);
    bool IsMounted() const;
    int Result() const;
};