#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H

#include "vector3d.h"
#include "vector4d.h"

typedef struct transform3d
{
	Vector3D position,
		 scale;
	Vector4D rotation;
} Transform3D;

void Transform3D_release(Transform3D* const);
void Transform3D_set(Transform3D* const, const Transform3D*);
void Transform3D_set_default(Transform3D* const);

#endif
