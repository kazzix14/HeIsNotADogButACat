#define GLUT_DISABLE_ATEXIT_HACK

#include <stdlib.h>

#include "transform3d.h"
#include "vector3d.h"
#include "vector4d.h"

Transform3D* Transform3D_new()
{
	Transform3D* p_t;
	p_t = (Transform3D*)malloc(sizeof(Transform3D));
	return p_t;
}

void Transform3D_release(Transform3D* const p_this)
{
	free(&(p_this->p_vars));	
	free(p_this);
}

void Transform3D_set(Transform3D* const p_this, const Transform3D* p_oprnd)
{
	Vector3D_set(&(p_this->position), &(p_oprnd->position));
	Vector3D_set(&(p_this->rotation), &(p_oprnd->rotation));
	Vector3D_set(&(p_this->scale), &(p_oprnd->scale));
}

void Transform3D_set_default(Transform3D* const p_this)
{
	Vector3D_set_zero(&(p_this->position));
	Vector3D_set_identity(&(p_this->scale));
	Vector4D_set_zero(&(p_this->rotation));
}
