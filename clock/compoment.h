#pragma once
#include <GL/glut.h>

struct private_varibles;
typedef struct compoment
{
	struct private_varibles* p_vars;	
} Compoment;

void Compoment_construct(Compoment* const p_this, GLuint const pos_x, GLuint const pos_y);
void Compoment_deconstruct(Compoment* const p_this);
void Compoment_get_position(Compoment* const p_this, GLuint p_rtrn[2]);
