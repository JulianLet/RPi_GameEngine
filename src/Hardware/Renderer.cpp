#include "Hardware/Renderer.h"
#include "Font.h"

void Renderer::SetPixel(int x, int y, uint16_t color)
{
    if (x < 0 || x >= ST7735::WIDTH || y < 0 || y >= ST7735::HEIGHT) return;
    myFramebuffer[ y * ST7735::WIDTH + x] = color;
}

Renderer::Renderer(ST7735 &display)
    : myDisplay(display)
{
    Clear();
}

void Renderer::Clear(uint16_t color)
{
    for (int i = 0; i < ST7735::WIDTH * ST7735::HEIGHT; i++)
    {
        myFramebuffer[i] = color;
    }
}

void Renderer::Display()
{
    myDisplay.Present(myFramebuffer);
}

void Renderer::DrawRectangle(int x, int y, int w, int h, uint16_t color, bool filled)
{
    if (filled)
    {
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                SetPixel(x + i, y + j, color);
            }
        }
    }
    else
    {
        for (int i = 0; i < w; i++)
        {
            SetPixel(x + i, y, color);
            SetPixel(x + i, y + h, color); //draw both horizontal lines
        }
        
        for (int i = 0; i < h; i++)
        {
            SetPixel(x, y + i, color);
            SetPixel(x + w, y + i, color); //draw both vertical lines
        }
    }
}

void Renderer::DrawCircle(int x, int y, int radius, uint16_t color, bool filled)
{
    float radiusSqr = radius * radius;

    for (int i = x - radius; i < x + radius; i++)
    {
        float dx = x - i;

        for (int j = y - radius; j < y + radius; j++)
        {
            float dy = y - j;
            float distanceSqr = dx*dx + dy* dy;

            if (distanceSqr > radiusSqr) continue;
            if (!filled && distanceSqr > radiusSqr + 1) continue; //this might be wonky

            SetPixel(i, j, color);
        }
    }
}

void Renderer::DrawChar(int x, int y, char c, uint16_t color)
{
    const uint8_t* bitmap;
    
    if(c < 32 || c > 126) bitmap = font_unknown;
    
    bitmap = font5x7_ascii[c - 32];
    int yOffset = (c=='g'||c=='j'||c=='p'||c=='q'||c=='y') ? 1 : 0;

    for(int col=0; col<5; col++)
    {
        uint8_t line = bitmap[col];
        for(int row=0; row<7; row++)
        {
            if(line & (1<<row)) SetPixel(x+col, y+row+yOffset, color);
        }
    }
}


void Renderer::DrawText(int x, int y, const char *text, uint16_t color)
{
    int cursorX = x;
    int charWidth = 5;

    for (int i = 0; text[i] != '\0'; i++)
    {
        DrawChar(cursorX, y, text[i], color);
        cursorX += charWidth + 1;
    }
}

void Renderer::DrawSprite(int x, int y, int width, int height, const std::vector<uint16_t>& pixels, float zoom)
{
    int scaledWidth  = (int)(width  * zoom);
    int scaledHeight = (int)(height * zoom);

    for (int dy = 0; dy < scaledHeight; ++dy)
    {
        // Map destination Y back to source sprite Y
        int srcY = (int)(dy / zoom);
        if (srcY >= height) srcY = height - 1;

        for (int dx = 0; dx < scaledWidth; ++dx)
        {
            // Map destination X back to source sprite X
            int srcX = (int)(dx / zoom);
            if (srcX >= width) srcX = width - 1;

            uint16_t color = pixels[srcY * width + srcX];

            if (color == transparentColor) continue;

            // Draw at screen position
            SetPixel(x + dx, y + dy, color);
        }
    }
}
