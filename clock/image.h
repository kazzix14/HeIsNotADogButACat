#pragma once

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
void Image_get_info(Image* const p_this, pngInfo* p_info);
