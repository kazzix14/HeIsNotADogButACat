#ifndef COMPONENT3D_H
#define COMPONENT3D_H

#include <GL/glut.h>

#include "vector3d.h"

struct private_varibles;
typedef struct component3d
{
	Vector3D position,
		 rotation,
		 scale;
	struct private_varibles* p_vars;
} Component3D;

Component3D* Component3D_construct();
void Component3D_deconstruct(Component3D* const p_this);

#endif
