#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <GL/glut.h>

struct private_varibles;
typedef struct vector3d
{
	int x,
	    y,
	    z;
} Vector3D;

void Vector3D_set(Vector3D* const, Vector3D* const);
void Vector3D_add(Vector3D* const, Vector3D* const);
void Vector3D_sub(Vector3D* const, Vector3D* const);

#endif
