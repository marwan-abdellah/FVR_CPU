#include "OpenGL.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cutil_inline.h>
#include <cuda_runtime_api.h>
#include <cutil_gl_inline.h>
#include <cutil_gl_error.h>
#include <cuda_gl_interop.h>
#include <cudaGL.h>

#include <vector_types.h>
#include <vector_functions.h>
#include <driver_functions.h>


#include "globals.h"

void OpenGL::InitOpenGLContext(int argc, char** argv)
{
    printf ("Initializing OpenGL Contex ... \n");
    printf ("	First Initialize OpenGL Context, So We Can Properly Set the GL for CUDA. \n");
    printf ("	This is Necessary in order to Achieve Optimal Performance with OpenGL/CUDA Interop. \n");

    /*
    if (IsOpenGLAvailable(appName))
        fprintf(stderr, "	OpenGL Device is Available \n\n");
    else
    {
        fprintf(stderr, "	OpenGL device is NOT Available, [%s] exiting...\n  PASSED\n\n", appName );
        exit(0);
    }
    */

    // GLUT Initialization
    initGlut(argc, argv);

    /*
    // Initialize Necessary OpenGL Extensions
    if (CUTFalse == CheckOpenGLExtensions())
    {
        printf("	Missing OpenGL Necessary Extensions  \n Exiting ..... \n");
        exit(0);
    }
    else
        printf("	Requied OpenGL Extensions are Found \n\n");

    // Register OpenGL CallBack Functions
    RegisterOpenGLCallBacks();
    */
}


void OpenGL::initGlut(int argc, char** argv)
{
    // Initializing GLUT
    printf("Initializing GLUT ... \n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(Global::mWindowWidth, Global::mWindowHeight);
    glutCreateWindow("Fourier Volume Rendering on CUDA");

    printf("	Display Mode		: GLUT_RGBA | GLUT_DOUBLE \n");
    printf("	GLUT Windows Size	: %d mLoop %d \n \n", Global::mWindowWidth, Global::mWindowHeight);
}

void OpenGL::initOpenGL()
{
    printf("Initializing OpenGL ... \n");

    // Clearing Buffer
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // Pixel Storage Mode
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    printf("	Initializing OpenGL Done \n\n");
}

void DisplayGL()
{
    // Clearing color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Disable depth buffer
    glDisable(GL_DEPTH_TEST);

    // Binding slice texture to be displayed on OpenGL polygon
    glBindTexture(GL_TEXTURE_2D, Global::mSliceTextureID);
    glEnable(GL_TEXTURE_2D);

    // Slice texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Adjusting Viewport
    glViewport(-Global::sWindowWidth / 2, -Global::sWindowHeight / 2, Global::sWindowWidth * 2, Global::sWindowHeight * 2);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Center Slice Texture (0,0)
    glScalef(Global::mImageScale, Global::mImageScale, 1);
    glTranslatef(-0.5, -0.5, 0.0);

    glBegin(GL_QUADS);
        glVertex2f(0, 0);		glTexCoord2f(0, 0);
        glVertex2f(0, 1);		glTexCoord2f(1, 0);
        glVertex2f(1, 1);		glTexCoord2f(1, 1);
        glVertex2f(1, 0);		glTexCoord2f(0, 1);
    glEnd();
    glPopMatrix();

    // Release Texture Reference & Disable Texturing
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    // Swapping Buffer Contents
    glutSwapBuffers();
}


void OpenGL::Reshape(int windowWidth, int windowHight)
{
    // Adjust your viewport
    glViewport(0, 0, windowWidth, windowHight);

    // Update global window parameters to reflect texture updates
    Global::sWindowHeight = windowHight;
    Global::sWindowWidth = windowWidth;

    // Adjust the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void OpenGL::Idle()
{
    // Redisplay loop
    glutPostRedisplay();
}

void OpenGL::KeyBoard(unsigned char fKey, int fX, int fY)
{

    // Dummy
    if (fX | fY | fKey) {}

    switch(fKey)
    {
        case 27:
            exit (0);
            break;
        case 'Q':
            Global::mXrot += 5.0;
            printf("Rotating %f around mLoop ... \n", (float) Global::mXrot);
            break;
        case 'q':
            Global::mXrot -= 5.0;
            printf("Rotating %f around mLoop ... \n", (float) Global::mXrot);
            break;
        case 'W':
            Global::mYrot += 5.0;
            printf("Rotating %f around Y ... \n", (float) Global::mYrot);
            break;
        case 'w':
            Global::mYrot -= 5.0;
            printf("Rotating %f around Y ... \n", (float) Global::mYrot);
            break;
        case 'E':
           Global::mZrot += 5.0;
            printf("Rotating %f around Z ... \n", (float) Global::mZrot);
            break;
        case 'e':
            Global::mZrot -= 5.0;
            printf("Rotating %f around Z ... \n", (float) Global::mZrot);
            break;
        case ' ':
           // CUDA_ENABLED = (!CUDA_ENABLED);
            printf("Enabling / Disabling CUDA Processing");
            break;

        case 'R':
            Global::sVal = Global::sVal * 10;
            printf("sVal %f \n", Global::sVal);
            break;

        case 'r':
            Global::sVal = Global::sVal / 10;
            printf("sVal %f \n", Global::sVal);
            break;

        case 'o':
            Global::trans = Global::trans + 1;
            printf("trans : %f/256 \n", Global::trans);
            break;

        case 'p':
            Global::trans = Global::trans - 1;
            printf("trans : %f/256 \n", Global::trans);
            break;

        case 's':
            Global::mScalingFactor *= 5;
            printf("mScalingFactor : %f \n", Global::mScalingFactor);
            break;

        case 'S':
            Global::mScalingFactor /= 5;
            printf("mScalingFactor : %f \n", Global::mScalingFactor);
            break;

        case 'a':
            Global::mScalingFactor += 10;
            printf("mScalingFactor : %f \n", Global::mScalingFactor);
            break;

        case 'A':
            Global::mScalingFactor -= 10;
            printf("mScalingFactor : %f \n", Global::mScalingFactor);
            break;

        case 'z' :Global::mImageScale += 0.5;
        break;

        case 'Z' :Global::mImageScale -= 0.5;
        break;

        default:
            break;
    }

    // Re-Slice & Re-Display
   // GetSpectrumSlice();
    glutPostRedisplay();
}


void OpenGL::Mouse(int fButton, int fState, int fX, int fY)
{
    glutPostRedisplay();

    // Dummy
    if (fX | fY | fState | fButton) {}

}

void OpenGL::MouseMotion(int fX, int fY)
{
    glutPostRedisplay();

    // Dummy
    if (fX | fY) {}
}

