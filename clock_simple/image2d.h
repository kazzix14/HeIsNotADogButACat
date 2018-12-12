#ifndef IMAGE2D_H
#define IMAGE2D_H

#include <GL/glut.h>
#include <GL/glpng.h>

#include "component2d.h"

struct private_varibles;
typedef struct image2d
{
	Component2D parent;	
	struct private_varibles* p_vars;	
} Image2D;

void Image2D_construct(Image2D* const p_this, char* const image_path);
void Image2D_deconstruct(Image2D* const p_this);
void Image2D_put(Image2D* const p_this);
void Image2D_get_info(Image2D* const p_this, pngInfo* const p_rtrn);
void Image2D_get_id(Image2D* const p_this, GLuint* const p_rtrn);
void Image2D_get_size(Image2D* const p_this, Vector2D*);

#endif
