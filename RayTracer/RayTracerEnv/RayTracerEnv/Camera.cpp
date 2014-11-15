#include "Gz.h"
#include "Vector.h"
#include "Camera.h"
#include "math.h"

#define RAD_CONV_CONST (M_PI/180.0)


Camera::Camera()
{
	position = Vector(10.0,10.0,10.0);
	upVector = Vector(0.0,1.0,0.0);
	lookAt   = Vector(0.0,0.0,0.0);
	cameraXYZ(position,upVector,lookAt);
}
Camera::Camera(const Vector pos)
{
	position = pos;
	upVector = Vector(0.0,1.0,0.0);
	lookAt   = Vector(0.0,0.0,0.0);
	cameraXYZ(position,upVector,lookAt);
}
Camera::Camera(const Vector pos,const Vector look)
{
	position = pos;
	lookAt   = look;
	upVector = Vector(0.0,1.0,0.0);
	cameraXYZ(position,upVector,lookAt);
}
Camera::Camera(Vector pos,Vector up,Vector look)
{
	position = pos;
	upVector = up;
	lookAt   = lookAt;
	cameraXYZ(position,upVector,lookAt);
}

#if 0
	this->camZ = lookAt.subVector(pos);
	this->camZ = camZ.normalize(); // Reversed and negative Z, if lookat subrated from pos ??
	this->camX = (camZ.crossProduct(up)).normalize(); //@todo : it is actually this->camX
	this->camY = camX.crossProduct(camZ); //Because Z and Y are normalized this should be normalized??
	return SUCCESS;

  this->camZ = lookAt.subVector(pos);
	this->camZ = camZ.normalize(); // Reversed and negative Z, if lookat subrated from pos ??
	this->camY = (up.crossProduct(camZ)).normalize(); //@todo : it is actually this->camX
	this->camX = camZ.crossProduct(camY); //Because Z and Y are normalized this should be normalized??
#endif

int Camera::cameraXYZ( Vector pos, Vector up, Vector look)
{
	this->camZ = lookAt.subVector(pos);
	this->camZ = camZ.normalize(); // Reversed and negative Z, if lookat subrated from pos ??
	this->camX = (camZ.crossProduct(up)).normalize(); //@todo : it is actually this->camX
	this->camY = camX.crossProduct(camZ); //Because Z and Y are normalized this should be normalized??
  return SUCCESS;
}

/*  @todo, hack again, how should the aspect ratio be sent by the application? */
void Camera::setFOV(double newFOV, double aspectRatio)
{
  double rad = tan((newFOV/2.0) * RAD_CONV_CONST);
  
  this->FOV = newFOV;
  this->width  = rad * 2.0;
  this->height = rad * aspectRatio * 2.0;

}
