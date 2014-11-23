#include "Transform.h"
#include "math.h"

void Transform::rotateX(Vector *vertex,float xRotation){
	float angle = degToRad(xRotation);
	TMatrix rotX = {
						{1,			0,		0,		0},
						{0,	static_cast<float>(cos(angle)),	static_cast<float>(-sin(angle)),0},
						{0,	static_cast<float>(sin(angle)),	 static_cast<float>(cos(angle)),0},
						{0,			0,		0,		1}
					};
	multiplyMatrix(vertex,&rotX);
}
void Transform::rotateY(Vector *vertex,float yRotation){
	float angle = degToRad(yRotation);
	TMatrix rotY = {
						{static_cast<float>(cos(angle)),	0,	static_cast<float>(sin(angle)),	0},
						{0,			1,	0,			0},
						{static_cast<float>(-sin(angle)),0,	static_cast<float>(cos(angle)),	0},
						{0,			0,	0,			1}
					};
	multiplyMatrix(vertex,&rotY);
}
void Transform::rotateZ(Vector *vertex,float zRotation){
	float angle = degToRad(zRotation);
	TMatrix rotZ = {
						{static_cast<float>(cos(angle)),static_cast<float>(-sin(angle)),	0,	0},
						{static_cast<float>(sin(angle)),static_cast<float>(cos(angle)),	0,	0},
						{0,					0,	1,	0},
						{0,					0,	0,	1}
				   };
	multiplyMatrix(vertex,&rotZ);
}
void Transform::scaleXYZ(Vector *vertex,float xScale,float yScale,float zScale){
	TMatrix scale = {
						{xScale,	0,		0,		0},
						{	0,	yScale,	0,		0},
						{	0,	0,		zScale,	0},
						{	0,	0,		0,		1}
				    };
	multiplyMatrix(vertex,&scale);
}
void Transform::translateXYZ(Vector *vertex,float xTranslate,float yTranslate,float zTranslate){
	TMatrix translate = {
						{1,	0,	0,	xTranslate},
						{0,	1,	0,	yTranslate},
						{0,	0,	1,	zTranslate},
						{0,	0,	0,		1}
					    };
	multiplyMatrix(vertex,&translate);
}
float Transform::degToRad(float degree){
	return degree * PI / 180.0;
}
void Transform::multiplyMatrix(Vector *vertex,TMatrix *m){
	Vector *v = vertex;
	v->x = (*m)[0][0]*vertex->x+ (*m)[0][1]*vertex->y + (*m)[0][2]*vertex->z + (*m)[0][3];
	v->y = (*m)[1][0]*vertex->x+ (*m)[1][1]*vertex->y + (*m)[1][2]*vertex->z + (*m)[1][3];
	v->z = (*m)[2][0]*vertex->x+ (*m)[2][1]*vertex->y + (*m)[2][2]*vertex->z + (*m)[2][3];
	vertex = v;
}