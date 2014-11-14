#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>

class Vector{
	public:
	double x,y,z;
	Vector();
	Vector(double,double,double);
	Vector crossProduct(Vector);
	double dotProduct(Vector);
	Vector normalize();
	Vector subVector(Vector);
	Vector addVector(Vector);
	Vector scalarMult(double);
	double lengthVector();
	Vector negate();
  bool checkRange(float down, float up);
};
#endif