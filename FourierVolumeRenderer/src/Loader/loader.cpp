#include "loader.h"
#include "shared.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

volDim* Loader::loadHeader(char *path)
{
    // Allocating structure
    volDim* iVolDim = (volDim*) malloc (sizeof(volDim));

    // getting the header file
    char hdrFile[1000];
    sprintf(hdrFile, "%s.hdr", path);

    // Input file stream
    std::ifstream ifile;

    // Open file
    ifile.open(hdrFile, std::ios::in);

    // Double checking for the existance of the header file
    if (ifile.fail())
    {
        printf("Error opening the heder file %s \n", hdrFile);
        exit(0);
    }

    // Read the dimensions in XYZ order
    ifile >> iVolDim->size_X;
    ifile >> iVolDim->size_Y;
    ifile >> iVolDim->size_Z;

    // Close the innput stream
    ifile.close();

    return iVolDim;
}

volume* Loader::loadVolume(char* path)
{
    // Getting the volume file
    char volFile[1000];
    sprintf(volFile, "%s.img", path);

    // Checking for the existance of the volume file
    if (!volFile)
    {
        printf("Error finding the raw volume file %s \n", volFile);
        exit(0);
    }

    // Allocating volume structre
    volume* iVolume = (volume*) malloc (sizeof(volume));

    // Read the header file to get volume dimensions
    volDim* iVolDim = loadHeader(path);
    iVolume->sizeX = iVolDim->size_X;
    iVolume->sizeY = iVolDim->size_Y;
    iVolume->sizeZ = iVolDim->size_Z;

    // Volume size in bytes
    iVolume->volSizeBytes = sizeof(char) *
            iVolume->sizeX * iVolume->sizeY * iVolume->sizeZ;

    // Allocating volume
    iVolume->ptrVol_char = (char*) malloc (iVolume->volSizeBytes);

    // Open volume file
    FILE* ptrFile = fopen(volFile, "rb");

     // Double checking for the existance of the volume file
    if (!ptrFile)
    {
        printf("Error finding the raw volume file %s \n", volFile);
        exit(0);
    }

    // Read the volume file
    size_t imageSize = fread(iVolume->ptrVol_char,
                             BYTE,
                             iVolume->volSizeBytes,
                             ptrFile);

    // Checking if the volume was loaded or not
    if (!imageSize)
    {
        printf("Error readng the raw file %s \n", volFile);
        exit(0);
    }

    return iVolume;
}
