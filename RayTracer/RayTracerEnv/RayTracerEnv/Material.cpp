#include "Material.h"

Material::Material()
{
	matColor = Color(0.5,0.5,0.5,1.0);
}
Material::Material(Color c)
{
	matColor = c;
}