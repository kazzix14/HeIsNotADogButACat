#define GLUT_DISABLE_ATEXIT_HACK

#include "vector2d.h"

void Vector2D_set(Vector2D* const p_this, const Vector2D* p_pos)
{
	p_this->x = p_pos->x;
	p_this->y = p_pos->y;
}

void Vector2D_set_zero(Vector2D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
}

void Vector2D_set_identity(Vector2D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 1.0;
}
void Vector2D_set_unit_x(Vector2D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 0.0;
}

void Vector2D_set_unit_y(Vector2D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 1.0;
}

void Vector2D_add(Vector2D* const p_this, const Vector2D* p_oprnd)
{
	p_this->x += p_oprnd->x;
	p_this->y += p_oprnd->y;
}

void Vector2D_sub(Vector2D* const p_this, const Vector2D* p_oprnd)
{
	p_this->x -= p_oprnd->x;
	p_this->y -= p_oprnd->y;
}
