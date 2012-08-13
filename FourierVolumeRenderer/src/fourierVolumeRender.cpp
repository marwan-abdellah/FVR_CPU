#include "fourierVolumeRender.h"
#include "cOpenGL.h"

#include "SpectrumProcessing/Spectrum.h"


#include "shared.h"
#include "Loader/loader.h"
#include "VolumeProcessing/volume.h"
#include "FFTShift/FFTShift.h"

#include "WrappingAround/WrappingAround.h"

#include "OpenGL/DisplayList.h"
#include "OpenGL/OpenGL.h"
#include "SliceProcessing/Slice.h"
#include "FFTShift/FFTShift.h"

#include "RenderingLoop/RenderingLoop.h"


GLuint eVolumeTexture_ID;
GLuint eSliceTexture_ID;
GLuint eImageTexture_ID;
GLuint eFBO_ID;

fftwf_complex* 	eSlice_complex;


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



float mXrot = 0;
float mYrot = 0;
float mZrot = 0;
int mScalingFactor = 1;
float trans = 0;

// Framebuffer Object Globals


char* 		mVolumeData;
unsigned char*		mRecImage;
float* 		mVolumeDataFloat;
float*	 	mAbsoluteReconstructedImage;

// OpenGL Texture Arrays
float* 		mTextureArray;
float* 		mFrameBufferArray;








// Extract Slice from the 3D Spectrum
void GetSpectrumSlice()
{

//RenderingLoop::run(mXrot, mYrot, mZrot, 0, 1, 256, 256,
  //                 &eSliceTexture_ID, &eVolumeTexture_ID, eFBO_ID, &eImageTexture_ID);


    int iSliceWidth = 256;
    int iSliceHeight = 256;
    /* @ Extrat the projection slice from the spectral volume texture */
    Slice::GetSlice(0, 1, mXrot, mYrot, mZrot,
                    &eSliceTexture_ID, &eVolumeTexture_ID, iFBO_ID);

    /* @ Reading back the FBO after initialization */
    float* eFB = (float*) malloc (iSliceWidth * iSliceHeight * sizeof(float));
    fftwf_complex* eSlice_complex = (fftwf_complex*) fftwf_malloc
            (iSliceWidth * iSliceHeight * sizeof(fftwf_complex));

    for (int i = 0; i < iSliceWidth * iSliceHeight * 2; i++)
        eFB[i] = 0;

    Slice::readBackSlice(iSliceWidth, iSliceHeight, eFBO_ID, eFB, eSlice_complex );

    unsigned char* eRecImage = ( unsigned char*) malloc (iSliceWidth * iSliceHeight * sizeof( unsigned char));
    float* eRecImageAbsolute = (float*) malloc (iSliceWidth * iSliceHeight * sizeof(float));

    /* 2D arrays for the wrapping around operations */
    float** eImage_MAIN = (float**) malloc (iSliceWidth * sizeof(float*));
    float** eImage_TEMP = (float**) malloc (iSliceWidth * sizeof(float*));
    for (int i = 0; i < iSliceWidth; i++)
    {
        eImage_MAIN[i] = (float*) malloc(iSliceHeight * sizeof(float));
        eImage_TEMP[i] = (float*) malloc(iSliceHeight * sizeof(float));
    }

    /* @ Back transform the extracted slice to create the projection */
    Slice::backTransformSlice(eRecImage, eImage_TEMP, eImage_MAIN,
    eSlice_complex, eRecImageAbsolute);

    /* @ Update the rendering context with the new image */
    cOpenGL::updateSliceTexture(&eImageTexture_ID);

    /* @ Upload the image to the GPU */
    Slice::UploadImage(iSliceWidth, iSliceHeight, eRecImage, &eImageTexture_ID);


}

int main(int argc, char** argv)
{ 	
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

    free(eVolumeData_float);
	
    /* @ OpenGL initialization */
    cOpenGL::initOpenGL();
	
    /* @ Creating the projection slice texture & binding it */
    Slice::createSliceTexture(256, 256, &eSliceTexture_ID);
	
    /* @ Prepare the FBO & attaching the slice texture to it */
    cOpenGL::prepareFBO(&eFBO_ID, &eSliceTexture_ID);
	
	// Render Slice to FB Texture & Save It into Array	
	GetSpectrumSlice(); 

	glutMainLoop(); 
	return 0;
}


