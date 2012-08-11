#include "fourierVolumeRender.h"

// Create Spectrum Data 
void CreateSpectrum()
{
	printf("Creating Complex Spectrum ... \n"); 	
	
	// Single Precision Complex Array 
	mVolumeArrayComplex = (fftwf_complex*) fftwf_malloc (mVolWidth * mVolHeight * mVolDepth * sizeof(fftwf_complex)); 
	
	// Packing Complex Array 
	printf("	Packing Volume Array for Single Precision Format ... \n"); 
	for (int i = 0; i < mVolumeSize; i++)
	{
		mVolumeArrayComplex[i][0] = mVolumeDataFloat[i]; 
		mVolumeArrayComplex[i][1] = 0;
	}
	printf("	Packing Done Successfully \n\n"); 
	
	// 3D Forward Fourier Transforming Data 
	printf("	Executing 3D Forward FFT  ... \n"); 

	mFFTWPlan = fftwf_plan_dft_3d(mVolWidth, mVolHeight, mVolDepth, mVolumeArrayComplex, mVolumeArrayComplex, FFTW_FORWARD, FFTW_ESTIMATE);
	fftwf_execute(mFFTWPlan);

	printf("	3D Forward FFT Done Successfully  \n\n");	
}

//  Packing 3D Texture with 2 Components, Real+Imaginary 
void PackingSpectrumTexture()
{
	// Allocate Complex Texture Array to be Sent to the GPU
	mTextureArray = (float*) malloc (mVolumeSize * 2 * sizeof(float)); 
	
	printf("Packing Spectrum Into Texture ... \n");

	int ctr = 0; 
	for (int i = 0; i < (mVolumeSize * 2); i += 2)
	{	
		mTextureArray[i]		= mVolumeArrayComplex[ctr][0];
		mTextureArray[i + 1]	= mVolumeArrayComplex[ctr][1];
		ctr++; 
	}
	
	printf("	Packing Spectrum Into Texture Done Successfully \n\n");
}

// Send Texture Volume to the GPU Texture Memory 
void SendSpectrumTextureToGPU(void)
{
	printf("Creating & Binding Spectrum Texture To GPU ... \n"); 
	
	// 3D Texture Creation & Binding 
	glGenTextures(1, &mVolTexureID);
	glBindTexture(GL_TEXTURE_3D, mVolTexureID);
	
	// Parameters 
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// For Automatic Texture Coordinate Generation
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	 
	// Transfer Data to GPU Memory 
	printf("	Transfer Data to GPU Memory ... \n");

	glTexImage3D(GL_TEXTURE_3D, 0, RG32F, mVolWidth, mVolHeight, mVolDepth, 0, RG, GL_FLOAT,  mTextureArray);

	printf("	Transfering Data to GPU Memory Done Successfully \n\n"); 
}
