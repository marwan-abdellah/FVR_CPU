#ifndef _FOURIER_VOLUME_RENDER_H_
#define _FOURIER_VOLUME_RENDER_H_

// Includes _______________________________________________________________________________________*/ 
#ifdef _WIN32
#  include <windows.h>
#endif

// Systems Include 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <math.h>

// FFTW Includes 
#ifndef WIN32 
	#include <fftw3.h>
#else
	#include "fftw3.h"
#endif

// OpenGL Includes
#include <GL/glew.h>
#include <GL/glu.h>

#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// CUDA Utilities and System includes
// #include <shrUtils.h>
#include "nvShaderUtils.h"
#include "rendercheck_gl.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <cutil_inline.h>

#include <cutil_inline.h>
#include <cuda_runtime_api.h>
#include <cutil_gl_inline.h>
#include <cutil_gl_error.h>
#include <cuda_gl_interop.h>
#include <cudaGL.h>

#include <vector_types.h>
#include <vector_functions.h>
#include <driver_functions.h>

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// includes, GL
#include <GL/glew.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cuda_runtime_api.h>
#include <cutil_inline.h>
#include <cutil_gl_inline.h>
#include <cutil_gl_error.h>
#include <cuda_gl_interop.h>

// #include <rendercheck_gl.h>

// Type Definitions _______________________________________________________________________________*/ 
typedef unsigned int 	uint;
typedef unsigned char 	uchar;
typedef unsigned char 	mVolumeType;

using namespace std; 

#define OFFSET(i) ((char *)NULL + (i))

// OpenGL Checking 
const char *appName = "fourierVolumeRender";

#ifdef WIN32
     bool IsOpenGLAvailable(const char *appName) { return true; }
#else
  #if (defined(__APPLE__) || defined(MACOSX))
     bool IsOpenGLAvailable(const char *appName) { return true; }
  #else
     // Linux Machine
     #include <X11/Xlib.h>

     bool IsOpenGLAvailable(const char *appName)
     {
        Display *Xdisplay = XOpenDisplay(NULL);
        
        if (Xdisplay == NULL) 
        {
           return false;
        } 
        else 
        {
           XCloseDisplay(Xdisplay);
           return true;
        }
     }
  #endif
#endif



float sVal = 1.0;

// Globals ________________________________________________________________________________________*/
// GLUT Globals ____________________________________________________________*/
int mWindowWidth		= 512;
int mWindowHeight		= 512;


// Volume Attributes _______________________________________________________*/
// Volume File Path
// char* mPath =  "../../Data/CTHead1/CTData.img";
char* mPath = "/home/abdellah/Software/DataSets/CTData/CTData";

// Volume Dimensions
int mVolWidth			= 0;
int mVolHeight			= 0;
int mVolDepth			= 0;
int mUniDim 			= 0;
int mVolArea 			= 0;

// Original Volume Dimensions
int xWidth		= 256;
int xHeight		= 256;
int xDepth		= 256;

// Volume Size
int mVolumeSize			= 0;
int mVolumeSizeBytes	= 0;


// Slice Attributes ________________________________________________________*/
int mSliceWidth 		= 0;
int mSliceHeight 		= 0;
int mSliceSize			= 0;



// FFTW Globals ____________________________________________________________*/
fftwf_complex* 	mVolumeArrayComplex;
fftwf_complex* 	mSliceArrayComplex;
fftwf_plan 	mFFTWPlan;


// Wrapping Around Globals _________________________________________________*/
float** 	mImg_2D;
float** 	mImg_2D_Temp;
float***	mVol_3D;

float2*** mPlane3D;
float2*** mPlane3D_Out;



// Dummy CUDA Context Globals ______________________________________________*/
int 		mArraySize	= 64;
float*		mDeviceArray;
float*		mHostArray;

int mNumTexels 			= 0;
int mNumValues 			= 0;
int mTextureDataSize 	= 0;


// OpenGL Globals  _________________________________________________________*/
// Rotation Parameters
int mXrot			= -0;
int mYrot			= -0;
int mZrot			= 0;
int mScalingFactor 	= 25;

// Framebuffer Object Globals
GLuint mFBO_ID;
GLuint mFBO_ID_P2;
GLuint mFBO_ID_P1;
GLuint mFBO_ID_N1;
GLuint mFBO_ID_N2;




// Data Arrays _____________________________________________________________*/
// Default Arrays
char* 		mVolumeData;
uchar*		mRecImage;
float* 		mVolumeDataFloat;
float*	 	mAbsoluteReconstructedImage;

// OpenGL Texture Arrays
float* 		mTextureArray;
float* 		mFrameBufferArray;

float* 		mFrameBufferArray_N2;
float* 		mFrameBufferArray_N1;
float* 		mFrameBufferArray_P1;
float* 		mFrameBufferArray_P2;

// OpenGL Texture IDs
GLuint 		mVolTexureID;		// 3D Spectrum Texture ID
GLuint		mSliceTextureID; 	// Extracted Slice ID
GLuint 		mSliceTextureSrcID; 	// Input Texture to CUDA ID
GLuint 		mProcessTextureID; 	// Input Processing Texture to CUDA ID
GLuint 		mSliceTextureResID;	// Destination of CUDA Results ID



float trans = 0;


GLuint 		mSliceTextureSrcID_N1; 	// Input Texture to CUDA ID
GLuint 		mSliceTextureSrcID_N2; 	// Input Texture to CUDA ID
GLuint 		mSliceTextureSrcID_P1; 	// Input Texture to CUDA ID
GLuint 		mSliceTextureSrcID_P2; 	// Input Texture to CUDA ID


struct cudaGraphicsResource *mCUDA_InputTexSliceResource;	// Input Slice Texture to CUDA
struct cudaGraphicsResource *mCUDA_InputProcesstextureResource;	// Input Proecssing Texture to CUDA
struct cudaGraphicsResource *mCUDA_TexResultResource;		// Resulting Texture After CUDA Processing 

float* CUDA_DestResource;

float2* fReconstructedImage; 
float2* fReconstructedImageDevice;   

bool CUDA_ENABLED = true; 

char*** volIn3D; 
char*** volOut3D; 
// Forward Declarations __________________________________________________________________________*/ 
// OpenGL Initialization Function __________________________________________*/
void InitOpenGLContext(int argc, char** argv); 
void initOpenGL(); 
void initGlut(int argc, char** argv);


// OpenGL Versioning Functions _____________________________________________*/
CUTBoolean CheckOpenGLExtensions();


// OpenGL Contetx Function _________________________________________________*/
void DisplayGL(); 
void Reshape(); 
void KeyBoard(); 
void Idel(); 
void Mouse(int fButton, int fState, int fX, int fY); 
void MouseMotion(int fX, int fY);
void RegisterOpenGLCallBacks();  


// Loading & Initializing Data Functions ___________________________________*/
void InitVars(); 
void InitData(); 
char* LoadRawFile(char* fFileName, size_t fSize); 


// CUDA Functions __________________________________________________________*/
void initCUDA(); 
void RunCUDAKernel();
//void Init_CUDA_Buffers(); 

extern "C"		   
void LaunchSliceProcessingOnCUDA
(dim3 grid, dim3 block, cudaArray* fInputSlice, float2* fOutputSlice, int fSliceWidth, int fSliceHeigh);

extern "C"
void CreateFilterGPU(dim3 grid, dim3 block, float* fFilter, int fPitch, int N, float* HostFilterCopied);  


// Processing Functions ____________________________________________________*/ 
void PrepareArrays();
void CreateFloatData(); 	
void WrapAroundVolume(); 
void CreateSpectrum(); 
void WrapAroundSpectrum(); 	
void CreateSpectrumTexture(); 
void GetSpectrumSlice(); 
void PackingSpectrumTexture(); 
void SendSpectrumTextureToGPU(); 
void SetDisplayList(float fCenter); 	

void PrepareFBO(); 
void ProcessSliceCUDA(int, int);


float* Filter1DforCUDA; 

float** pKernel;
float** CreateFilter(int Size); 

double cpu_time; 
int mLoopCPU = 1; 
int N__ ;



int mFilterSize = 5; 

float *mWindow, *mSinc2D, *mKernel2D; 
size_t mPitchWin, mPitchSinc, mPitchKernel; 

void ProcessSlice();

float2** inputSlice;
float2** outputSlice;

void CreateSliceTextureSrc(); 
void CreateSliceTextureDst();

// Wrapping Around Functions _______________________________________________*/
float**  FFT_Shift_2D(float** iArr, float** oArr, int N); 
float*** FFT_Shift_3D(float* Input, int N); 

float* 	Repack_2D(float** Input_2D, float* Input_1D, int N); 
float* 	Repack_3D(float*** Input_3D, float* Input_1D, int N);

#endif // _FOURIER_VOLUME_RENDER_H_

