#pragma once

#include "point.h"
#include "wall.h"
#include "sector.h"
#include "texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct World
{
    int pointCount;
    Point *points;

    int wallCount;
    Wall *walls;

    int sectorCount;
    Sector *sectors;

    int textureCount;
    Texture *textures;

    World() { }

    ~World()
    {
        delete [] this->points;
        delete [] this->walls;
        delete [] this->sectors;
        delete [] this->textures;
    }

    void set()
    {
        pointCount = 11;
        points = new Point[pointCount] { Point(-64, 100), Point(64, 100), Point(200, 50), Point(0, -600), Point(-200, -50), Point(200, 300), Point(0, 600), Point(-200, 300), Point(-200, 427), Point(-64, 224), Point(64, 224) };

        wallCount = 15;
        walls = new Wall[wallCount] { Wall(0, 1, 6, 2), Wall(1, 2, 1, -1), Wall(2, 3, 1, -1), Wall(3, 4, 1, -1), Wall(4, 0, 1, -1), Wall(10, 9, 6, 2), Wall(9, 7, 2, -1), Wall(7, 8, 0, -1), Wall(8, 6, 2, -1), Wall(6, 5, 2, -1), Wall(5, 10, 2, -1), Wall(1, 0, 6, 0), Wall(0, 9, 6, -1), Wall(9, 10, 6, 1), Wall(10, 1, 6, -1) };

        sectorCount = 3;
        sectors = new Sector[sectorCount] { Sector(5, new int[5] {0, 1, 2, 3, 4}, -50, 228, 3, 5), Sector(6, new int[6] {5, 6, 7, 8, 9, 10}, 0, 128, 3, 5), Sector(4, new int[4] {11, 12, 13, 14}, 20,  98, 4, 4) };

        textureCount = 7;
        textures = new Texture[textureCount] {
            Texture("textures/161.png"),
            Texture("textures/211.png"),
            Texture("textures/212.png"),
            Texture("textures/117.png"),
            Texture("textures/373.png"),
            Texture("textures/091.png"),
            Texture("textures/547.png")
        };
    }

    void save(const char *filename)
    {
        std::ofstream file_obj;
        file_obj.open(filename, std::ios::out | std::ios::binary);

        file_obj.write((char*)&this->pointCount, sizeof(int));
        for (int i = 0; i < this->pointCount; i++) {
            int *serializedPoint;
            int size = points[i].serialize(&serializedPoint);
            file_obj.write((char*)&size, sizeof(int));
            file_obj.write((char*)serializedPoint, sizeof(int) * size);
        }

        file_obj.write((char*)&this->wallCount, sizeof(int));
        for (int i = 0; i < this->wallCount; i++) {
            int *serializedWall;
            int size = walls[i].serialize(&serializedWall);
            file_obj.write((char*)&size, sizeof(int));
            file_obj.write((char*)serializedWall, sizeof(int) * size);
        }

        file_obj.write((char*)&this->sectorCount, sizeof(int));
        for (int i = 0; i < this->sectorCount; i++) {
            int *serializedSector;
            int size = sectors[i].serialize(&serializedSector);
            file_obj.write((char*)&size, sizeof(int));
            file_obj.write((char*)serializedSector, sizeof(int) * size);
        }

        file_obj.write((char*)&this->textureCount, sizeof(int));
        for (int i = 0; i < this->textureCount; i++) {
            int *serializedTexture;
            int size = textures[i].serialize(&serializedTexture);
            file_obj.write((char*)&size, sizeof(int));
            file_obj.write((char*)serializedTexture, sizeof(int) * size);
        }

        file_obj.close();
    }

    void load(const char *filename)
    {
        std::ifstream file_obj;
        file_obj.open(filename, std::ios::in | std::ios::binary);

        file_obj.read((char*)&this->pointCount, sizeof(int));
        this->points = new Point[this->pointCount];
        for (int i = 0; i < this->pointCount; i++) {
            int size;
            file_obj.read((char*)&size, sizeof(int));
            int *serializedPoint = new int[size];
            file_obj.read((char*)serializedPoint, sizeof(int) * size);
            this->points[i] = Point(&serializedPoint);
        }

        file_obj.read((char*)&this->wallCount, sizeof(int));
        this->walls = new Wall[this->wallCount];
        for (int i = 0; i < this->wallCount; i++) {
            int size;
            file_obj.read((char*)&size, sizeof(int));
            int *serializedWall = new int[size];
            file_obj.read((char*)serializedWall, sizeof(int) * size);
            this->walls[i] = Wall(&serializedWall);
        }

        file_obj.read((char*)&this->sectorCount, sizeof(int));
        this->sectors = new Sector[this->sectorCount];
        for (int i = 0; i < this->sectorCount; i++) {
            int size;
            file_obj.read((char*)&size, sizeof(int));
            int *serializedSector = new int[size];
            file_obj.read((char*)serializedSector, sizeof(int) * size);
            this->sectors[i] = Sector(&serializedSector);
        }

        file_obj.read((char*)&this->textureCount, sizeof(int));
        this->textures = new Texture[this->textureCount];
        for (int i = 0; i < this->textureCount; i++) {
            int size;
            file_obj.read((char*)&size, sizeof(int));
            int *serializedtexture = new int[size];
            file_obj.read((char*)serializedtexture, sizeof(int) * size);
            this->textures[i] = Texture(&serializedtexture);
        }

        file_obj.close();
    }
};
