#ifndef COMPONENT3D_H
#define COMPONENT3D_H

#include <GL/glut.h>

struct private_varibles;
typedef struct component3d
{
	struct private_varibles* p_vars;	
} Component3D;

void Component3D_construct(Component3D* const p_this);
void Component3D_deconstruct(Component3D* const p_this);
void Component3D_get_position(Component3D* const p_this, GLuint p_rtrn[3]);

#endif
