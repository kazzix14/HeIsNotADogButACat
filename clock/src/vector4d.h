#ifndef VECTOR4D_H
#define VECTOR4D_H

typedef struct vector4d
{
	double x,
	       y,
	       z,
	       w;
} Vector4D;

void Vector4D_set(Vector4D* const, const Vector4D*);
void Vector4D_set_zero(Vector4D* const);
void Vector4D_set_identity(Vector4D* const);
void Vector4D_set_unit_x(Vector4D* const);
void Vector4D_set_unit_y(Vector4D* const);
void Vector4D_set_unit_z(Vector4D* const);
void Vector4D_set_unit_w(Vector4D* const);
void Vector4D_add(Vector4D* const, const Vector4D*);
void Vector4D_sub(Vector4D* const, const Vector4D*);

#endif
