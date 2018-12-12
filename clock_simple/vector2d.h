#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <GL/glut.h>

struct private_varibles;
typedef struct vector2d
{
	struct private_varibles* p_vars;	
} Vector2D;

void Vector2D_construct(Vector2D* const);
void Vector2D_release(Vector2D* const);
void Vector2D_get_x(Vector2D* const, int* const);
void Vector2D_get_y(Vector2D* const, int* const);
void Vector2D_get(Vector2D* const, int [2]);
void Vector2D_set_x(Vector2D* const, int const);
void Vector2D_set_y(Vector2D* const, int const);
void Vector2D_set(Vector2D* const, int const, int const);
void Vector2D_equal(Vector2D* const, Vector2D* const);
void Vector2D_add(Vector2D* const, Vector2D* const);
void Vector2D_sub(Vector2D* const, Vector2D* const);

#endif
