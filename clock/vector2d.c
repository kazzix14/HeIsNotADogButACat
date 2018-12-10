#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "vector2d.h"

struct private_varibles
{
	int pos[2];
};

void Vector2D_construct(Vector2D* const p_this)
{
	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;
}

void Vector2D_deconstruct(Vector2D* const p_this)
{
	free(p_this->p_vars);
}

void Vector2D_get_x(Vector2D* const p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->pos[0];
}

void Vector2D_get_y(Vector2D* const p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->pos[1];
}

void Vector2D_get(Vector2D* const p_this, int p_rtrn[2])
{
	p_rtrn[0] = p_this->p_vars->pos[0];
	p_rtrn[1] = p_this->p_vars->pos[1];
}

void Vector2D_set_x(Vector2D* const p_this, int const pos_x)
{
	p_this->p_vars->pos[0] = pos_x;
}

void Vector2D_set_y(Vector2D* const p_this, int const pos_y)
{
	p_this->p_vars->pos[1] = pos_y;
}

void Vector2D_set(Vector2D* const p_this, int const pos[2])
{
	p_this->p_vars->pos[0] = pos[0];
	p_this->p_vars->pos[1] = pos[1];
}

void Vector2D_equal(Vector2D* const p_this, Vector2D* const p_pos)
{
	p_this->p_vars->pos[0] = p_pos->p_vars->pos[0];
	p_this->p_vars->pos[1] = p_pos->p_vars->pos[1];
}

void Vector2D_add(Vector2D* const p_this, Vector2D* const p_oprnd)
{
	p_this->p_vars->pos[0] += p_oprnd->p_vars->pos[0];
	p_this->p_vars->pos[1] += p_oprnd->p_vars->pos[1];
}

void Vector2D_sub(Vector2D* const p_this, Vector2D* const p_oprnd)
{
	p_this->p_vars->pos[0] -= p_oprnd->p_vars->pos[0];
	p_this->p_vars->pos[1] -= p_oprnd->p_vars->pos[1];
}
