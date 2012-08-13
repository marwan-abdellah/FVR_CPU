#include "fourierVolumeRender.h"
#include "cOpenGL.h"

#include "SpectrumProcessing/Spectrum.h"

#include "OpenGL/OpenGL.h"

#include "shared.h"
#include "Loader/loader.h"
#include "VolumeProcessing/volume.h"
#include "FFTShift/FFTShift.h"

#include "WrappingAround/WrappingAround.h"

#include "OpenGL/DisplayList.h"

#include "SliceProcessing/Slice.h"
#include "FFTShift/FFTShift.h"


GLuint 		eVolumeTexture_ID;
GLuint 		eSliceTexture_ID;

int mVolWidth			= 0;
int mVolHeight			= 0;
int mVolDepth			= 0;
int mUniDim 			= 0;
int mVolumeSize			= 0;
int mVolumeSizeBytes	= 0;
int mSliceWidth 		= 0;
int mSliceHeight 		= 0;
int mSliceSize			= 0;

// Wrapping Around Globals _________________________________________________*/
float** 	mImg_2D;
float** 	mImg_2D_Temp;
float***	mVol_3D;


GLuint		mSliceTextureID; 	// Extracted Slice ID
float mXrot = 0;
float mYrot = 0;
float mZrot = 0;
int mScalingFactor = 1;
float trans = 0;

// Framebuffer Object Globals
GLuint mFBO_ID;

char* 		mVolumeData;
unsigned char*		mRecImage;
float* 		mVolumeDataFloat;
float*	 	mAbsoluteReconstructedImage;

// OpenGL Texture Arrays
float* 		mTextureArray;
float* 		mFrameBufferArray;





// FFTW Globals ____________________________________________________________*/
fftwf_complex* 	mVolumeArrayComplex;
fftwf_complex* 	mSliceArrayComplex;


// Extract Slice from the 3D Spectrum
void GetSpectrumSlice()
{
    Slice::GetSlice(0, 1, mXrot, mYrot, mZrot, &eSliceTexture_ID, &eVolumeTexture_ID, mFBO_ID);

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
    mRecImage = (unsigned char*) malloc (mVolWidth * mVolHeight * sizeof(unsigned char));

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

int main(int argc, char** argv)
{ 	
    printf("Initializing Variables ... \n\n");

        // Volume Attributes
        mVolWidth			= 256;
        mVolHeight			= 256;
        mVolDepth			= 256;
        mUniDim 			= 256;

        mVolumeSize 		= mVolWidth * mVolHeight * mVolDepth;
        mVolumeSizeBytes	= mVolumeSize * sizeof(char);


        // Slice Attributes
        mSliceWidth 	= mVolWidth;
        mSliceHeight 	= mVolHeight;
        mSliceSize = mSliceWidth * mSliceHeight;




	// Initializing OpenGL Contex
	// First Initialize OpenGL Context, So We Can Properly Set the GL for CUDA.
    	// This is Necessary in order to Achieve Optimal Performance with OpenGL/CUDA Interop.

        cOpenGL::InitOpenGLContext(argc, argv);

    /* @ Loading the input volume dataset */
    volume* eVolume_char = Loader::loadVolume(eVolPath);

    /* @ Concerning volume data only and ignoring dimensions */
    char* eVolumeData_char =  eVolume_char->ptrVol_char;

    /* @ Extracting the volume brick from the big one */
    volDim eOriginalDim;
    eOriginalDim.size_X = 256;
    eOriginalDim.size_Y = 256;
    eOriginalDim.size_Z = 256;

    subVolDim eSubVolDim;
    eSubVolDim.max_X = 256;
    eSubVolDim.max_Y = 256;
    eSubVolDim.max_Z = 256;
    eSubVolDim.min_X = 0;
    eSubVolDim.min_Y = 0;
    eSubVolDim.min_Z = 0;
    eVolumeData_char = Volume::extractFinalVolume(eVolumeData_char, &eOriginalDim, &eSubVolDim);

    /* @ Creating the float volume */
    float* eVolumeData_float = Volume::createFloatVolume(eVolumeData_char, &eOriginalDim);

    /* @ Wrapping around the spatial volume */
    WrappingAround::WrapAroundVolume(mVol_3D, eVolumeData_float, 256);

    /* @ Creating the complex spectral volume */
    fftwf_complex* eVolumeData_complex = Spectrum::createSpectrum(eVolumeData_float, &eOriginalDim);
	
    /* @ Wrapping around the spectral volume */
    WrappingAround::WrapAroundSpectrum(mVol_3D, eVolumeData_float, eVolumeData_complex, 256);
	
    /* @ Packing the spectrum volume data into texture
     * array to be sent to OpenGL */
    float* eSpectrumTexture = Spectrum::packingSpectrumTexture(eVolumeData_complex, &eOriginalDim);

    /* @ Uploading the spectrum to the GPU texture */
    Spectrum::UploadSpectrumTexture(&eVolumeTexture_ID, eSpectrumTexture, &eOriginalDim);
	
	// We Don't Need Float Data Ayn More As It Resides in the GPU Texture Memory	
    free(eVolumeData_float);
	
	// OpenGL Initialization 
    cOpenGL::initOpenGL();
	
	// Creating & Binding Slice Textures (Src & Destination) 	
    Slice::CreateSliceTextureSrc(256, 256, &eSliceTexture_ID);
	
	// Preparing FBO & Its Associated Texture Getting Ready to Slice 3D Texture 
    OpenGL::PrepareFBO(&mFBO_ID, &eSliceTexture_ID);
	
	// Render Slice to FB Texture & Save It into Array	
	GetSpectrumSlice(); 
     
	printf("Start Rendering Mainloop ..... \n");
	glutMainLoop(); 
	return 0;
}


