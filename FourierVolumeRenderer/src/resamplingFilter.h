// #define __DEBUG__

#include "fourierVolumeRender.h"

float** CreateFilter(int filterSize)
{
	float PI = 3.1416;   
	int N = filterSize; 
	float *mSlice, *mSinc1D, *mKernel1D; 
	
	mSlice = (float*) malloc (sizeof(float) * N); 
	mSinc1D = (float*) malloc (sizeof(float) * N);
	mKernel1D = (float*) malloc (sizeof(float) * N);

	float mSincRange = 3;
	float mSincIndex; 
	
	for (int i = 0; i < N; i++)
	{
		mSincIndex =  (i * (2 * mSincRange / (N-1))) - mSincRange;

		if (mSincIndex == 0)
			mSinc1D[i] = 1; 
		else 
			mSinc1D[i] = sin (PI * mSincIndex) / (PI * mSincIndex);	 
	}

	for (int i = 0; i < N; i++)
	{
		int idx = -(((N - 1) /2) - i); 
		mSlice[i] = 0.42 - 0.5 * cos (2 * PI * (idx + ((N - 1) / 2)) / (N-1)) + 0.08 * cos (4 * PI * (idx + ((N - 1) / 2)) / (N-1)); 
		mKernel1D[i] = mSlice[i] * mSinc1D[i]; 
	}

#ifdef __DEBUG__
	printf("1D Sinc Display \n");
	for (int i = 0; i < N; i++)
	{
		printf("%f \n", mSinc1D[i]);
	}

	printf("1D Slice Display \n");
	for (int i = 0; i < N; i++)
	{
		printf("%f \n", mKernel1D[i]);
	}
#endif 



float **mWindow, **mSinc2D, **mKernel2D; 
	mWindow = (float**)  malloc (N * N * sizeof(float*));
	mSinc2D = (float**)  malloc (N * N * sizeof(float*));
	mKernel2D = (float**)  malloc (N * N * sizeof(float*));
	
	for (int i = 0; i < N; i++)
	{
		mWindow[i] = (float*) malloc(N * sizeof(float));
		mSinc2D[i] = (float*) malloc(N * sizeof(float));
		mKernel2D[i] = (float*) malloc(N * sizeof(float));
	}  
	
	for (int i = 0; i < N; i++)
	{	
		for (int j = 0; j < N; j++) 
		{
			mKernel2D[i][j] = mKernel1D[i] * mKernel1D[j]; 
		}
	}

#ifdef __DEBUG__
	printf("2D Filter Display \n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) 
		{
			printf("%f \n", mKernel2D[i][j]);
		}
		printf("\n"); 
	}
#endif
	
	return mKernel2D; 
}