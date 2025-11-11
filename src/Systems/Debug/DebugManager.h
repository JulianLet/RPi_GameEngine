#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include "Common.hpp"
#include "Entities/Entity.h"

class Renderer;

class DebugManager
{
    private:
    DebugManager() {};
    ~DebugManager() = default;

    const int MAX_LOGS = 8;
    std::vector<std::unique_ptr<Entity>> textElements;
    
    public:
    bool printLogs = false;
    
    static DebugManager& GetInstance();
    void ClearLogs();
    void Log(std::string msg, uint16_t color = Color::RED);
    void Render(Renderer& renderer);
};