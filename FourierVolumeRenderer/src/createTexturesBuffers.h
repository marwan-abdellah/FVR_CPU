#include "fourierVolumeRender.h"

// Slice Textures (To be Sent from OpenGL to CUDA )
void CreateSliceTextureSrc()
{
	printf("Creating & Binding Input Slice Texture ... \n"); 
	
	// Create 2D Texture Object as a Render Target 
	glGenTextures(1, &mSliceTextureSrcID);
	glBindTexture(GL_TEXTURE_2D, mSliceTextureSrcID);

	// 2D Texture Creation & Parameters 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
	// Automatic Mipmap Generation Included in OpenGL v1.4
	glTexImage2D(GL_TEXTURE_2D, 0, RG32F, mSliceWidth, mSliceHeight, 0, RG, GL_FLOAT, mTextureArray);
	
	// Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	printf("	Creating Input Slice Texture Done Successfully \n\n"); 
	
	// Create 2D Texture Object as a processing Slice
	glGenTextures(1, &mProcessTextureID);
	glBindTexture(GL_TEXTURE_2D, mProcessTextureID);

	// 2D Texture Creation & Parameters 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
	// Automatic Mipmap Generation Included in OpenGL v1.4
	glTexImage2D(GL_TEXTURE_2D, 0, RG32F, mSliceWidth, mSliceHeight, 0, RG, GL_FLOAT, NULL);
	
	// Register Texture with CUDA
	cudaGraphicsGLRegisterImage
	(&mCUDA_InputProcesstextureResource, mProcessTextureID, GL_TEXTURE_2D, cudaGraphicsMapFlagsReadOnly); 
	          
	// Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	printf("	Creating Input Slice Texture Done Successfully \n\n"); 	
}

void CreateSliceTextureDst()
{
	printf("Creating Destination Slice Texture ... \n "); 
	
	// Create & Bind Destination Texture
	glGenTextures(1, &mSliceTextureResID);
	glBindTexture(GL_TEXTURE_2D, mSliceTextureResID);

	// 2D Texture Creation & Parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Allocating Texture Memory on GPU
	glTexImage2D(GL_TEXTURE_2D, 0, RG32F , mSliceWidth, mSliceHeight, 0, RG, GL_FLOAT, NULL);
	
	
	printf("	Destination Slice Texture Created Successfully \n\n"); 
}


void PrepareFBO()
{
	printf("Preparing FrameBuffer Object & Its Associated Texture \n"); 	
		
	////////////////////////////////////////////////////////////////////////// 0
	// Framebuffer Object 
	glGenFramebuffersEXT(1, &mFBO_ID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO_ID);

	// Attach Texture to FBO Color Attachement Point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mSliceTextureSrcID, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	printf("	Frame Buffer Preparation Done Successfully \n\n"); 
}

void PrepareArrays()
{	
	printf("Prepare Arrays .... Start \n"); 
	
	// Creating FB Array to Recieve Data From FB Texture 
	mFrameBufferArray	= (float*) malloc (mSliceWidth * mSliceHeight * 2 * sizeof(float));



	for (int i = 0; i < mSliceWidth * mSliceHeight * 2; i++)
	{
		mFrameBufferArray[i] = 0;	
	}
	
	// Extracted Slice in the Frequency Domain 
	mSliceArrayComplex = (fftwf_complex*) fftwf_malloc (mVolWidth * mVolHeight * sizeof(fftwf_complex)); 

	// Reconstructed Absolute Image 
	mAbsoluteReconstructedImage = (float*) malloc (mVolWidth * mVolHeight * sizeof(float));

	// Character Reconstructed Image 
	mRecImage = (uchar*) malloc (mVolWidth * mVolHeight * sizeof(uchar)); 

	// Creating 2D Arrays 
	mImg_2D = (float**) malloc (mVolWidth * sizeof(float*));
	for (int x = 0; x < mVolWidth; x++)
	{
		mImg_2D[x] = (float*) malloc(mVolHeight * sizeof(float));
	}

	mImg_2D_Temp = (float**) malloc (mVolWidth * sizeof(float*));
	for (int x = 0; x < mVolWidth; x++)
	{
		mImg_2D_Temp[x] = (float*) malloc(mVolHeight * sizeof(float));
	}
	
	printf("Prepare Arrays .... End \n"); 
}
