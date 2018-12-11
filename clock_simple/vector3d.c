#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "vector3d.h"

struct private_varibles
{
	int pos[3];
};

void Vector3D_construct(Vector3D* const p_this)
{
	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;
}

void Vector3D_release(Vector3D* const p_this)
{
	free(p_this->p_vars);
}

void Vector3D_get_x(Vector3D* const p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->pos[0];
}

void Vector3D_get_y(Vector3D* const p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->pos[1];
}

void Vector3D_get_z(Vector3D* const p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->pos[2];
}

void Vector3D_get(Vector3D* const p_this, int pos[3])
{
	pos[0] = p_this->p_vars->pos[0];
	pos[1] = p_this->p_vars->pos[1];
	pos[2] = p_this->p_vars->pos[2];
}

void Vector3D_set_x(Vector3D* const p_this, int const pos_x)
{
	p_this->p_vars->pos[0] = pos_x;
}

void Vector3D_set_y(Vector3D* const p_this, int const pos_y)
{
	p_this->p_vars->pos[1] = pos_y;
}

void Vector3D_set_z(Vector3D* const p_this, int const pos_y, int const pos_z)
{
	p_this->p_vars->pos[1] = pos_y;
}

void Vector3D_set(Vector3D* const p_this, int const pos_x, int const pos_y, int const pos_z)
{
	p_this->p_vars->pos[0] = pos_x;
	p_this->p_vars->pos[1] = pos_y;
	p_this->p_vars->pos[2] = pos_z;
}

void Vector3D_equal(Vector3D* const p_this, Vector3D* const p_pos)
{
	p_this->p_vars->pos[0] = p_pos->p_vars->pos[0];
	p_this->p_vars->pos[1] = p_por->p_vars->pos[1];
	p_this->p_vars->pos[2] = p_por->p_vars->pos[2];
}

void Vector3D_add(Vector3D* const p_this, Vector3D* const p_oprnd)
{
	p_this->p_vars->pos[0] += p_oprnd->p_vars->pos[0];
	p_this->p_vars->pos[1] += p_oprnd->p_vars->pos[1];
	p_this->p_vars->pos[2] += p_oprnd->p_vars->pos[2];
}

void Vector3D_sub(Vector3D* const p_this, Vector3D* const p_oprnd)
{
	p_this->p_vars->pos[0] -= p_oprnd->p_vars->pos[0];
	p_this->p_vars->pos[1] -= p_oprnd->p_vars->pos[1];
	p_this->p_vars->pos[2] -= p_oprnd->p_vars->pos[2];
}
