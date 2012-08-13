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



char* mPath = "/home/abdellah/Software/DataSets/CTData/CTData";

// OpenGL Globals  _________________________________________________________*/
// Rotation Parameters
 float mXrot = 0;
 float mYrot = 0;
 float mZrot = 0;
 int mScalingFactor = 1;
 float trans = 0;
  GLuint		mSliceTextureID; 	// Extracted Slice ID


// Forward Declarations __________________________________________________________________________*/ 
// OpenGL Initialization Function __________________________________________*/
void InitOpenGLContext(int argc, char** argv); 
void initOpenGL(); 






extern void GetSpectrumSlice();






#endif // _FOURIER_VOLUME_RENDER_H_

