#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H

#include "vector2d.h"
#include "vector4d.h"

typedef struct transform2d
{
	Vector2D position,
		 scale;
	Vector4D rotation;
} Transform2D;

Transform2D* Transform2D_new();
void Transform2D_release(Transform2D* const);
void Transform2D_set(Transform2D* const, const Transform2D*);
void Transform2D_set_default(Transform2D* const);

#endif
