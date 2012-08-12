#include "fourierVolumeRender.h"

void ProcessSlice()
{
	int ctr = 0; 
	for (int i = 0; i < mSliceWidth; i++)
	{
		for(int j = 0; j < mSliceHeight; j++)
		{
			inputSlice[i][j].x = mFrameBufferArray[ctr++];
			inputSlice[i][j].y = mFrameBufferArray[ctr++];	
		}
    }

	ctr = 0; 
	for (int i = 0; i < mSliceWidth; i++)
	{ 
		for(int j = 0; j < mSliceHeight; j++)
		{
            mSliceArrayComplex[ctr][0] = inputSlice[i][j].x;
            mSliceArrayComplex[ctr][1] = inputSlice[i][j].y;
			ctr++;
		} 
	}	

}
 
