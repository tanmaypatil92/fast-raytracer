#include "Gz.h"
#include "Vector.h"

Vector::Vector()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vector::Vector(double xval,double yval,double zval)
{
	x = xval;
	y = yval;
	z = zval;
}
Vector Vector::crossProduct(Vector v)
{
	Vector cross;
	cross.x = y*v.z - z*v.y;
	cross.y = z*v.x - x*v.z;
	cross.z = x*v.y - y*v.x;
	return cross;
}
double Vector::dotProduct(Vector v)
{
	return x*v.x+y*v.y+z*v.z;
}
Vector Vector::normalize()
{
	Vector norm;
	double root = lengthVector();
	norm.x = x/root;
	norm.y = y/root;
	norm.z = z/root;
	return norm;
}
Vector Vector::subVector(Vector v)
{
	Vector subtract;
	subtract.x = x - v.x;
	subtract.y = y - v.y;
	subtract.z = z - v.z;
	return subtract;
}
Vector Vector::addVector(Vector v)
{
	Vector add;
	add.x = x + v.x;
	add.y = y + v.y;
	add.z = z + v.z;
	return add;
}

Vector Vector::scalarMult(double val)
{
	Vector mult;
	mult.x = x*val;
	mult.y = y*val;
	mult.z = z*val;
	return mult;
}
double Vector::lengthVector()
{
	return sqrt(x*x+y*y+z*z);
}
Vector Vector::negate()
{
	return  Vector(x*-1,y*-1,z*-1);
}
bool Vector::checkRange(float down, float up)
{
  bool ret_value = true;
  return ret_value;
  
}

double dot(Vector A, Vector B)
{
	double dot;
	dot = A.x*B.x + A.y*B.y + A.z*B.z;
	return dot;
}

Vector add(Vector A, Vector B)
{
	Vector C;
	C.x = A.x + B.x;
	C.y = A.y + B.y;
	C.z = A.z + B.z;

	return C;
}
Vector add(Vector A, Vector B, Vector C)
{
	Vector D;
	D.x = A.x + B.x + C.x;
	D.y = A.y + B.y + C.y;
	D.z = A.z + B.z + C.z;

	return D;
}
Vector sub(Vector A, Vector B)
{
	Vector C;
	C.x = A.x - B.x;
	C.y = A.y - B.y;
	C.z = A.z - B.z;

	return C;
}
Vector cross(Vector A, Vector B)
{
	Vector C;
	C.x = A.y*B.z - A.z*B.y;
	C.y = A.z*B.x - A.x*B.z;
	C.z = A.x*B.y - A.y*B.x;

	return C;
}
