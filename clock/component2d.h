#ifndef COMPONENT2D_H
#define COMPONENT2D_H

#include <GL/glut.h>

#include "vector2d.h"

struct private_varibles;
typedef struct component2d
{
	struct private_varibles* p_vars;	
} Component2D;

void Component2D_construct(Component2D* const p_this);
void Component2D_deconstruct(Component2D* const p_this);
void Component2D_get_position(Component2D* const p_this, Vector2D* const p_rtrn);
void Component2D_set_position(Component2D* const p_this, Vector2D* const pos);
void Component2D_move(Component2D* const p_this, Vector2D* const dist);

#endif
