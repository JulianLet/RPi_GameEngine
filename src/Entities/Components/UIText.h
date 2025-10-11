#pragma once

#include "Entities/Component.h"
#include "Globals.h"

struct UITextComponent : public Component
{
    const char* text;
    uint16_t textColor;

    UITextComponent(const char* text, uint16_t color) : text(text), textColor(color) {};
    ~UITextComponent() override;

    void Reset() override;
};