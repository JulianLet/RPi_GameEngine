#pragma once
#include <cstdint>
#include <cstddef>
#include "SDCardModule.h"

class File
{
private:
    SDCardModule* module;
    uint32_t position;
    uint32_t size;
    uint32_t startCluster;
    uint32_t sectorsPerCluster;
    uint32_t firstDataSector;
    uint32_t fatStartSector;

public:
    File();
    File(SDCardModule* mod, uint32_t startCluster, uint32_t size,
         uint32_t spc, uint32_t dataStart, uint32_t fatStart);
    ~File();

    bool IsValid() const;
    size_t Read(uint8_t* buffer, size_t bytes);
    bool Seek(uint32_t pos);
    uint32_t Position() const;
    uint32_t Size() const;
    void Close();
};

class SDCardManager
{
private:
    SDCardModule& module;

public:
    SDCardManager(SDCardModule& mod);
    ~SDCardManager();

    File Open(const char* filename);
    void Close(File& file);
    size_t Read(File& file, uint8_t* buffer, size_t bytes);
    bool Seek(File& file, uint32_t pos);
};
