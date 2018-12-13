#ifndef VECTOR3D_H
#define VECTOR3D_H

typedef struct vector3d
{
	double x,
	       y,
	       z;
} Vector3D;

void Vector3D_set(Vector3D* const, const Vector3D*);
void Vector3D_set_zero(Vector3D* const);
void Vector3D_set_identity(Vector3D* const);
void Vector3D_set_unit_x(Vector3D* const);
void Vector3D_set_unit_y(Vector3D* const);
void Vector3D_set_unit_z(Vector3D* const);
void Vector3D_add(Vector3D* const, const Vector3D*);
void Vector3D_sub(Vector3D* const, const Vector3D*);

#endif
