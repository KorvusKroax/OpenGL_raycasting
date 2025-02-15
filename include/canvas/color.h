#pragma once

struct Color {
    public:
        unsigned int value;

        Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a = 255);
        Color(unsigned int value = 0, unsigned int a = 255);

        int getRed();
        int getGreen();
        int getBlue();
        int getAlpha();
};
