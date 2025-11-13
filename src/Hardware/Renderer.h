#pragma once
#include "Hardware/DisplayST7735.h"

#include <vector>

class Sprite;

class Renderer
{
    private:
    ST7735& myDisplay;
    uint8_t myFramebuffer[ST7735::WIDTH * ST7735::HEIGHT]; //set RBG565 framebuffer
    uint8_t transparentColor = 0xC7; 

    public:
    void SetPixel(int x, int y, uint8_t color); //basically draw pixel

    Renderer(ST7735& display);
    ~Renderer() = default;

    void Clear(uint8_t = 0x0000); //default black
    void Display();

    void DrawRectangle(int x, int y, int w, int h, uint8_t color, bool filled = true);
    void DrawCircle(int x, int y, int radius, uint8_t color, bool filled = true);

    void DrawChar(int x, int y, char c, uint8_t color);
    void DrawText(int x, int y, const char* text, uint8_t color);

    void DrawSprite(int x, int y, int width, int height, const std::vector<uint8_t>& pixels, float zoom); 
};