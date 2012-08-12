#include "fourierVolumeRender.h"
#include "shared.h"
#include "Loader/loader.h"
#include "VolumeProcessing/volume.h"

void initVolumeData()
{
    printf("Loading dataSet \n");

    volume* iVolume = Loader::loadVolume(mPath);
    mVolumeData =  iVolume->ptrVol_char;

    printf("\t Done loading DataSet successfully \n");
}

void extractVolume()
{
    volDim originalDim;
    originalDim.size_X = 256;
    originalDim.size_Y = 256;
    originalDim.size_Z = 256;

    subVolDim iSubVolDim;
    iSubVolDim.max_X = 256;
    iSubVolDim.max_Y = 256;
    iSubVolDim.max_Z = 256;
    iSubVolDim.min_X = 0;
    iSubVolDim.min_Y = 0;
    iSubVolDim.min_Z = 0;

    mVolumeData = Volume::extractFinalVolume(mVolumeData, &originalDim, &iSubVolDim);
}




void createFloatData()
{
    volDim originalDim;
    originalDim.size_X = 256;
    originalDim.size_Y = 256;
    originalDim.size_Z = 256;
    mVolumeDataFloat = Volume::createFloatVolume(mVolumeData, &originalDim);
}

