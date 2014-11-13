#include "Gz.h"
#include "Ray.h"

Ray::Ray()
{
	origin    = Vector(0,0,0);
	direction = Vector(1,0,0);
}
Ray::Ray(Vector org,Vector dir)
{
	origin    = org;
	direction = dir;
}