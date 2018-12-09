#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "compoment.h"

struct private_varibles
{
	GLuint position[2];
};

void Compoment_construct(Compoment* const p_this, GLuint const pos_x, GLuint const pos_y)
{
	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;

	p_this->p_vars->position[0] = pos_x;
}

void Compoment_deconstruct(Compoment* const p_this)
{
	free(p_this->p_vars);
}

void Compoment_get_position(Compoment* const p_this, GLuint* p_rtrn)
{
	p_rtrn = p_this->p_vars->position;
}
