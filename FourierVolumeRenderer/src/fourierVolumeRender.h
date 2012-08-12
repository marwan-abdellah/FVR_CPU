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


// Type Definitions _______________________________________________________________________________*/ 


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















     typedef unsigned int 	uint;
     typedef unsigned char 	uchar;
     typedef unsigned char 	mVolumeType;





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



     // OpenGL Globals  _________________________________________________________*/
     // Rotation Parameters
     int mXrot			= -0;
     int mYrot			= -0;
     int mZrot			= 0;
     int mScalingFactor 	= 25;

     // Framebuffer Object Globals
     GLuint mFBO_ID;



     // Data Arrays _____________________________________________________________*/
     // Default Arrays
     char* 		mVolumeData;
     uchar*		mRecImage;
     float* 		mVolumeDataFloat;
     float*	 	mAbsoluteReconstructedImage;

     // OpenGL Texture Arrays
     float* 		mTextureArray;
     float* 		mFrameBufferArray;

     // OpenGL Texture IDs
     GLuint 		mVolTexureID;		// 3D Spectrum Texture ID
     GLuint		mSliceTextureID; 	// Extracted Slice ID
     GLuint 		mSliceTextureSrcID; 	// Input Texture to CUDA ID




     float trans = 0;


     float2* fReconstructedImage;
     float2* fReconstructedImageDevice;

     char*** volIn3D;
     char*** volOut3D;

// Forward Declarations __________________________________________________________________________*/ 
// OpenGL Initialization Function __________________________________________*/
void InitOpenGLContext(int argc, char** argv); 
void initOpenGL(); 
void initGlut(int argc, char** argv);


// OpenGL Versioning Functions _____________________________________________*/
CUTBoolean CheckOpenGLExtensions();





void WrapAroundVolume(); 
void WrapAroundSpectrum(); 	
void GetSpectrumSlice();





#endif // _FOURIER_VOLUME_RENDER_H_

