#include <color.h>

Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
    this->value = r | (g << 8) | (b << 16) | (a << 24);
}

Color::Color(unsigned int value, unsigned int a)
{
    this->value = (value & 0x00ffffff) | (a << 24);
}

int Color::getRed() { return this->value & 0x000000ff; }
int Color::getGreen() { return (this->value >> 8) & 0x000000ff; }
int Color::getBlue() { return (this->value >> 16) & 0x000000ff; }
int Color::getAlpha() { return (this->value >> 24) & 0x000000ff; }
