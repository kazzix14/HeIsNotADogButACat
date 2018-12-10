#pragma once

#include <GL/glut.h>
#include <GL/glpng.h>

#include "compoment.h"

struct private_varibles;
typedef struct image
{
	Compoment parent;	
	struct private_varibles* p_vars;	
} Image;

void Image_construct(Image* const p_this, char* const image_path, GLuint const pos_x, GLuint const pos_y);
void Image_deconstruct(Image* const p_this);
void Image_put(Image* const p_this);
void Image_get_info(Image* const p_this, pngInfo* const p_info);
void Image_get_id(Image* const p_this, GLuint* const p_rtrn);
void Image_get_size(Image* const p_this, GLuint p_rtrn[2]);
