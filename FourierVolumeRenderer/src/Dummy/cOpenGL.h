#ifndef COPENGL_H
#define COPENGL_H


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



namespace cOpenGL
{
    void InitOpenGLContext(int argc, char** argv);
    void initGlut(int argc, char** argv);
    CUTBoolean CheckOpenGLExtensions();
    void initOpenGL();
    void DisplayGL();
    void Reshape(int fWindowWidth, int fWindowHeight);
    void Idle();
    void KeyBoard(unsigned char fKey, int fX, int fY);
    void Mouse(int fButton, int fState, int fX, int fY);
    void MouseMotion(int fX, int fY);
    void RegisterOpenGLCallBacks();





}
#endif // COPENGL_H
