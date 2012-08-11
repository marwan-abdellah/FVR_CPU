#include "fourierVolumeRender.h"
#include "Shared.h"
void InitData()
{
	printf("Loading DataSet ... \n"); 
	if (mPath == 0) 
    {
		// sprintf("Error Finding File '%s' ... \n", mPath); 
		exit(0); 
	}
	 
	// Loading Data from File 
	mVolumeData = LoadRawFile(mPath, mVolumeSizeBytes); 
	
	printf("	DataSet Loaded Successfully \n\n");
}




volume* loadVolume(char* volFile, char* hdrFile)
{
    // Checking for the existance of the volume file
    if (!volFile)
    {
        printf("Error finding the raw volume file %s \n", volFile);
        exit(0);
    }

    // Checking for the existance of the header file
    if (!hdrFile)
    {
        printf("Error finding the header file %s \n", hdrFile);
        exit(0);
    }

    // Allocating volume structre
    volume* iVolume = (volume*) malloc (sizeof(volume));

    // Read the header file to get volume dimensions
    iVolume->sizeX = mVolWidth;
    iVolume->sizeY = mVolHeight;
    iVolume->sizeZ = mVolDepth;

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

// Loading Raw Data 
char* LoadRawFile(char* fFileName, size_t fSize)
{
	FILE* fPtrFile; 
	fPtrFile = fopen(fFileName, "rb"); 
	
	if (!fPtrFile)
	{
		fprintf(stderr, "Error Opening File '%s' ... \n", fFileName); 
		exit(0); 
	}
	
	// Memory Allocation  
	char* fVolumeData = (char*) malloc (fSize); 
	
	// File Reading Operation 
	size_t fFileSizeBytes = fread(fVolumeData, 1, fSize, fPtrFile); 
	
	return fVolumeData; 
}

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

void CreateFloatData()
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

