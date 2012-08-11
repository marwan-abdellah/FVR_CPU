#include "fourierVolumeRender.h"
void InitVars()
{
	printf("Initializing Variables ... \n\n"); 
	
	// Volume Attributes
	mVolWidth			= 256; 
	mVolHeight			= 256; 
	mVolDepth			= 256;
	mUniDim 			= 256; 
	mVolArea 			= mVolWidth * mVolHeight; 
	mVolumeSize 		= mVolWidth * mVolHeight * mVolDepth;
	mVolumeSizeBytes	= mVolumeSize * sizeof(mVolumeType);
	
	printf("Loaded Volume Attributes  \n"); 
	printf("	Volume Width	: %d \n", mVolWidth);
	printf("	Volume Height 	: %d \n", mVolHeight);
	printf("	Volume Depth	: %d \n", mVolDepth);
	printf("	Volume Unified Dimension : %d \n", mUniDim);
	printf("	Volume Size	: [%d] x [%d] x [%d] = %d \n", mVolWidth, mVolHeight, mVolDepth, mVolumeSize);
	printf("	Volume Size (Bytes) : [%d] Bytes \n", mVolumeSizeBytes);
	printf("	Volume Size (MBytes): [%d] MBytes \n", mVolumeSizeBytes / (1024 * 1024));
	printf("	Volume Cross-Section Area	: [%d] x [%d] = %d \n\n", mVolWidth, mVolHeight, mVolArea); 
	
	// Slice Attributes
	mSliceWidth 	= mVolWidth; 
	mSliceHeight 	= mVolHeight;
	mSliceSize = mSliceWidth * mSliceHeight; 
	
	printf("Extracted Slice Attributes  \n"); 
	printf("	Slice Width 	: %d \n", mSliceWidth);
	printf("	Slice Height 	: %d \n", mSliceHeight);
	printf("	Slice Area : [%d] x [%d] = %d Pixels \n\n", mSliceWidth, mSliceHeight, mSliceSize);
	
	// Texture Attributes
	mNumTexels 		= mSliceSize; 
	mNumValues 		= mSliceSize * 1; 
	mTextureDataSize 	= mNumValues * sizeof(float);
	
	printf("Texture Parameters  \n"); 
	printf("	Number of Texels in Slice : %d \n", mNumTexels);
	printf("	Number of Color Components : %d \n", mNumValues);
	printf("	Texture Data Size (kBytes): [%d] kBytes \n\n", mTextureDataSize/1024); 

	N__ = 9;  
}
