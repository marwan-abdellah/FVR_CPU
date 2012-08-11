#ifndef _FVR_CU_INTERFACE_CU_
#define _FVR_CU_INTERFACE_CU_

// Includes, System
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Includes, Project
#ifndef _WIN32
	//#include <shrUtils.h>
#endif

#include <cutil_inline.h>

// #include "getTimeOfDay.h"

int mLoopGPU = 10; 
int gpuFilterSize = 5;

// Slice Texture
texture <float2, 2, cudaReadModeElementType> inSliceTex;


// timeval gpustartTimer_, gpustopTimer_;
double gpu_time; 

// Input Slice Texture 
/* 
	Type 		: float 
	No. Components	: 2 Components After Modefying GLEW  
	Dimensions 	: 2D Texture 	 
*/  

float* rowIndex; 
float* mFilter;
size_t mPitch; 


__global__
void CreateFilter(float* fFilter, size_t fPitch, int N)
{
	// PI 
	float PI = 3.1416; 
	
	// Indecies (x,y)
	int i = threadIdx.x;
	int j = threadIdx.y; 
	
	// Filter Function 
	fFilter[(i * N) + j]  = (0.42) - (0.5 * cosf (2 * PI * (i) / (N-1))) + (0.08 * cosf (4 * PI * (i) / (N-1))); 
	fFilter[(i * N) + j] = fFilter[(i * N) + j] * (0.42 - 0.5 * cosf (2 * PI * (j) / (N-1)) + 0.08 * cosf (4 * PI * (j) / (N-1))); 
}

extern "C"
void CreateFilterGPU(dim3 grid, dim3 block, float* fFilter, size_t fPitch, int N, float* HostFilterCopied)
{
	// Filter 
	mFilter = fFilter; 
	
	// Pitch  
	mPitch = fPitch; 

	// Create Filter 
	CreateFilter <<< grid, block >>> (mFilter, fPitch, gpuFilterSize); 
	
	// Copy Back Filter Values to Check 
	cudaMemcpy(HostFilterCopied, mFilter, (sizeof(float) * gpuFilterSize * gpuFilterSize), cudaMemcpyDeviceToHost); 
}



// Get Pixel from 2D Slice Image, With Clamping to Slice Border
__device__ float2 GetPixel(int fX, int fY)
{
    // Only 2 Components (Real + Imaginary)
    float2 TexelValue = tex2D(inSliceTex, fX, fY);
    return TexelValue; 
}

__global__
void ProcessSliceOnCUDA(float2* fOutputSlice, int fSliceWidth, int fSliceHeight, int nSize, float* fFilter, size_t fPitch)
{
	float2 fTexValue;

	// Initialize Values
	float2 varTemp;
	varTemp.x = 0; 
	varTemp.y = 0;
	
	// Thread Index = Thread(x,y)
	int xThreadID = threadIdx.x;
	int yThreadID = threadIdx.y;
	
	// Block Index = Block(x,y)
	int fBlkWidth = blockDim.x;
	int fBlkHeight = blockDim.y;
	
	// Indecies (x,y)
	int x = blockIdx.x * fBlkWidth + xThreadID;
	int y = blockIdx.y * fBlkHeight + yThreadID;
	
	// One Side 
	int Half =  (nSize - 1) /2;
	
	/*
	// Filter Loop
	for (int u = -Half; u <= Half; u++)
	{	
		for (int v = -Half; v <= Half; v++)
		{
			if ((x + u) < 0 || (x + u) > 255 || (y + v) < 0 || (y + v) > 255)
			{
				varTemp = GetPixel(x,y); 
			}
			else 
			{		
				fTexValue = GetPixel(x + u, y + v);
				
				varTemp.x += (fTexValue.x * fFilter[((u + Half) * nSize) + (v + Half)]); 
				varTemp.y += (fTexValue.y * fFilter[((u + Half) * nSize) + (v + Half)]);
			}
		}
	}
	*/ 
	fTexValue = GetPixel(x, y);
	fOutputSlice[(y * fSliceWidth) + x].x = fTexValue.x; 
	fOutputSlice[(y * fSliceWidth) + x].y = fTexValue.y; 
	
	// Wait for Loads to Complete
	__syncthreads();
}
		   
extern "C"		   
void LaunchSliceProcessingOnCUDA
(dim3 grid, dim3 block, cudaArray* fInputSlice, float2* fOutputSlice, int fSliceWidth, int fSliceHeight)
{
	// Bind Input Slice Texture To CUDA Array (At Which Processing Shall Be Done) 
	cutilSafeCall(cudaBindTextureToArray(inSliceTex, fInputSlice));
	
	// Channel Format 
	struct cudaChannelFormatDesc fDesc; 
	cutilSafeCall(cudaGetChannelDesc(&fDesc, fInputSlice));
     	
	// gettimeofday(&gpustartTimer_, NULL);
	
	for (int i = 0; i < mLoopGPU; i++)
	{
		// Run CUDA Kernel 
		ProcessSliceOnCUDA <<< grid, block >>> (fOutputSlice, fSliceWidth, fSliceHeight, gpuFilterSize, mFilter, mPitch); 
	}

	// gettimeofday(&gpustopTimer_, NULL);
	// gpu_time = ((gpustopTimer_.tv_usec - gpustartTimer_.tv_usec) / 1000); 

	// printf("gpu time for %d is %0.15f mSec \n", mLoopGPU, gpu_time); 
	// printf("gpu normalized time for resampling process is %f mSec \n", gpu_time/mLoopGPU);
}

#endif // _FVR_CU_INTERFACE_CU_

 
