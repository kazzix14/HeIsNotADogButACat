#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "vector3d.h"

void Vector3D_set(Vector3D* const p_this, Vector3D* const p_pos)
{
	p_this->x = p_pos->x;
	p_this->y = p_pos->y;
	p_this->z = p_pos->z;
}

void Vector3D_add(Vector3D* const p_this, Vector3D* const p_oprnd)
{
	p_this->x += p_oprnd->x;
	p_this->y += p_oprnd->y;
	p_this->z += p_oprnd->z;
}

void Vector3D_sub(Vector3D* const p_this, Vector3D* const p_oprnd)
{
	p_this->x -= p_oprnd->x;
	p_this->y -= p_oprnd->y;
	p_this->z -= p_oprnd->z;
}
