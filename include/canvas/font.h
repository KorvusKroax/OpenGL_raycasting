#pragma once

struct Font
{
    unsigned int width, height, count;
    int **charset;

    Font(unsigned int width, unsigned int height, unsigned int count)
    {
        this->width = width;
        this->height = height;
        this->count = count;

        this->charset = new int*[this->count];
    }

    ~Font()
    {
        for (int i = 0; i < this->count; i++) {
            delete[] charset[i];
        }
        delete[] charset;
    }
};
