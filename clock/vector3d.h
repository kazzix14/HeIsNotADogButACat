#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <GL/glut.h>

struct private_varibles;
typedef struct vector3d
{
	struct private_varibles* p_vars;	
} Vector3D;

void Vector3D_construct(Vector3D* const);
void Vector3D_release(Vector3D* const);
void Vector3D_get_x(Vector3D* const, int* const);
void Vector3D_get_y(Vector3D* const, int* const);
void Vector3D_get(Vector3D* const, int [3]);
void Vector3D_set_x(Vector3D* const, int const);
void Vector3D_set_y(Vector3D* const, int const);
void Vector3D_set(Vector3D* const, int const, int const);
void Vector3D_equal(Vector3D* const, Vector3D* const);
void Vector3D_add(Vector3D* const, Vector3D* const);
void Vector3D_sub(Vector3D* const, Vector3D* const);

#endif
