#include "utils/imageBMP.h"

#include <fstream>

imageBMP::imageBMP(const char* filename) {
    FILE* file = fopen(filename, "rb");
    
    if (!file) {
        printf("Изображение не может быть открытоn");
        exit(0);
    }
    
    if( fread(header, 1, 54, file) != 54 ) {
        printf("Некорректный BMP-файлn");
        exit(0);
    }

    if(header[0] != 'B' && header[1] != 'M') {
        printf("Некорректный BMP-файлn");
        exit(0);
    }

    dataPos = *(int*)&(header[0x0A]);
    size = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if(size == 0) size = width * height * 3;
    if(dataPos == 0) dataPos = 54;

    data = new unsigned char [size];
    fread(data, 1, size, file);
    fclose(file);
}