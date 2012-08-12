#include "fourierVolumeRender.h"
#include "FFTShift/FFTShift.h"

// 3D Wrapping Around for Space Data 
void WrapAroundVolume()
{
	printf("Wrapping Around Volume Data ... \n"); 
	
    mVol_3D = FFTShift::FFT_Shift_3D(mVolumeDataFloat, mUniDim);
    mVolumeDataFloat = FFTShift::Repack_3D(mVol_3D, mVolumeDataFloat, mUniDim);
	
	printf("	Wrapping Around Volume Data Done Successfully \n\n");
}

// 3D Wrapping Around the Spectrum 
void WrapAroundSpectrum()
{
	printf("Wrapping Around Spectrum Data ... \n");
	printf("	Real Part ... \n");
	
	for (int i = 0; i < mVolumeSize; i++)
		mVolumeDataFloat[i] =  mVolumeArrayComplex[i][0]; 
	
    mVol_3D = FFTShift::FFT_Shift_3D(mVolumeDataFloat, mUniDim);
    mVolumeDataFloat = FFTShift::Repack_3D(mVol_3D, mVolumeDataFloat, mUniDim);
	
	for (int i = 0; i < mVolumeSize; i++)
		mVolumeArrayComplex[i][0] = mVolumeDataFloat[i]; 
	
	printf("	Imaginary Part ... \n");
	
	for (int i = 0; i < mVolumeSize; i++)
		mVolumeDataFloat[i] =  mVolumeArrayComplex[i][1]; 
	
    mVol_3D = FFTShift::FFT_Shift_3D(mVolumeDataFloat, mUniDim);
    mVolumeDataFloat = FFTShift::Repack_3D(mVol_3D, mVolumeDataFloat, mUniDim);
	
	for (int i = 0; i < mVolumeSize; i++)
		mVolumeArrayComplex[i][1] = mVolumeDataFloat[i]; 
	
	printf("	Wrapping Around Spectrum Data Done Successfully \n\n");
} 
