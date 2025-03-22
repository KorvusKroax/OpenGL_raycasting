# include <iostream>
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "canvas.h"

// https://hackaday.io/project/6309-vga-graphics-over-spi-and-serial-vgatonic/log/20759-a-tiny-4x6-pixel-font-that-will-fit-on-almost-any-microcontroller-license-mit
void Canvas::drawChar(int x, int y, unsigned char asciiCode, Color color)
{
    int charIndex = asciiCode - 32;
    for (int j = 0; j < 6; j++) {
        unsigned char pixel = 0;
        switch (j - (this->font4x6[charIndex] & 1)) {
            case 0: pixel = (this->font4x6[charIndex] >> 12) & 0xe; break;
            case 1: pixel = (this->font4x6[charIndex] >> 9) & 0xe; break;
            case 2: pixel = ((this->font4x6[charIndex] & 0x0300) >> 6) | (this->font4x6[charIndex] & 0x02); break;
            case 3: pixel = (this->font4x6[charIndex] >> 4) & 0xe; break;
            case 4: pixel = (this->font4x6[charIndex] >> 1) & 0xe; break;
        }

        for (int i = 0; i < 4; i++) {
            if ((pixel >> i) & 1) {
                setPixel(x + 3 - i,  y + 5 - j, color);
            }
        }

    }
}

void Canvas::drawText(int x, int y, const char *text, Color color)
{
    int index = 0;
    while (text[index] != '\0') {
        drawChar(x + index * 4, y, text[index], color);
        index++;
    }
}

void Canvas::drawChar_loadedCharset(int x, int y, unsigned char asciiCode, Color color)
{
    if (!this->charsetLoaded) {
        drawChar(x, y, asciiCode, color);
        return;
    }

    int charIndex = asciiCode - 32;
    for (int j = 0; j < this->charHeight; j++) {
        for (int i = 0; i < this->charWidth; i++) {
            if (this->charset[charIndex][i + j * this->charWidth] & 0xff000000) {
                setPixel(x + i, y + j, color);
            }

            // int pixel = this->charset[charIndex][i + j * this->charWidth];
            // color.setAlpha(int((pixel >> 24) & 0x000000ff));
            // setPixel(x + i, y + j, color);
        }
    }
}

void Canvas::drawText_loadedCharset(int x, int y, const char *text, Color color)
{
    if (!this->charsetLoaded) {
        drawText(x, y, text, color);
        return;
    }

    int index = 0;
    while (text[index] != '\0') {
        drawChar_loadedCharset(x + index * this->charWidth, y, text[index], color);
        index++;
    }
}

void Canvas::loadCharset(const char *fileName, int fontSheetGridWidth, int fontSheetGridHeight, int charWidth, int charHeight)
{
    int w, h, c;
    unsigned char *fontSheet = stbi_load(fileName, &w, &h, &c, 0);

    this->charWidth = charWidth;
    this->charHeight = charHeight;
    this->charCount = fontSheetGridWidth * fontSheetGridHeight;
    this->charset = new int*[this->charCount];
    for (int row = 0; row < fontSheetGridHeight; row++) {
        for (int col = 0; col < fontSheetGridWidth; col++) {
            int charIndex = col + row * fontSheetGridWidth;
            this->charset[charIndex] = new int[this->charWidth * this->charHeight];
            int charPos = (col * this->charWidth * c) + row * this->charHeight * (w * c);
            for (int y = 0; y < this->charHeight; y++) {
                for (int x = 0; x < this->charWidth; x++) {
                    this->charset[charIndex][x + (this->charHeight - 1 - y) * this->charWidth] =
                        fontSheet[charPos + (x * c + 0) + y * (w * c)] |
                        fontSheet[charPos + (x * c + 1) + y * (w * c)] << 8 |
                        fontSheet[charPos + (x * c + 2) + y * (w * c)] << 16 |
                        fontSheet[charPos + (x * c + 3) + y * (w * c)] << 24
                    ;
                }
            }
        }
    }
    this->charsetLoaded = true;
}
