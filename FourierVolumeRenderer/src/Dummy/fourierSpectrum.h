#include "fourierVolumeRender.h"
#include "SpectrumProcessing/Spectrum.h"

// Create Spectrum Data 
void CreateSpectrum()
{
    volDim iVolDim;
    iVolDim.size_X = 256;
    iVolDim.size_Y = 256;
    iVolDim.size_Z = 256;
    mVolumeArrayComplex = Spectrum::createSpectrum(mVolumeDataFloat, &iVolDim);
}

//  Packing 3D Texture with 2 Components, Real+Imaginary 
void PackingSpectrumTexture()
{
    volDim iVolDim;
    iVolDim.size_X = 256;
    iVolDim.size_Y = 256;
    iVolDim.size_Z = 256;
    mTextureArray = Spectrum::packingSpectrumTexture(mVolumeArrayComplex, &iVolDim);
}

// Send Texture Volume to the GPU Texture Memory 
void SendSpectrumTextureToGPU()
{
    volDim iVolDim;
    iVolDim.size_X = 256;
    iVolDim.size_Y = 256;
    iVolDim.size_Z = 256;
    Spectrum::UploadSpectrumTexture(&mVolTexureID, mTextureArray, &iVolDim);
}
