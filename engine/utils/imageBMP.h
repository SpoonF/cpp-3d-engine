#ifndef IMAGEBMP_H
#define IMAGEBMP_H

class imageBMP {
    public:
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int width, height;
        unsigned int size;
        unsigned char* data;
        imageBMP(const char* filename);
};

#endif