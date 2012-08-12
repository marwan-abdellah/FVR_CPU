#include "fourierVolumeRender.h"
#include "contextOpenGL.h"

#include "SpectrumProcessing/Spectrum.h"

#include "OpenGL/OpenGL.h"

#include "shared.h"
#include "Loader/loader.h"
#include "VolumeProcessing/volume.h"
#include "FFTShift/FFTShift.h"




#include "OpenGL/DisplayList.h"

#include "SliceProcessing/Slice.h"
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

void backTransformSlice()
{

    // 2D FFT
    // printf("Executing 2D Inverse FFT - Begin.... \n");
    fftwf_plan plan = fftwf_plan_dft_2d(256, 256, mSliceArrayComplex,mSliceArrayComplex , FFTW_BACKWARD, FFTW_ESTIMATE);
    fftwf_execute(plan);
    //* printf("Executing 2D Inverse FFT - End.... \n");

    // Scaling
    int mSliceSize = mVolWidth * mVolHeight;
    int mNormalizedVal = mSliceSize * mScalingFactor * 1;
    for (int sliceCtr = 0; sliceCtr < mSliceSize; sliceCtr++)
    {
        mAbsoluteReconstructedImage[sliceCtr] =  (float) sqrt((mSliceArrayComplex[sliceCtr][0] * mSliceArrayComplex[sliceCtr][0]) + (mSliceArrayComplex[sliceCtr][1] * mSliceArrayComplex[sliceCtr][1]))/(mNormalizedVal);
    }

    int ctr = 0;
    for (int i = 0; i < mVolWidth; i++)
        for(int j = 0; j < mVolHeight; j++)
        {
            mImg_2D_Temp[i][j] = mAbsoluteReconstructedImage[ctr];
            mImg_2D[i][j] = 0;
            ctr++;
        }

    //* printf("Wrapping Around Resulting Image - Begin.... \n");
    mImg_2D = FFTShift::FFT_Shift_2D(mImg_2D_Temp, mImg_2D, mUniDim);
    mAbsoluteReconstructedImage = FFTShift::Repack_2D(mImg_2D, mAbsoluteReconstructedImage, mUniDim);
    //* printf("Wrapping Around Resulting Image - End.... \n");

    for (int i = 0; i < mVolWidth * mVolHeight; i++)
    {
        mRecImage[i] = (uchar)(mAbsoluteReconstructedImage[i]);
    }

}

void UploadImage()
{
    // Create 2D Texture Object as a Render Target
    glGenTextures(1, &mSliceTextureID);
    glBindTexture(GL_TEXTURE_2D, mSliceTextureID);

    // 2D Texture Creation & Parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Automatic Mipmap Generation Included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mSliceWidth, mSliceHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mRecImage);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Extract Slice from the 3D Spectrum
void GetSpectrumSlice()
{
    Slice::GetSlice(0, 1, mXrot, mYrot, mZrot, &mSliceTextureSrcID, &mVolTexureID, mFBO_ID);

    // Creating FB Array to Recieve Data From FB Texture
    mFrameBufferArray	= (float*) malloc (mSliceWidth * mSliceHeight * 2 * sizeof(float));



    for (int i = 0; i < mSliceWidth * mSliceHeight * 2; i++)
    {
        mFrameBufferArray[i] = 0;
    }

    // Extracted Slice in the Frequency Domain
    mSliceArrayComplex = (fftwf_complex*) fftwf_malloc (mVolWidth * mVolHeight * sizeof(fftwf_complex));


    Slice::readBackSlice(256,256, mFBO_ID, mFrameBufferArray, mSliceArrayComplex );

    // Reconstructed Absolute Image
    mAbsoluteReconstructedImage = (float*) malloc (mVolWidth * mVolHeight * sizeof(float));

    // Character Reconstructed Image
    mRecImage = (uchar*) malloc (mVolWidth * mVolHeight * sizeof(uchar));

    // Creating 2D Arrays
    mImg_2D = (float**) malloc (mVolWidth * sizeof(float*));
    for (int x = 0; x < mVolWidth; x++)
    {
        mImg_2D[x] = (float*) malloc(mVolHeight * sizeof(float));
    }

    mImg_2D_Temp = (float**) malloc (mVolWidth * sizeof(float*));
    for (int x = 0; x < mVolWidth; x++)
    {
        mImg_2D_Temp[x] = (float*) malloc(mVolHeight * sizeof(float));
    }

    Slice::backTransformSlice(mRecImage, mImg_2D_Temp, mImg_2D,
    mSliceArrayComplex, mAbsoluteReconstructedImage);


    Slice::UploadImage(256,256, mRecImage, &mSliceTextureID);



}

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

int main(int argc, char** argv)
{ 	
    printf("Initializing Variables ... \n\n");

        // Volume Attributes
        mVolWidth			= 256;
        mVolHeight			= 256;
        mVolDepth			= 256;
        mUniDim 			= 256;
        mVolArea 			= mVolWidth * mVolHeight;
        mVolumeSize 		= mVolWidth * mVolHeight * mVolDepth;
        mVolumeSizeBytes	= mVolumeSize * sizeof(mVolumeType);


        // Slice Attributes
        mSliceWidth 	= mVolWidth;
        mSliceHeight 	= mVolHeight;
        mSliceSize = mSliceWidth * mSliceHeight;




	// Initializing OpenGL Contex
	// First Initialize OpenGL Context, So We Can Properly Set the GL for CUDA.
    	// This is Necessary in order to Achieve Optimal Performance with OpenGL/CUDA Interop.
    InitOpenGLContext(argc, argv);

    initVolumeData();

    extractVolume();

    createFloatData();

	// Wrapping Around Spatial Domain Data 
	WrapAroundVolume(); 

	// Creating Spectrum Complex Arrays
	CreateSpectrum(); 
	
	// Wrapping Around Spectrum Data 
	WrapAroundSpectrum(); 
	
	// Packing Spectrum Complex Data into Texture Array to be Sent to OpenGL
	PackingSpectrumTexture(); 

	// Binding 3D Spectrum Texture & Sending Texture to GPU 	
	SendSpectrumTextureToGPU(); 
	
	// We Don't Need Float Data Ayn More As It Resides in the GPU Texture Memory	
    free(mVolumeDataFloat);
	
	// OpenGL Initialization 
	initOpenGL(); 
	
	// Creating & Binding Slice Textures (Src & Destination) 	
    Slice::CreateSliceTextureSrc(256, 256, &mSliceTextureSrcID);
	
	// Preparing FBO & Its Associated Texture Getting Ready to Slice 3D Texture 
    OpenGL::PrepareFBO(&mFBO_ID, &mSliceTextureSrcID);
	
	// Render Slice to FB Texture & Save It into Array	
	GetSpectrumSlice(); 
     
	printf("Start Rendering Mainloop ..... \n");
	glutMainLoop(); 
	return 0;
}


