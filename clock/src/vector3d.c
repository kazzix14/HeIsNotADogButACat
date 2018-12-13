#define GLUT_DISABLE_ATEXIT_HACK

#include "vector3d.h"

void Vector3D_set(Vector3D* const p_this, const Vector3D* p_pos)
{
	p_this->x = p_pos->x;
	p_this->y = p_pos->y;
	p_this->z = p_pos->z;
}

void Vector3D_set_zero(Vector3D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
}

void Vector3D_set_identity(Vector3D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 1.0;
	p_this->z = 1.0;
}

void Vector3D_set_unit_x(Vector3D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
}

void Vector3D_set_unit_y(Vector3D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 1.0;
	p_this->z = 0.0;
}

void Vector3D_set_unit_z(Vector3D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 1.0;
}

void Vector3D_add(Vector3D* const p_this, const Vector3D* p_oprnd)
{
	p_this->x += p_oprnd->x;
	p_this->y += p_oprnd->y;
	p_this->z += p_oprnd->z;
}

void Vector3D_sub(Vector3D* const p_this, const Vector3D* p_oprnd)
{
	p_this->x -= p_oprnd->x;
	p_this->y -= p_oprnd->y;
	p_this->z -= p_oprnd->z;
}
