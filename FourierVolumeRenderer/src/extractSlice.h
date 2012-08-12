#include "fourierVolumeRender.h"

#include "OpenGL/DisplayList.h"

#include "SliceProcessing/Slice.h"

void backTransformSlice()
{

    // 2D FFT
    // printf("Executing 2D Inverse FFT - Begin.... \n");
    mFFTWPlan = fftwf_plan_dft_2d(256, 256, mSliceArrayComplex,mSliceArrayComplex , FFTW_BACKWARD, FFTW_ESTIMATE);
    fftwf_execute(mFFTWPlan);
    //* printf("Executing 2D Inverse FFT - End.... \n");

    // Scaling
    int mSliceSize = mVolWidth * mVolHeight;
    int mNormalizedVal = mSliceSize * mScalingFactor * 1;
    for (int sliceCtr = 0; sliceCtr < mSliceSize; sliceCtr++)
    {
        mAbsoluteReconstructedImage[sliceCtr] =  (float) sqrt((mSliceArrayComplex[sliceCtr][0] * mSliceArrayComplex[sliceCtr][0]) + (mSliceArrayComplex[sliceCtr][1] * mSliceArrayComplex[sliceCtr][1]))/(mNormalizedVal);
    }

    int ctr = 0;
    for (int i = 0; i < mVolWidth; i++)
        for(int j = 0; j < mVolHeight; j++)
        {
            mImg_2D_Temp[i][j] = mAbsoluteReconstructedImage[ctr];
            mImg_2D[i][j] = 0;
            ctr++;
        }

    //* printf("Wrapping Around Resulting Image - Begin.... \n");
    mImg_2D = FFT_Shift_2D(mImg_2D_Temp, mImg_2D, mUniDim);
    mAbsoluteReconstructedImage = Repack_2D(mImg_2D, mAbsoluteReconstructedImage, mUniDim);
    //* printf("Wrapping Around Resulting Image - End.... \n");

    for (int i = 0; i < mVolWidth * mVolHeight; i++)
    {
        mRecImage[i] = (uchar)(mAbsoluteReconstructedImage[i]);
    }

}

void UploadImage()
{
    // Create 2D Texture Object as a Render Target
    glGenTextures(1, &mSliceTextureID);
    glBindTexture(GL_TEXTURE_2D, mSliceTextureID);

    // 2D Texture Creation & Parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Automatic Mipmap Generation Included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mSliceWidth, mSliceHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, mRecImage);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Extract Slice from the 3D Spectrum 
void GetSpectrumSlice()
{
    Slice::GetSlice(0, 1, mXrot, mYrot, mZrot, &mSliceTextureSrcID, &mVolTexureID, mFBO_ID);

    Slice::readBackSlice(256,256, mFBO_ID, mFrameBufferArray, mSliceArrayComplex );

    // Filter CPU
    // ProcessSlice();


   // backTransformSlice();

    Slice::backTransformSlice(mRecImage, mImg_2D_Temp, mImg_2D,
    mSliceArrayComplex, mAbsoluteReconstructedImage);

    // UploadImage();

    Slice::UploadImage(256,256, mRecImage, &mSliceTextureID);



}
