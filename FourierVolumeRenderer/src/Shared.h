#ifndef SHARED_H
#define SHARED_H

#define BYTE 1

struct volume
{
    char* ptrVol_char;
    float* ptrVol_float;
    double* ptrVol_double;

    int sizeX;
    int sizeY;
    int sizeZ;

    int volSizeBytes;
};










#endif // SHARED_H
