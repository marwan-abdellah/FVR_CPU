#include "fourierVolumeRender.h"
#include "fftShift.h"
#include "loadData.h"
#include "contextOpenGL.h"
#include "contextCUDA.h"
#include "displayList.h"
#include "genericVariables.h"
#include "wrapAround.h"
#include "fourierSpectrum.h"
#include "createTexturesBuffers.h"
#include "extractSlice.h"
#include "resamplingFilter.h"
#include "processSlice.h"
 
int main(int argc, char** argv)
{ 	
	// Initializing Necessary Variables 
	InitVars(); 

	// Filter Size 
	N__ = 11; 
	pKernel = CreateFilter(N__);

	
	// Filtering Slices 
	inputSlice = (float2**)  malloc (mSliceWidth * mSliceHeight * sizeof(float2*));
	outputSlice = (float2**)  malloc (mSliceWidth * mSliceHeight * sizeof(float2*));
	
	for (int x = 0; x < mSliceWidth; x++)
	{
		inputSlice[x] = (float2*) malloc(mSliceHeight * sizeof(float2));
		outputSlice[x] = (float2*) malloc(mSliceHeight * sizeof(float2));
	}

	/*
	mPlane3D = (float2***) malloc(mSliceWidth * sizeof(float2**));
	for (int y = 0; y < mSliceWidth; y++)
	{
		mPlane3D[y] = (float2**) malloc (mSliceHeight * sizeof(float2*));
		for (int x = 0; x < mSliceHeight; x++)
		{
			mPlane3D[y][x] = (float2*) malloc( 5 * sizeof(float2));
		}
	}

	mPlane3D_Out = (float2***) malloc(mSliceWidth * sizeof(float2**));
	for (int y = 0; y < mSliceWidth; y++)
	{
		mPlane3D_Out[y] = (float2**) malloc (mSliceHeight * sizeof(float2*));
		for (int x = 0; x < mSliceHeight; x++)
		{
			mPlane3D_Out[y][x] = (float2*) malloc( 5 * sizeof(float2));
		}
	}

	for (int i = 0; i < 256; i++)
		for(int j = 0; j < 256; j++)		
			for (int k = 0; k < 5; k++)
			{
				mPlane3D_Out[i][j][k].x = 0;
				mPlane3D_Out[i][j][k].y = 0;
			}
	*/

	// Initializing OpenGL Contex
	// First Initialize OpenGL Context, So We Can Properly Set the GL for CUDA.
    	// This is Necessary in order to Achieve Optimal Performance with OpenGL/CUDA Interop.
	InitOpenGLContext(argc, argv); 
	
	printf("OpenGL Context Created Successfully - Moving to CUDA \n\n"); 
		
	// CUDA Initialization
	initCUDA();
	
	// Reading & Initializing Data 	
    // InitData();

    initVolumeData();

	// Creating Float Data & Releasing Byte Data 	
	CreateFloatData(); 

	// Wrapping Around Spatial Domain Data 
	WrapAroundVolume(); 

	// Creating Spectrum Complex Arrays
	CreateSpectrum(); 
	
	// Wrapping Around Spectrum Data 
	WrapAroundSpectrum(); 
	
	// Packing Spectrum Complex Data into Texture Array to be Sent to OpenGL
	PackingSpectrumTexture(); 

	// Binding 3D Spectrum Texture & Sending Texture to GPU 	
	SendSpectrumTextureToGPU(); 
	
	// We Don't Need Float Data Ayn More As It Resides in the GPU Texture Memory	
	delete [] mVolumeDataFloat; 
	
	// OpenGL Initialization 
	initOpenGL(); 

	// Create Enclosing Rectangles
	// Modified 
	// SetDisplayList(0.0);
	
	// Creating & Binding Slice Textures (Src & Destination) 	
	CreateSliceTextureSrc(); 
	CreateSliceTextureDst();
	
	// Preparing FBO & Its Associated Texture Getting Ready to Slice 3D Texture 
	PrepareFBO(); 
	
	// Allocating Needed Arrays 
	PrepareArrays(); 
	
	// Render Slice to FB Texture & Save It into Array	
	GetSpectrumSlice(); 
     
	printf("Start Rendering Mainloop ..... \n");
	glutMainLoop(); 
	return 0;
}

void RegisterOpenGLCallBacks()
{
	// Registering OpenGL Context
	printf("Registerng OpenGL Context CallBacks ... \n"); 
	
	glutDisplayFunc(DisplayGL);
	glutKeyboardFunc(KeyBoard);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	
	printf("	CallBacks Registered Successfully \n\n"); 
}
