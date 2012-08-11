#include "fourierVolumeRender.h"

	int u,v;
	int x,y;
void ProcessSlice()
{
	// Half of Filter Size 
	int Half = (N__ - 1) /2; 
	
	int ctr = 0; 
	for (int i = 0; i < mSliceWidth; i++)
	{
		for(int j = 0; j < mSliceHeight; j++)
		{
			inputSlice[i][j].x = mFrameBufferArray[ctr++];
			inputSlice[i][j].y = mFrameBufferArray[ctr++];	
		}
	}

	/// Timer start 
///	gettimeofday(&startTimer_, NULL);
	for (int times = 0; times < mLoopCPU; times++)
	{
	
		for (int i = 0; i < mSliceWidth; i++)
		{
			for(int j = 0; j < mSliceHeight; j++)
			{
				for (u = -Half; u <= Half; u++)
				{
					for (v = -Half; v <= Half; v++)
					{
						// Don't Process Outside Array Boundary 
						if ((i + u) < 0 || (i + u) > (mSliceWidth-1) || (j + v) < 0 || (j + v) > (mSliceHeight-1))
						{
							outputSlice[i][j].x += 0; // inputSlice[i][j].x;
							outputSlice[i][j].y += 0; // inputSlice[i][j].y;
						}
				
						else 
						{
							outputSlice[i][j].x += inputSlice[i + u][j + v].x * pKernel[u + Half][v + Half];
							outputSlice[i][j].y += inputSlice[i + u][j + v].y * pKernel[u + Half][v + Half];
						}
			
					}
			
				}
		
				// Normalizing Value 
				outputSlice[i][j].x /= N__;
				outputSlice[i][j].y /= N__;	
			} 
		}
	
	}
	
	// Stop Timer & Get Result 
	///gettimeofday(&stopTimer_, NULL);
	///cpu_time = ((stopTimer_.tv_sec - startTimer_.tv_sec) * 1000); 

	printf("cpu time for %d is %f mSec \n", mLoopCPU, cpu_time);
	printf("cpu normalized time for resampling process is %f mSec \n", cpu_time/mLoopCPU);
	  
	ctr = 0; 
	// Return it back to array
	for (int i = 0; i < mSliceWidth; i++)
	{ 
		for(int j = 0; j < mSliceHeight; j++)
		{
			mSliceArrayComplex[ctr][0] = outputSlice[i][j].x;
			mSliceArrayComplex[ctr][1] = outputSlice[i][j].y;
			ctr++;
		} 
	}	
}
 