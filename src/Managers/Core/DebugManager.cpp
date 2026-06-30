#include "DebugManager.h"

#include "Hardware/Renderer.h"

#include <cstring>
#include <stdio.h>

DebugManager& DebugManager::GetInstance()
{
    static DebugManager instance;
    return instance;
}

void DebugManager::ClearLogs()
{
    logIndex = 0;
    logCount = 0;
}

void DebugManager::Log(const char* msg, uint8_t color)
{
    printf("%s\n", msg);

    strncpy(logs[logIndex].msg, msg, sizeof(logs[logIndex].msg) - 1);
    logs[logIndex].msg[sizeof(logs[logIndex].msg) - 1] = '\0';

    logs[logIndex].color = color;

    logIndex = (logIndex + 1) % MAX_LOGS;

    if (logCount < MAX_LOGS) logCount++;
}

void DebugManager::Render(Renderer& renderer)
{
    if (!printLogs) return;

    int y = 2;

    uint8_t start = (logCount == MAX_LOGS) ? logIndex : 0;

    for (uint8_t i = 0; i < logCount; i++)
    {
        uint8_t index = (start + i) % MAX_LOGS;

        const LogData& log = logs[index];

        renderer.DrawText(
            2,
            y,
            log.msg,
            log.color
        );

        y += 10;
    }
}