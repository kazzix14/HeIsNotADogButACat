#ifndef COMPONENT2D_H
#define COMPONENT2D_H

#include <GL/glut.h>

#include "vector2d.h"

struct private_varibles;
typedef struct component2d
{
	Vector2D position,
	 	 rotation,
		 scale;
	struct private_varibles* p_vars;	
} Component2D;

Component2D* Component2D_construct();
void Component2D_deconstruct(Component2D* const p_this);

#endif
