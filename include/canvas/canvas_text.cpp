// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <canvas.h>

#include <iostream>

void Canvas::loadCharset(const char *fileName, int charWidth, int charHeight)
{
    int w, h, c;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *fontSheet = stbi_load(fileName, &w, &h, &c, 0);

    this->charWidth = charWidth;
    this->charHeight = charHeight;

    int colCount = w / this->charWidth;
    int rowCount = h / this->charHeight;

    this->charCount = rowCount * colCount;
    this->charset = new int*[this->charCount];

    for (int row = 0; row < rowCount; row++) {
        for (int col = 0; col < colCount; col++) {
            int charIndex = col + row * colCount;
            this->charset[charIndex] = new int[this->charWidth * this->charHeight];
            int charPos = (col * this->charWidth * c) + (rowCount - 1 - row) * (w * this->charWidth * c);
            for (int y = 0; y < this->charHeight; y++) {
                for (int x = 0; x < this->charWidth; x++) {
                    this->charset[charIndex][x + y * this->charWidth] =
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

void Canvas::drawChar(int x, int y, int asciiCode, Color color)
{
    if (!this->charsetLoaded) {
        std::cout << "Charset not loaded" << std::endl;
        return;
    }

    int charIndex = asciiCode - 32;
    for (int j = 0; j < this->charHeight; j++) {
        for (int i = 0; i < this->charWidth; i++) {
            int colorValue = this->charset[charIndex][i + j * this->charWidth];
            if (colorValue != 0) {
                // pixels[(10 + (col * charWidth) + i) + (75 + (row * charHeight) +  j) * this->width] = colorValue;
                setPixel(x + i, y + j, color);
            }
        }
    }
}

void Canvas::drawText(int x, int y, const char *text, Color color)
{
    if (!this->charsetLoaded) {
        std::cout << "Charset not loaded" << std::endl;
        return;
    }

    int i = 0;
    while (text[i] != '\0') {
        drawChar(x, y, text[i++], color);
        x += this->charWidth;
    }
}
