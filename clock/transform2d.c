#define GLUT_DISABLE_ATEXIT_HACK

#include <stdlib.h>

#include "transform2d.h"
#include "vector2d.h"
#include "vector3d.h"

Transform2D* Transform2D_new()
{
	Transform2D* p_t;
	p_t = (Transform2D*)malloc(sizeof(Transform2D));
	return p_t;
}	

void Transform2D_release(Transform2D* const p_this)
{
	free(&(p_this->p_vars));
	free(p_this);
}

void Transform2D_set(Transform2D* const p_this, const Transform2D* p_oprnd)
{
	Vector2D_set(&(p_this->position), &(p_oprnd->position));
	Vector2D_set(&(p_this->scale), &(p_oprnd->scale));
	Vector3D_set(&(p_this->rotation), &(p_oprnd->rotation));
}

void Transform2D_set_default(Transform2D* const p_this)
{
	Vector2D_set_zero(&(p_this->position));
	Vector2D_set_identity(&(p_this->scale));
	Vector3D_set_zero(&(p_this->rotation));
}
