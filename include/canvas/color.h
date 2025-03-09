#pragma once

// EGA colors
#define BLACK           Color(0x00, 0x00, 0x00)
#define BLUE            Color(0x00, 0x00, 0xA8)
#define GREEN           Color(0x00, 0xA8, 0x00)
#define CYAN            Color(0x00, 0xA8, 0xA8)
#define RED             Color(0xA8, 0x00, 0x00)
#define MAGENTA         Color(0xA8, 0x00, 0xA8)
#define ORANGE          Color(0xA8, 0x54, 0x00)
#define GREY            Color(0xA8, 0xA8, 0xA8)
#define DARK_GREY       Color(0x54, 0x54, 0x54)
#define LIGHT_BLUE      Color(0x54, 0x54, 0xFE)
#define LIGHT_GREEN     Color(0x54, 0xFE, 0x54)
#define LIGHT_CYAN      Color(0x54, 0xFE, 0xFE)
#define BRIGHT_RED      Color(0xFE, 0x54, 0x54)
#define BRIGHT_MAGENTA  Color(0xFE, 0x54, 0xFE)
#define YELLOW          Color(0xFE, 0xFE, 0x54)
#define WHITE           Color(0xFE, 0xFE, 0xFE)

struct Color {
    public:
        unsigned int value;

        Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255)
        {
            this->value = r | (g << 8) | (b << 16) | (a << 24);
        }

        Color(unsigned int value = 0, unsigned int a = 255)
        {
            this->value = (value & 0x00ffffff) | (a << 24);
        }

        inline int getRed() { return this->value & 0x000000ff; }
        inline int getGreen() { return (this->value >> 8) & 0x000000ff; }
        inline int getBlue() { return (this->value >> 16) & 0x000000ff; }
        inline int getAlpha() { return (this->value >> 24) & 0x000000ff; }
};
