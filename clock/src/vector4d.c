#define GLUT_DISABLE_ATEXIT_HACK

#include "vector4d.h"

void Vector4D_set(Vector4D* const p_this, const Vector4D* p_pos)
{
	p_this->x = p_pos->x;
	p_this->y = p_pos->y;
	p_this->z = p_pos->z;
	p_this->w = p_pos->w;
}

void Vector4D_set_zero(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
	p_this->w = 0.0;
}

void Vector4D_set_identity(Vector4D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 1.0;
	p_this->z = 1.0;
	p_this->w = 1.0;
}

void Vector4D_set_unit_x(Vector4D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
	p_this->w = 0.0;
}

void Vector4D_set_unit_y(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 1.0;
	p_this->z = 0.0;
	p_this->w = 0.0;
}

void Vector4D_set_unit_z(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 1.0;
	p_this->w = 0.0;
}

void Vector4D_set_unit_w(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
	p_this->w = 1.0;
}

void Vector4D_add(Vector4D* const p_this, const Vector4D* p_oprnd)
{
	p_this->x += p_oprnd->x;
	p_this->y += p_oprnd->y;
	p_this->z += p_oprnd->z;
	p_this->w += p_oprnd->w;
}

void Vector4D_sub(Vector4D* const p_this, const Vector4D* p_oprnd)
{
	p_this->x -= p_oprnd->x;
	p_this->y -= p_oprnd->y;
	p_this->z -= p_oprnd->z;
	p_this->w -= p_oprnd->w;
}
