#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "vector2d.h"

void Vector2D_set(Vector2D* const p_this, Vector2D* const p_pos)
{
	p_this->p_varsx = p_pos->p_varsx;
	p_this->p_varsy = p_pos->p_varsy;
}

void Vector2D_add(Vector2D* const p_this, Vector2D* const p_oprnd)
{
	p_this->p_varsx += p_oprnd->p_varsx;
	p_this->p_varsy += p_oprnd->p_varsy;
}

void Vector2D_sub(Vector2D* const p_this, Vector2D* const p_oprnd)
{
	p_this->p_varsx -= p_oprnd->p_varsx;
	p_this->p_varsy -= p_oprnd->p_varsy;
}
