#pragma once

#include <string>
#include <vector>
#include <memory>

class Entity;

class DebugManager
{
    private:
    DebugManager() {};
    ~DebugManager() = default;
    std::vector<std::string> messages;

    public:
    static DebugManager& GetInstance();
    void ClearLogs();
    void Log(std::string msg);
    void PrintLogsOnScreen(std::vector<std::unique_ptr<Entity>> &uiElements, uint16_t color);
};