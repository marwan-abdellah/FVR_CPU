#include "fourierVolumeRender.h"
#include "shared.h"
#include "Loader/loader.h"

void initVolumeData()
{
    printf("Loading dataSet \n");

    volume* iVolume = Loader::loadVolume(mPath);
    mVolumeData =  iVolume->ptrVol_char;

    printf("\t Done loading DataSet successfully \n");
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

