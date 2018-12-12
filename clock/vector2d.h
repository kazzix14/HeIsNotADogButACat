#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <GL/glut.h>

struct private_varibles;
typedef struct vector2d
{
	int x,
	    y;
} Vector2D;

void Vector2D_set(Vector2D* const, Vector2D* const);
void Vector2D_add(Vector2D* const, Vector2D* const);
void Vector2D_sub(Vector2D* const, Vector2D* const);

#endif
