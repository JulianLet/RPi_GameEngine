#pragma once
#include "Hardware/DisplayST7735.h"

#include <vector>

class Sprite;

class Renderer
{
    private:
    ST7735& myDisplay;
    uint16_t myFramebuffer[ST7735::WIDTH * ST7735::HEIGHT]; //set RBG565 framebuffer
    uint16_t transparentColor = 0xFFE0; 

    public:
    void SetPixel(int x, int y, uint16_t color); //basically draw pixel

    Renderer(ST7735& display);
    ~Renderer() = default;

    void Clear(uint16_t = 0x0000); //default black
    void Display();

    void DrawRectangle(int x, int y, int w, int h, uint16_t color, bool filled = true);
    void DrawCircle(int x, int y, int radius, uint16_t color, bool filled = true);

    void DrawChar(int x, int y, char c, uint16_t color);
    void DrawText(int x, int y, const char* text, uint16_t color);

    void DrawSprite(int x, int y, int width, int height, const std::vector<uint16_t>& pixels, float zoom); 
};