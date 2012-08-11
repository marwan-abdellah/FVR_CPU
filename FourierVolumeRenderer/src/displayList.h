#include "fourierVolumeRender.h"

void SetDisplayList(float fCenter)
{
	printf("Creating Display List ... \n"); 
	
	// Center (Z = 0), Side Lenght = 1, Just 1 Slice, 4 Vertices, 3 Coordinates 	
	float mCenter		= fCenter;
	float mSide			= 0.5;
	int nSlices			= 1;
	int nElements		= 4 * nSlices;
	GLfloat *vPoints	= new GLfloat [3 * nElements];
	// GLfloat *ptr		= vPoints;

	// Fill the Display List with Vertecies 
	//*(ptr++) = -mSide;
	//*(ptr++) = -mSide;
	//*(ptr++) =  mCenter;

	//*(ptr++) =  mSide;
	//*(ptr++) = -mSide;
	//*(ptr++) =  mCenter;

	//*(ptr++) =  mSide;
	//*(ptr++) =  mSide;
	//*(ptr++) =  mCenter;

	//*(ptr++) = -mSide;
	//*(ptr++) =  mSide;
	//*(ptr++) =  mCenter;

	vPoints[0] = -mSide;
	vPoints[1] = -mSide;
	vPoints[2] =  mCenter;

	vPoints[3] =  mSide;
	vPoints[4] = -mSide;
	vPoints[5] =  mCenter;

	vPoints[6] =  mSide;
	vPoints[7] =  mSide;
	vPoints[8] =  mCenter;

	vPoints[9] = -mSide;
	vPoints[10] =  mSide;
	vPoints[11] =  mCenter;
	// Fill the Display List with Vertecies 

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vPoints);
	mDiaplayList = glGenLists(1);
	glNewList(mDiaplayList, GL_COMPILE);
	glDrawArrays(GL_QUADS, 0, nElements); 
	glEndList();
	delete [] vPoints;
	
	printf("	Display List Created Successfully \n\n"); 
}
