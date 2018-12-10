#pragma once
#include <GL/glut.h>

struct private_varibles;
typedef struct component
{
	struct private_varibles* p_vars;	
} Component;

void Component_construct(Component* const p_this, GLuint const pos_x, GLuint const pos_y);
void Component_deconstruct(Component* const p_this);
void Component_get_position(Component* const p_this, GLuint p_rtrn[2]);
