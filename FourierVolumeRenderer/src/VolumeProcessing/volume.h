#ifndef VOLUME_H
#define VOLUME_H

#include "shared.h"
namespace Volume
{
    char*** allocCubeVolume_char(int size_X, int size_Y, int size_Z);
    float*** allocCubeVolume_float(int size_X, int size_Y, int size_Z);
    void packFlatVolume(char* flatVolume, char*** cubeVolume, volDim* iVolDim);
    void packCubeVolume(char*** cubeVolume, char* flatVolume, volDim* iVolDim);
    void extractSubVolume(char*** originalCubeVol, char*** finalCubeVol, subVolDim* iSubVolDim);
    char* extractFinalVolume(char* originalFlatVol, volDim* originalDim, subVolDim* iSubVol);
    float* createFloatVolume(char* flatVol_char, volDim* iVolDim);
}

#endif // VOLUME_H
