#pragma once

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
