#include "Light.h"

Light::Light()
{
	direction  = Vector(0.0,0.0,0.0);
	lightColor = Color(1.0,1.0,1.0);
}
Light::Light(Color c)
{
	lightColor = c;
}
Light::Light(Vector d,Color c)
{
	lightColor = c;
	direction  = d;
}