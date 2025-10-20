#include "SDCardManager.h"
#include <cstring>
#include <strings.h>
#include <vector>
#include <string>
#include <map>
#include <cstdint>
#include <cstdio>

// ------------------------
// Helpers
// ------------------------

// Read 512 bytes into buffer from block LBA
static bool ReadBlockSafe(SDCardModule& mod, uint32_t lba, uint8_t* buffer) {
    return mod.ReadBlock(buffer, lba);
}

// If sector 0 is an MBR, find the first partition's starting LBA (partition table entry at offset 446).
// If sector 0 looks like a FAT boot sector, return 0.
static uint32_t FindBootSectorLBA(SDCardModule& mod) {
    uint8_t sector[512];
    if (!ReadBlockSafe(mod, 0, sector)) return 0;

    // Check for 0x55 0xAA signature (MBR or boot sector both have this signature).
    bool hasSig = (sector[510] == 0x55 && sector[511] == 0xAA);

    // A simple heuristic: if bytesPerSector (offset 11/12) and sectorsPerCluster (offset 13) look sane,
    // treat sector 0 as a boot sector. Otherwise treat as MBR and read partition table.
    uint16_t bytesPerSector = sector[11] | (sector[12] << 8);
    uint8_t sectorsPerCluster = sector[13];

    if (hasSig && bytesPerSector >= 512 && bytesPerSector <= 4096 && sectorsPerCluster != 0) {
        // Looks like a boot sector
        return 0;
    }

    // Otherwise assume MBR: partition entry starts at offset 446 (0x1BE), 16 bytes each, first partition at 446.
    if (!hasSig) return 0; // fallback

    uint32_t partEntryOffset = 446;
    uint32_t startLBA = sector[partEntryOffset + 8] |
                       (sector[partEntryOffset + 9] << 8) |
                       (sector[partEntryOffset + 10] << 16) |
                       (sector[partEntryOffset + 11] << 24);

    // If startLBA == 0 then fallback to 0.
    if (startLBA == 0) return 0;
    return startLBA;
}

// Safely read a 32-bit FAT entry (FAT32). Handles the case where the 4 bytes cross a sector boundary.
static bool ReadFATEntry(SDCardModule& mod, uint32_t fatStartSector, uint32_t cluster, uint32_t &outNext) {
    // FAT32 entries are 4 bytes each
    uint32_t fatOffset = cluster * 4;
    uint32_t sectorIndex = fatStartSector + (fatOffset / 512);
    uint32_t offsetInSector = fatOffset % 512;

    uint8_t sector[512];
    if (!ReadBlockSafe(mod, sectorIndex, sector)) return false;

    // If the 4 bytes are contained, read directly; otherwise read next sector and assemble.
    if (offsetInSector <= 508) {
        // all 4 bytes in this sector
        uint32_t val = sector[offsetInSector] |
                       (sector[offsetInSector + 1] << 8) |
                       (sector[offsetInSector + 2] << 16) |
                       (sector[offsetInSector + 3] << 24);
        outNext = val & 0x0FFFFFFF;
        return true;
    } else {
        // spans boundary: read next sector
        uint8_t nextSector[512];
        if (!ReadBlockSafe(mod, sectorIndex + 1, nextSector)) return false;
        // build the 4 bytes manually
        uint8_t b0 = sector[offsetInSector];
        uint8_t b1 = (offsetInSector + 1 < 512) ? sector[offsetInSector + 1] : nextSector[(offsetInSector + 1) % 512];
        uint8_t b2 = nextSector[(offsetInSector + 2) % 512];
        uint8_t b3 = nextSector[(offsetInSector + 3) % 512];
        uint32_t val = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
        outNext = val & 0x0FFFFFFF;
        return true;
    }
}

// Return next cluster in chain, or 0x0FFFFFFF on error/end
static uint32_t NextCluster(SDCardModule& mod, uint32_t cluster, uint32_t fatStartSector) {
    uint32_t next = 0x0FFFFFFF;
    if (!ReadFATEntry(mod, fatStartSector, cluster, next)) return 0x0FFFFFFF;
    return next;
}

// Split path into components, ignoring leading/trailing slashes
static std::vector<std::string> SplitPath(const char* path) {
    std::vector<std::string> parts;
    const char* s = path;
    while (*s) {
        while (*s == '/' || *s == '\\') s++;
        if (!*s) break;
        const char* e = s;
        while (*e && *e != '/' && *e != '\\') e++;
        parts.emplace_back(s, e - s);
        s = e;
    }
    return parts;
}

// Extract up to 13 UTF-16 characters from an LFN entry and convert to a narrow string:
// NOTE: This simple implementation keeps only low byte (ASCII-range). For full UTF-16 -> UTF-8 conversion,
// you'd add proper decoding; for many configs ASCII is sufficient.
static std::string LFNPartFromEntry(const uint8_t* dir) {
    // name1 (5 chars) at offsets 1..10, name2 (6 chars) at 14..25, name3 (2 chars) at 28..31
    std::string out;
    // helper to fetch low byte and stop at 0x00 0x00
    auto append_char = [&](int loIdx, int hiIdx) {
        // low byte is at loIdx, high byte at hiIdx (loIdx increments by 2)
        uint8_t lo = dir[loIdx];
        uint8_t hi = dir[hiIdx];
        uint16_t u = (uint16_t)lo | ((uint16_t)hi << 8);
        if (u == 0x0000 || u == 0xFFFF) return false; // terminator or unused
        // keep only ASCII-range for now
        out.push_back((char)(u & 0xFF));
        return true;
    };

    // name1
    for (int i = 1; i <= 10; i += 2) {
        if (!append_char(i, i+1)) return out;
    }
    // name2
    for (int i = 14; i <= 25; i += 2) {
        if (!append_char(i, i+1)) return out;
    }
    // name3
    for (int i = 28; i <= 31; i += 2) {
        if (!append_char(i, i+1)) return out;
    }
    return out;
}

// Case-insensitive compare using POSIX strcasecmp semantics for ASCII
static bool CaseInsensitiveEqual(const std::string& a, const std::string& b) {
    return strcasecmp(a.c_str(), b.c_str()) == 0;
}

// ------------------------
// File implementation (unchanged logic, but next-cluster uses ReadFATEntry helper)
// ------------------------

File::File() : module(nullptr), position(0), size(0), startCluster(0),
               sectorsPerCluster(0), firstDataSector(0), fatStartSector(0) {}

File::File(SDCardModule* mod, uint32_t startClus, uint32_t sz,
           uint32_t spc, uint32_t dataStart, uint32_t fatStart)
    : module(mod), position(0), size(sz), startCluster(startClus),
      sectorsPerCluster(spc), firstDataSector(dataStart), fatStartSector(fatStart) {}

File::~File() { Close(); }

bool File::IsValid() const { return module != nullptr; }

size_t File::Read(uint8_t* buffer, size_t bytes) {
    if (!IsValid()) return 0;
    if (position >= size) return 0;
    if (position + bytes > size) bytes = size - position;

    uint8_t sector[512];
    size_t bytesRead = 0;
    uint32_t cluster = startCluster;

    // skip clusters until current position
    uint32_t clusterSizeBytes = 512 * sectorsPerCluster;
    uint32_t skip = position / clusterSizeBytes;
    uint32_t offsetInCluster = position % clusterSizeBytes;

    // advance `cluster` skip times
    for (uint32_t i = 0; i < skip; ++i) {
        uint32_t next = 0;
        if (!ReadFATEntry(*module, fatStartSector, cluster, next)) return 0;
        cluster = next;
        if (cluster >= 0x0FFFFFF8) return 0;
    }

    while (bytes > 0 && cluster < 0x0FFFFFF8) {
        for (uint8_t s = 0; s < sectorsPerCluster; ++s) {
            uint32_t blockAddr = firstDataSector + (cluster - 2) * sectorsPerCluster + s;
            if (!module->ReadBlock(sector, blockAddr)) return bytesRead;

            uint32_t toCopy = (bytes > (512 - offsetInCluster)) ? (512 - offsetInCluster) : bytes;
            memcpy(buffer + bytesRead, sector + offsetInCluster, toCopy);

            bytesRead += toCopy;
            bytes -= toCopy;
            position += toCopy;
            offsetInCluster = 0;

            if (bytes == 0) return bytesRead;
        }

        // move to next cluster
        uint32_t next = 0;
        if (!ReadFATEntry(*module, fatStartSector, cluster, next)) return bytesRead;
        cluster = next;
    }

    return bytesRead;
}

bool File::Seek(uint32_t pos) {
    if (!IsValid() || pos > size) return false;
    position = pos;
    return true;
}

uint32_t File::Position() const { return position; }
uint32_t File::Size() const { return size; }

void File::Close() {
    module = nullptr;
    position = 0;
    size = 0;
    startCluster = 0;
}

// ------------------------
// SDCardManager
// ------------------------

SDCardManager::SDCardManager(SDCardModule& mod) : module(mod) 
{

}

SDCardManager::~SDCardManager() {}

// FindEntry: searches a directory (given its starting cluster) for an entry whose name matches `name`.
// Supports LFN by collecting LFN parts (their order field) and concatenating when the SFN is found.
static bool FindEntry(SDCardModule& mod,
                      const std::string& name,
                      uint32_t startCluster,
                      uint32_t spc,
                      uint32_t firstDataSector,
                      uint32_t fatStart,
                      uint8_t* outEntry)
{
    uint8_t sector[512];

    uint32_t cluster = startCluster;

    // Temporary storage for LFN parts indexed by order (1..n)
    std::map<int, std::string> lfnParts;

    while (cluster < 0x0FFFFFF8) {
        for (uint8_t s = 0; s < spc; ++s) {
            uint32_t blockAddr = firstDataSector + (cluster - 2) * spc + s;
            if (!mod.ReadBlock(sector, blockAddr)) return false;

            for (int e = 0; e < 16; ++e) {
                uint8_t* dir = &sector[e * 32];

                if (dir[0] == 0x00) {
                    // End of directory: no match
                    return false;
                }
                if (dir[0] == 0xE5) {
                    // deleted entry
                    lfnParts.clear();
                    continue;
                }

                uint8_t attr = dir[11];

                if ((attr & 0x0F) == 0x0F) {
                    // LFN entry
                    uint8_t order = dir[0] & 0x1F;
                    std::string part = LFNPartFromEntry(dir);
                    // store by order (LFN entries are in reverse order in the directory)
                    lfnParts[(int)order] = part;
                    continue;
                }

                // SFN entry encountered. Build the long name if present, otherwise use 8.3 name
                std::string entryName;
                if (!lfnParts.empty()) {
                    // concatenate parts in descending order: highest order first down to 1
                    // find highest key
                    int maxOrder = lfnParts.rbegin()->first;
                    for (int k = maxOrder; k >= 1; --k) {
                        auto it = lfnParts.find(k);
                        if (it != lfnParts.end()) entryName += it->second;
                    }
                    // clear for next entry
                    lfnParts.clear();
                } else {
                    // build short 8.3 name
                    char shortName[13] = {0};
                    memcpy(shortName, dir, 8);
                    for (int i = 7; i >= 0 && shortName[i] == ' '; --i) shortName[i] = '\0';
                    if (dir[8] != ' ') {
                        size_t curLen = strlen(shortName);
                        shortName[curLen] = '.';
                        memcpy(shortName + curLen + 1, &dir[8], 3);
                        // trim trailing spaces in ext
                        for (int i = strlen(shortName) - 1; i >= 0 && shortName[i] == ' '; --i) shortName[i] = '\0';
                    }
                    entryName = shortName;
                }

                // Compare case-insensitive
                if (CaseInsensitiveEqual(entryName, name)) {
                    memcpy(outEntry, dir, 32);
                    return true;
                }

                // Continue scanning directory
            }
        }
        // Next cluster in directory
        uint32_t next = NextCluster(mod, cluster, fatStart);
        if (next >= 0x0FFFFFF8) break;
        cluster = next;
    }

    return false;
}

File SDCardManager::Open(const char* path) {
    // Read sector 0 and locate boot sector LBA (handles MBR)
    uint32_t bootLBA = FindBootSectorLBA(module);
    uint8_t sector[512];
    if (!module.ReadBlock(sector, bootLBA)) return File();

    // Parse boot sector
    uint8_t sectorsPerCluster = sector[13];
    uint16_t reservedSectors = sector[14] | (sector[15] << 8);
    uint8_t numFATs = sector[16];
    uint32_t fatSize = sector[36] | (sector[37] << 8) | (sector[38] << 16) | (sector[39] << 24);
    uint32_t rootCluster = sector[44] | (sector[45] << 8) | (sector[46] << 16) | (sector[47] << 24);

    // The FAT and data region are relative to the boot LBA
    uint32_t fatStart = bootLBA + reservedSectors;
    uint32_t firstDataSector = bootLBA + reservedSectors + (uint32_t)numFATs * fatSize;

    // Split path into components
    std::vector<std::string> parts = SplitPath(path);
    if (parts.empty()) return File();

    uint32_t currentCluster = rootCluster; // start at root
    uint8_t dirEntry[32];

    for (size_t i = 0; i < parts.size(); ++i) {
        bool found = FindEntry(module, parts[i], currentCluster, sectorsPerCluster,
                               firstDataSector, fatStart, dirEntry);
        if (!found) return File();

        uint32_t high = dirEntry[20] | (dirEntry[21] << 8);
        uint32_t low  = dirEntry[26] | (dirEntry[27] << 8);
        uint32_t startCluster = (high << 16) | low;

        bool isDir = (dirEntry[11] & 0x10) != 0;

        if (isDir && i < parts.size() - 1) {
            // step into subdirectory
            currentCluster = startCluster;
            if (currentCluster == 0) return File(); // invalid
        } else if (!isDir && i == parts.size() - 1) {
            // found file
            uint32_t fileSize = dirEntry[28] | (dirEntry[29] << 8) |
                                (dirEntry[30] << 16) | (dirEntry[31] << 24);

            return File(&module, startCluster, fileSize, sectorsPerCluster,
                        firstDataSector, fatStart);
        } else {
            // mismatch (file used as folder or folder used as file)
            return File();
        }
    }

    return File();
}

void SDCardManager::Close(File& file) 
{ 
    file.Close(); 
}

size_t SDCardManager::Read(File& file, uint8_t* buffer, size_t bytes) 
{ 
    return file.Read(buffer, bytes); 
}

bool SDCardManager::Seek(File& file, uint32_t pos) 
{ 
    return file.Seek(pos); 
}
