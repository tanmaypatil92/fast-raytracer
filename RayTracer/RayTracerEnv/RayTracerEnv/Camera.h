#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector.h"

class Camera{
	public:
	Vector position;
	Vector upVector;
	Vector lookAt;
	Vector camX,camY,camZ;
	Camera();
	Camera(const Vector);               // Constructor if only position Vector is given
	Camera(const Vector,const Vector);        // Constructor if only position and lookat Vectors are given
	Camera(const Vector,const Vector,const Vector); // Constructor if all three position,lookat,up Vectors are given
	int cameraXYZ(const Vector,const Vector,const Vector);
};

#endif