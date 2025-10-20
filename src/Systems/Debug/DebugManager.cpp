#include "DebugManager.h"
#include "Entities/Entity.h"
#include "Entities/Common/UI/UITextObject.h"

DebugManager &DebugManager::GetInstance()
{
    static DebugManager instance;
    return instance;
}

void DebugManager::ClearLogs()
{
    messages.clear();
}

void DebugManager::Log(std::string msg)
{
    messages.push_back(msg);
}

void DebugManager::PrintLogsOnScreen(std::vector<std::unique_ptr<Entity>> &uiElements, uint16_t color)
{
    for (int i = 0; i < messages.size(); i++)
    {
        auto textObj = std::make_unique<UITextObject>(Vector2(2, 2 + i * 10), messages[i].c_str(), color, -1);
        uiElements.emplace_back(std::move(textObj));
    }
}
