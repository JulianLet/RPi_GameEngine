#include "DebugManager.h"

#include "Hardware/Renderer.h"
#include "Entities/Common/UI/UITextObject.h"

#include "Entities/Components/Core/TransformComponent.h"
#include "Entities/Components/UI/UITextComponent.h"

DebugManager &DebugManager::GetInstance()
{
    static DebugManager instance;
    return instance;
}

void DebugManager::ClearLogs()
{
    textElements.clear();
}

void DebugManager::Log(std::string msg, uint16_t color)
{
    int currentLogs = textElements.size();

    if (currentLogs >= MAX_LOGS)
    {
        //remove oldest
        textElements.erase(textElements.begin());
        currentLogs = textElements.size();

        //scroll others to top of screen
        for (int i = 0; i < currentLogs; i++)
        {
            auto* transform = textElements.at(i).get()->GetComponent<TransformComponent>();
            transform->currentPosition = Vector2(2, 2 + i*10);
        }
    }

    auto textObj = std::make_unique<UITextObject>(Vector2(2, 2 + currentLogs * 10), msg.c_str(), color, -1);
    textElements.emplace_back(std::move(textObj));
}

void DebugManager::Render(Renderer &renderer)
{
    if (!printLogs) return;

    for (auto& entity : textElements)
    {
        auto* transform = entity->GetComponent<TransformComponent>();
        if (!transform) continue;


        auto* text = entity->GetComponent<UITextComponent>();
        if (text)
        {
            renderer.DrawText((int)transform->currentPosition.x, (int)transform->currentPosition.y, text->text.c_str(), text->textColor);
            continue;
        }
    }
}
