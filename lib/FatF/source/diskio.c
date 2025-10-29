#include "ff.h"        // FatFs definitions
#include "diskio.h"    // Declarations for FatFs disk I/O
#include "pico/stdlib.h"
#include "lib/pico-extras/src/common/pico_sd_card/include/pico/sd_card.h"

#define DEV_MMC 0       // We only have one storage device: SD card

// -------------------------------
// Drive status
// -------------------------------
DSTATUS disk_status(BYTE pdrv) {
    if (pdrv != DEV_MMC) return STA_NOINIT;
    // You can check card presence or status here if needed
    return 0; // 0 means OK
}

// -------------------------------
// Initialize drive
// -------------------------------
DSTATUS disk_initialize(BYTE pdrv) {
    if (pdrv != DEV_MMC) return STA_NOINIT;

    int rc = sd_init_4pins();
    if (rc != SD_OK) {
        return STA_NOINIT;
    }

    return 0;
}

// -------------------------------
// Read sectors
// -------------------------------
DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != DEV_MMC) return RES_PARERR;

    int rc = sd_readblocks_sync((uint32_t*)buff, sector, count);
    if (rc != SD_OK) {
        return RES_ERROR;
    }

    return RES_OK;
}

// -------------------------------
// Write sectors (if writable)
// -------------------------------
#if FF_FS_READONLY == 0
DRESULT disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != DEV_MMC) return RES_PARERR;

    int rc = sd_writeblocks_async((const uint32_t*)buff, sector, count);
    if (rc != SD_OK) {
        return RES_ERROR;
    }

    // Wait for write completion
    int status;
    while (!sd_write_complete(&status)) {
        tight_loop_contents(); // yield
    }

    return (status == SD_OK) ? RES_OK : RES_ERROR;
}
#endif

// -------------------------------
// Misc control functions
// -------------------------------
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
    if (pdrv != DEV_MMC) return RES_PARERR;

    switch (cmd) {
    case CTRL_SYNC:
        // Ensure all writes done
        return RES_OK;

    case GET_SECTOR_SIZE:
        *(WORD*)buff = SD_SECTOR_SIZE;
        return RES_OK;

    case GET_BLOCK_SIZE:
        *(DWORD*)buff = 1;
        return RES_OK;

    case GET_SECTOR_COUNT:
        // You can hardcode or read from SD card info if available
        *(DWORD*)buff = 32768; // e.g., 16 MB card
        return RES_OK;
    }

    return RES_PARERR;
}
