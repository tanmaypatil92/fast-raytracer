#ifndef _RAY_H_
#define _RAY_H_

#include "Vector.h"

class Ray{
	public:
	Vector origin;
	Vector direction;
	Ray();
	Ray(Vector,Vector);
};
#endif