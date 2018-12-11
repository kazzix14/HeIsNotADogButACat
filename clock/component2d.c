#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/gl.h>

#include "component2d.h"
#include "vector2d.h"

struct private_varibles
{
	Vector2D position;
	Vector2D rotation;
	Vector2D scale;
};

void Component2D_construct(Component2D* const p_this)
{
	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;
	
	Vector2D_construct(&(p_this->p_vars->position));
	Vector2D_construct(&(p_this->p_vars->rotation));
	Vector2D_construct(&(p_this->p_vars->scale));
}

void Component2D_deconstruct(Component2D* const p_this)
{
	free(p_this->p_vars);
}

void Component2D_get_position(Component2D* const p_this, Vector2D* const p_rtrn)
{
	Vector2D_equal(p_rtrn, &(p_this->p_vars->position));
}

void Component2D_set_position(Component2D* const p_this, Vector2D* const pos)
{
	Vector2D_equal(&(p_this->p_vars->position), pos);
}

void Component2D_move(Component2D* const p_this, Vector2D* const dist)
{
	Vector2D_add(&(p_this->p_vars->position), dist);
}
