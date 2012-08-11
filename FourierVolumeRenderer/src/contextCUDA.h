#include "fourierVolumeRender.h"
#include <cutil_inline.h>

#ifndef _WIN32 
	//#include <shrUtils.h>
#else 
	#include "nvShaderUtils.h"
	#include "rendercheck_gl.h"
#endif

float* FilterGPUHost;


/*
	Initialize CUDA Context 
*/
void initCUDA()
{
	printf("Initializing CUDA .... \n");
	printf("	Now We Can Safely Initialize CUDA Context (OpenGL Context has Already been Created \n");  
	printf("	Setting GPU for CUDA/GL Interop. \n\n");
	cudaGLSetGLDevice (cutGetMaxGflopsDeviceId());
	
	 
	fReconstructedImage = (float2*) malloc (sizeof(float2) * mSliceWidth * mSliceHeight);
	
	cutilSafeCall(cudaMalloc((void**)&fReconstructedImageDevice, (sizeof(float2) *mSliceWidth * mSliceHeight))); 
	
	
	FilterGPUHost = (float*) malloc (sizeof(float) *  (mFilterSize * mFilterSize)); 
	 
	
	

	
	// Aloocating Filter
	
	cudaMallocPitch((void**)&mWindow, &mPitchWin, mFilterSize * sizeof(float), mFilterSize);
	cudaMallocPitch((void**)&mSinc2D, &mPitchSinc, mFilterSize * sizeof(float), mFilterSize);
	
	
	
	
	cudaMallocPitch((void**)&mKernel2D, &mPitchKernel, mFilterSize * sizeof(float), mFilterSize); 
	

}




 
// Process Slice on CUDA - Entry Point 
void TransferSliceToCUDA(int fImgWidth, int fImgHeight)
{
    // Extracted Slice from 3D Spectrum Texture is Sent to CUDA for Processing
    cudaArray *fExtractedTextureSlice; 

    // Reservation - Linking Slice Texture to CUDA Context 
    // Map Buffer Objects to Get CUDA Device Pointers
    cutilSafeCall
    (cudaGraphicsMapResources(1, &mCUDA_InputProcesstextureResource, 0));
    
    // Link Extracted Slice from OpenGL Context to CUDA Context 
    cutilSafeCall
    (cudaGraphicsSubResourceGetMappedArray(&fExtractedTextureSlice, mCUDA_InputProcesstextureResource, 0, 0));
    
    // Bind Texture Image to Buffer
    glBindTexture(GL_TEXTURE_2D, mProcessTextureID);
	
	// Replace a Texture of 2 float Components 
    // Replace the Old Image with the New Reconstructed One
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mSliceWidth, mSliceHeight, RG , GL_FLOAT, mFrameBufferArray);

    // Filter Creation 
	// Calculate Block Size
    dim3 block1(mFilterSize, mFilterSize, 1);
    
    // Calculate Grid Size
    dim3 grid1(mFilterSize / block1.x, mFilterSize / block1.y, 1); 
    
    
    CreateFilterGPU(grid1, block1, mKernel2D, mPitchKernel, mFilterSize, FilterGPUHost);
    
  
      
       // Calculate Block Size
    dim3 block(16, 16, 1);
    
    // Calculate Grid Size
    dim3 grid(fImgWidth / block.x, fImgHeight / block.y, 1); 
    
	
    printf("Filter Creation done successfully \n"); 
    
    // Execute CUDA Kernel on the Extratced Slice 
    LaunchSliceProcessingOnCUDA(grid, block, fExtractedTextureSlice, fReconstructedImageDevice, fImgWidth, fImgHeight);
    
    // Take the fReconstructedImage to be displayed on the OpenGL Side 
    
    cutilSafeCall(cudaMemcpy(fReconstructedImage, fReconstructedImageDevice, (sizeof(float2) * mSliceWidth * mSliceHeight), cudaMemcpyDeviceToHost)); 
    
  

    
    // Unlink All Buffers Connected to CUDA Contexts to be Used by OpenGL 
    cudaGraphicsUnmapResources(1, &mCUDA_InputProcesstextureResource, 0);
    
     // Release Buffers
    glBindTexture(GL_TEXTURE_2D, 0);
}



// Copy Image & Process it on CUDA Platform 
void ProcessSliceCUDA(int width, int height)
{
    // Run the CUDA Kernel
    TransferSliceToCUDA(width, height);

    
}


