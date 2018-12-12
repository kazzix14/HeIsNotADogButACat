#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>

#include "component2d.h"
#include "vector2d.h"

struct private_varibles
{
};

Component2D* Component2D_construct()
{
	// allocate memory for private varibles
	return (struct private_varibles*)malloc(sizeof(struct private_varibles));
}

void Component2D_deconstruct(Component2D* const p_this)
{
	free(p_this->p_vars);
}
