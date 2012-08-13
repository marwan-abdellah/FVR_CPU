#ifndef RENDERINGLOOP_H
#define RENDERINGLOOP_H


#include "SliceProcessing/Slice.h"

namespace RenderingLoop
{
    void run(float iRot_X, float iRot_Y, float iRot_Z,
                            float iSliceCenter, float iSliceSideLength,
                            int iSliceWidth, int iSliceHeight,
                            GLuint* iSliceTexture_ID,
                            GLuint* iVolumeTexture_ID,
                            GLuint iFBO_ID,
                            GLuint* iImageTexture_ID);
}

#endif // RENDERINGLOOP_H
