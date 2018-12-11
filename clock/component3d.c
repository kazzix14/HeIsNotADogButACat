#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "component3d.h"

struct private_varibles
{
	GLuint position[3];
	GLuint rotation[3];
	GLuint scale[3];
};

void Component3D_construct(Component3D* const p_this)
{
	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;
}

void Component3D_deconstruct(Component3D* const p_this)
{
	free(p_this->p_vars);
}

void Component3D_get_position(Component3D* const p_this, GLuint p_rtrn[3])
{
	p_rtrn[0] = p_this->p_vars->position[0];
	p_rtrn[1] = p_this->p_vars->position[1];
	p_rtrn[2] = p_this->p_vars->position[2];
}
