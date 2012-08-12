#include "volume.h"
#include "shared.h"

/// mVolumeSize - original one
/// xSize - new one

char*** Volume::allocCubeVolume_char(int size_X, int size_Y, int size_Z)
{
    printf("Allocating char cube volume [%d][%d][%d] \n", size_X, size_Y, size_Z);

    char*** cubeVolume;
    cubeVolume = (char***) malloc(size_X * sizeof(char**));
    for (int y = 0; y < size_X; y++)
    {
        cubeVolume[y] = (char**) malloc (size_Y* sizeof(char*));
        for (int x = 0; x < size_Y; x++)
        {
            cubeVolume[y][x] = (char*) malloc(size_Z * sizeof(char));
        }
    }

    printf("\t Done allocating char cube volume [%d][%d][%d] \n", size_X, size_Y, size_Z);
    return cubeVolume;
}

float*** Volume::allocCubeVolume_float(int size_X, int size_Y, int size_Z)
{
    printf("Allocating float cube volume [%d][%d][%d] \n", size_X, size_Y, size_Z);

    float*** cubeVolume;
    cubeVolume = (float***) malloc(size_X * sizeof(float**));
    for (int y = 0; y < size_X; y++)
    {
        cubeVolume[y] = (float**) malloc (size_Y* sizeof(float*));
        for (int x = 0; x < size_Y; x++)
        {
            cubeVolume[y][x] = (float*) malloc(size_Z * sizeof(float));
        }
    }

    printf("\t Done allocating float cube volume [%d][%d][%d] \n", size_X, size_Y, size_Z);
    return cubeVolume;
}

void Volume::packFlatVolume(char* flatVolume, char*** cubeVolume, volDim* iVolDim)
{
    printf("Packing cube volume in flat array [%d][%d][%d] \n",
           iVolDim->size_X, iVolDim->size_Y, iVolDim->size_Z);

    int voxel = 0;
    for (int i = 0; i < iVolDim->size_X; i++)
    {
        for (int j = 0; j < iVolDim->size_Y; j++)
        {
            for (int k = 0; k < iVolDim->size_Z; k++)
            {
                flatVolume[voxel] = cubeVolume[i][j][k];
                voxel++;
            }
        }
    }

    printf("\t Done packing cube volume in flat array [%d][%d][%d] \n",
           iVolDim->size_X, iVolDim->size_Y, iVolDim->size_Z);
}

void Volume::packCubeVolume(char*** cubeVolume, char* flatVolume, volDim* iVolDim)
{
    printf("Packing flat array in cube volume [%d][%d][%d] \n",
           iVolDim->size_X, iVolDim->size_Y, iVolDim->size_Z);

    int voxel = 0;
    for (int i = 0; i < iVolDim->size_X; i++)
    {
        for (int j = 0; j < iVolDim->size_Y; j++)
        {
            for (int k = 0; k < iVolDim->size_Z; k++)
            {
                cubeVolume[i][j][k] = flatVolume[voxel];
                voxel++;
            }
        }
    }

    printf("\t Done packing flat array in cube volume [%d][%d][%d] \n",
           iVolDim->size_X, iVolDim->size_Y, iVolDim->size_Z);
}

float* Volume::createFloatVolume(char* flatVol_char, volDim* iVolDim)
{
    printf("Creating flat float volume [%d][%d][%d] \n",
           iVolDim->size_X, iVolDim->size_Y, iVolDim->size_Z);

    float* flatVol_float =
            (float*) malloc (sizeof(float*) * iVolDim->size_X *
                             iVolDim->size_Y * iVolDim->size_Z);

    for (int i = 0; i < iVolDim->size_X * iVolDim->size_Y * iVolDim->size_Z; i++)
        flatVol_float[i] = (float) (unsigned char) flatVol_char[i];

    // Free the char flat array
    free(flatVol_char);

    printf("\t Done creating flat float volume [%d][%d][%d] \n",
           iVolDim->size_X, iVolDim->size_Y, iVolDim->size_Z);

    return flatVol_float;
}

void Volume::extractSubVolume(char*** originalCubeVol, char*** finalCubeVol, subVolDim* iSubVolDim)
{
    const int finalSize_X = iSubVolDim->max_X - iSubVolDim->min_X;
    const int finalSize_Y = iSubVolDim->max_Y - iSubVolDim->min_Y;
    const int finalSize_Z = iSubVolDim->max_Z - iSubVolDim->min_Z;

    printf("Extracting sub-volume with size [%d][%d][%d] \n",
           finalSize_X, finalSize_Y, finalSize_Z);

    for (int i = 0; i < finalSize_X; i++)
    {
        for (int j = 0; j < finalSize_Y; j++)
        {
            for (int k = 0; k < finalSize_Z; k++)
            {
                finalCubeVol[i][j][k] = originalCubeVol
                        [(iSubVolDim->min_X) + i]
                        [(iSubVolDim->min_Y) + j]
                        [(iSubVolDim->min_Z) + k];

            }
        }
    }

    printf("\t Done extracting sub-volume with size [%d][%d][%d] \n",
           finalSize_X, finalSize_Y, finalSize_Z);
}

char* Volume::extractFinalVolume(char* originalFlatVol, volDim* originalDim, subVolDim* iSubVol)
{
    printf("Extracting final volume \n");

    // Allocating 3D cube for the original volume
    char*** originalCubeVol = Volume::allocCubeVolume_char
            (originalDim->size_X, originalDim->size_Y, originalDim->size_Z);

    // Packing the original flat volume in the cube
    Volume::packCubeVolume(originalCubeVol, originalFlatVol, originalDim);

    // Dellocating the original flat volume
    free(originalFlatVol);

    // Allocating the final cube volume
    volDim iVolDim;
    iVolDim.size_X = iSubVol->max_X - iSubVol->min_X;
    iVolDim.size_Y = iSubVol->max_Y - iSubVol->min_Y;
    iVolDim.size_Z = iSubVol->max_Z - iSubVol->min_Z;
    char*** finalCubeVol = Volume::allocCubeVolume_char
            (iVolDim.size_X, iVolDim.size_Y, iVolDim.size_Z);

    // Extractig the sub-volume
    Volume::extractSubVolume(originalCubeVol, finalCubeVol, iSubVol);

    // Dellocating the original cube volume

    // Allocating the final flat volume
    char* finalFlatVolume = (char*) malloc (sizeof(char) * (iVolDim.size_X)
                                            * (iVolDim.size_Y)
                                            * (iVolDim.size_Z));

    // Packing the final cube volume in the flat array
    Volume::packFlatVolume(finalFlatVolume, finalCubeVol, &iVolDim);

    printf("Extracting final volume \n");
    return finalFlatVolume;
}


/*

void AllocateVolumeArrays()
{
    volIn3D = (char***) malloc(mVolWidth * sizeof(char**));
    for (int y = 0; y < mVolWidth; y++)
    {
        volIn3D[y] = (char**) malloc (mVolHeight * sizeof(char*));
        for (int x = 0; x < mVolHeight; x++)
        {
            volIn3D[y][x] = (char*) malloc(mVolDepth * sizeof(char));
        }
    }

    volOut3D = (char***) malloc(xWidth * sizeof(char**));
    for (int y = 0; y < xWidth; y++)
    {
        volOut3D[y] = (char**) malloc (xHeight * sizeof(char*));
        for (int x = 0; x < xHeight; x++)
        {
            volOut3D[y][x] = (char*) malloc(xDepth * sizeof(char));
        }
    }
}
void ExtractVolume()
{
    // We have the fVolumeData

    // Allocate 3D Arrays
    AllocateVolumeArrays();

    // Rearrange in 3D Array
    int ctr = 0;
    for (int i = 0; i < mVolWidth; i++)
    {
        for (int j = 0; j < mVolHeight; j++)
        {
            for (int k = 0; k < mVolDepth; k++)
            {
                volIn3D[i][j][k] = mVolumeData[ctr];
                ctr++;
            }
        }
    }

    int startX = 0;
    int startY = 0;
    int startZ = 0;

    // Extract Smaller Volume from the Dataset
    for (int i = 0; i < xWidth; i++)
    {
        for (int j = 0; j < xHeight; j++)
        {
            for (int k = 0; k < xDepth; k++)
            {
                volOut3D[i][j][k] = volIn3D[(startX) + i][(startY) + j][(startZ) + k];

            }
        }
    }

    // Zero the Original Array
    for (int i = 0; i < mVolWidth; i++)
    {
        for (int j = 0; j < mVolHeight; j++)
        {
            for (int k = 0; k < mVolDepth; k++)
            {
                volIn3D[i][j][k] = 0;
            }
        }
    }

    for (int i = 0; i < xWidth; i++)
    {
        for (int j = 0; j < xHeight; j++)
        {
            for (int k = 0; k < xDepth; k++)
            {
                volIn3D[i][j][k] = volOut3D[i][j][k];
            }
        }
    }

    // Repack the Extracted Voolume in 1D Array
    ctr = 0;
    for (int i = 0; i < mVolWidth; i++)
    {
        for (int j = 0; j < mVolHeight; j++)
        {
            for (int k = 0; k < mVolDepth; k++)
            {
                mVolumeData[ctr] = volIn3D[i][j][k];
                ctr++;
            }
        }
    }
}

void sssCreateFloatData()
{
    ExtractVolume();

    mVolumeDataFloat = (float*) malloc ( mVolumeSize * sizeof(float));

    // Packing Data in Float Array
    printf("Packing Data in a Float Array ... \n");
    for (int i = 0; i < mVolumeSize; i++)
        mVolumeDataFloat[i] = (float) (unsigned char) mVolumeData[i];
    printf("	Packing is Done  \n");

    // Realeasing Data in Char Array (Memory is a Scare Reource)
    printf("	Releasing Byte Data 	\n\n");
    delete [] mVolumeData;
}
*/
