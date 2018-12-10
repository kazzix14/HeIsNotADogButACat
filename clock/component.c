#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "component.h"

struct private_varibles
{
	GLuint position[2];
};

void Component_construct(Component* const p_this, GLuint const pos_x, GLuint const pos_y)
{
	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;

	p_this->p_vars->position[0] = pos_x;
	p_this->p_vars->position[1] = pos_y;
}

void Component_deconstruct(Component* const p_this)
{
	free(p_this->p_vars);
}

void Component_get_position(Component* const p_this, GLuint p_rtrn[2])
{
	p_rtrn[0] = p_this->p_vars->position[0];
	p_rtrn[1] = p_this->p_vars->position[1];
}
