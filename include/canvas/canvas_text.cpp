# include <iostream>

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

void Canvas::drawChar(int x, int y, unsigned char asciiCode, Color color, Color background)
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
            setPixel(x + 3 - i,  y + 5 - j, (pixel >> i) & 1 ? color : background);
        }
    }
}

void Canvas::drawChar(int x, int y, unsigned char asciiCode, Color color, Font *font)
{
    int charIndex = asciiCode - 32;
    for (int j = 0; j < font->height; j++) {
        for (int i = 0; i < font->width; i++) {
            if (font->charset[charIndex][i + j * font->width] & 0xff000000) {
                setPixel(x + i, y + j, color);
            }
        }
    }
}

void Canvas::drawChar(int x, int y, unsigned char asciiCode, Color color, Color background, Font *font)
{
    int charIndex = asciiCode - 32;
    for (int j = 0; j < font->height; j++) {
        for (int i = 0; i < font->width; i++) {
            setPixel(x + i, y + j, font->charset[charIndex][i + j * font->width] & 0xff000000 ? color : background);
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

void Canvas::drawText(int x, int y, const char *text, Color color, Color background)
{
    int index = 0;
    while (text[index] != '\0') {
        drawChar(x + index * 4, y, text[index], color, background);
        index++;
    }
}

void Canvas::drawText(int x, int y, const char *text, Color color, Font *font)
{
    int index = 0;
    while (text[index] != '\0') {
        drawChar(x + index * font->width, y, text[index], color, font);
        index++;
    }
}

void Canvas::drawText(int x, int y, const char *text, Color color, Color background, Font *font)
{
    int index = 0;
    while (text[index] != '\0') {
        drawChar(x + index * font->width, y, text[index], color, background, font);
        index++;
    }
}
