#pragma once

#include "Entities/Component.h"
#include "Globals.h"

#include <string>

struct UITextComponent : public Component
{
    std::string text;
    uint16_t textColor;

    UITextComponent(const char* text, uint16_t color) : text(text), textColor(color) {};
    ~UITextComponent() override;

    void Reset() override;
};