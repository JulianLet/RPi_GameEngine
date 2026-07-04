#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Common.hpp"
#include "Entities/Entity.h"

class Renderer;

constexpr int MAX_LOGS = 10;

class DebugManager
{
private:
    DebugManager() = default;

    struct LogData
    {
        char msg[64];
        uint16_t color;
    };

    LogData logs[MAX_LOGS];
    uint8_t logIndex = 0;
    uint8_t logCount = 0;


public:
    bool printLogs = false;
    static DebugManager& GetInstance();

    void ClearLogs();
    void Log(const char* msg, uint16_t color = Color::RED);
    void Render(Renderer& renderer);
};