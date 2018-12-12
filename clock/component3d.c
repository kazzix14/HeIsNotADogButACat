#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>
#include "component3d.h"

struct private_varibles
{
};

Component3D* Component3D_construct()
{
	// allocate memory for private varibles
	return (struct private_varibles*)malloc(sizeof(struct private_varibles));
}

void Component3D_deconstruct(Component3D* const p_this)
{
	free(p_this->p_vars);
}
