#ifndef OPENGL_H
#define OPENGL_H

#include <shared.h>
// OpenGL Includes
#include <GL/glew.h>
#include <GL/glu.h>

#if defined (__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


namespace OpenGL
{
    void InitOpenGLContext(int argc, char** argv);
    void initGlut(int argc, char** argv);
    void initOpenGL();
    void DisplayGL();
    void Reshape(int windowWidth, int windowHight);
    void Idle();
    void KeyBoard(unsigned char fKey, int fX, int fY);
    void Mouse(int fButton, int fState, int fX, int fY);
    void MouseMotion(int fX, int fY);
    void OpenGLRegisterOpenGLCallBacks();
    void updateImageTexture(GLuint* newImageTexture_ID);
    void PrepareFBO(GLuint* FBO_ID, GLuint* sliceTexture);


}


#endif // OPENGL_H
