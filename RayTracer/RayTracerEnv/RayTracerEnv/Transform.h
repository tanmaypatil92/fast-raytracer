#ifndef _TRANSFORM_H
#define _TRANSFORM_H_

#include "Vector.h"
#define PI 3.14159265

#ifndef GZDATA
#define GZDATA
 typedef float TMatrix[4][4];
#endif

class Transform{
public:
	void rotateX(Vector *vertex,float xRotation);
	void rotateY(Vector *vertex,float yRotation);
	void rotateZ(Vector *vertex,float zRotation);
	void scaleXYZ(Vector *vertex,float xScale,float yScale,float zScale);
	void translateXYZ(Vector *vertex,float xTranslate,float yTranslate,float zTranslate);
	float degToRad(float degree);
	void multiplyMatrix(Vector *vertex,TMatrix *m);
};
#endif
