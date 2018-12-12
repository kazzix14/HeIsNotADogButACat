#ifndef VECTOR2D_H
#define VECTOR2D_H

typedef struct vector2d
{
	int x,
	    y;
} Vector2D;

void Vector2D_set(Vector2D* const, const Vector2D* );
void Vector2D_set_zero(Vector2D* const);
void Vector2D_set_identity(Vector2D* const);
void Vector2D_set_unit_x(Vector2D* const);
void Vector2D_set_unit_y(Vector2D* const);
void Vector2D_add(Vector2D* const, const Vector2D*);
void Vector2D_sub(Vector2D* const, const Vector2D*);

#endif
