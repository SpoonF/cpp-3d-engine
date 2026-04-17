#pragma once

#include <map>

class imageBMP {
public:
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int size;
    unsigned char* data;
    imageBMP() = default;
    imageBMP(const char* filename);

    static std::map<int, imageBMP*> instances;
    static imageBMP* getInstance(const char* filename, int id);
    static imageBMP* getInstance(int id);
};