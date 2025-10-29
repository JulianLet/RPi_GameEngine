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


// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "ff.h"
// #include "pico_sd_card.h"

// FATFS fs;     // Filesystem object
// FIL fil;      // File object
// FRESULT fr;   // Result code
// char buffer[256];

// int main() {
//     stdio_init_all();
//     sleep_ms(2000); // Give USB time to connect

//     printf("=== SD Card + FatFs Test ===\n");

//     // 1. Initialize SD card
//     int sd_result = sd_init_4pins();  // Or sd_init_1pin() if that's your wiring
//     if (sd_result != SD_OK) {
//         printf("SD init failed! (%d)\n", sd_result);
//         while (true) tight_loop_contents();
//     }
//     printf("SD initialized successfully.\n");

//     // 2. Mount filesystem
//     fr = f_mount(&fs, "", 1);
//     if (fr != FR_OK) {
//         printf("Mount failed: %d\n", fr);
//         while (true) tight_loop_contents();
//     }
//     printf("Filesystem mounted.\n");

//     // 3. Open and read log file
//     fr = f_open(&fil, "test/log.txt", FA_READ);
//     if (fr == FR_OK) {
//         printf("Opened test/log.txt successfully.\n");

//         UINT bytesRead;
//         fr = f_read(&fil, buffer, sizeof(buffer) - 1, &bytesRead);
//         if (fr == FR_OK) {
//             buffer[bytesRead] = '\0';  // Null-terminate
//             printf("File content:\n%s\n", buffer);
//         } else {
//             printf("Read failed: %d\n", fr);
//         }

//         f_close(&fil);
//     } else {
//         printf("Could not open test/log.txt (error %d)\n", fr);
//     }

//     // 4. Write to another file
//     fr = f_open(&fil, "print.txt", FA_WRITE | FA_CREATE_ALWAYS);
//     if (fr == FR_OK) {
//         UINT bytesWritten;
//         fr = f_write(&fil, buffer, strlen(buffer), &bytesWritten);
//         if (fr == FR_OK && bytesWritten > 0) {
//             printf("Copied content to print.txt (%u bytes)\n", bytesWritten);
//         } else {
//             printf("Write failed: %d\n", fr);
//         }
//         f_close(&fil);
//     } else {
//         printf("Could not open print.txt for writing (error %d)\n", fr);
//     }

//     // 5. Done
//     f_unmount("");
//     printf("Done.\n");

//     while (true) tight_loop_contents();
// }
