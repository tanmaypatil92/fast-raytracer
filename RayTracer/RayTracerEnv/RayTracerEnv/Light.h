#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vector.h"
#include "Color.h"

class Light{
	public:
	Vector direction;
	Color  lightColor;
	Light();
	Light(Color);       //Constructor for ambient light
	Light(Vector,Color);
};
#endif