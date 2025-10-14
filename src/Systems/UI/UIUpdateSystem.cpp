#include "UIUpdateSystem.h"

#include "Hardware/Input.h"

#include "Entities/Entity.h"
#include "Entities/Components/Core/TimerComponent.h"
#include "Entities/Components/UI/UITextComponent.h"
#include "Entities/Components/UI/UIButtonComponent.h"
#include "Entities/Components/Render/RectangleComponent.h"

void UIUpdateSystem::Update(const std::vector<std::unique_ptr<Entity>> &entities, Input& input, GameManager& gameManager)
{
    std::vector<Entity*> buttons;
    buttons.reserve(entities.size());

    for (auto& entity : entities)
    {
        auto* timer = entity->GetComponent<TimerComponent>();
        auto* text = entity->GetComponent<UITextComponent>();

        if (timer && text)
        {
            text->text = std::to_string(timer->currentTime);
            continue;
        }

        auto* btn = entity->GetComponent<UIButtonComponent>();

        if (btn)
        {
            buttons.push_back(entity.get());
        }
    }

    if (buttons.empty()) return;

    //selections
    if (input.GetKey(KEYCODE::UP).pressed) currentSelected--;
    if (input.GetKey(KEYCODE::DOWN).pressed) currentSelected++;

    //wrap selection around
    if (currentSelected < 0) currentSelected = (int)buttons.size() - 1;
    if (currentSelected >= (int)buttons.size()) currentSelected = 0;

    //update visuals
    for (int i = 0; i < buttons.size(); i++)
    {
        auto* rec = buttons[i]->GetComponent<RectangleComponent>();
        auto* btn = buttons[i]->GetComponent<UIButtonComponent>();

        if (!rec || !btn) continue;

        rec->currentColor = i == currentSelected ? btn->selectedColor : btn->defaultColor;
    }

    if (input.GetKey(KEYCODE::A).pressed)
    {
        auto* btn = buttons[currentSelected]->GetComponent<UIButtonComponent>();
        if (btn->OnClicked) 
        {
            btn->OnClicked(gameManager);
            return;
        }
    }
}