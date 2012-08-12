#ifndef OPENGL_H
#define OPENGL_H

#include <shared.h>


namespace OpenGL
{
    void InitOpenGLContext(int argc, char** argv);
    void initGlut(int argc, char** argv);
    void initOpenGL();
    void DisplayGL();
    void Idle();
    void Reshape(int fWindowWidth, int fWindowHeight);
    void KeyBoard(unsigned char fKey, int fX, int fY);
    void Mouse(int fButton, int fState, int fX, int fY);
    void MouseMotion(int fX, int fY);
}


#endif // OPENGL_H
