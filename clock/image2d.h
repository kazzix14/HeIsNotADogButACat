#ifndef IMAGE2D_H
#define IMAGE2D_H

#include <GL/glut.h>
#include <GL/glpng.h>

#include "transform2d.h"
#include "view.h"

struct private_variables;
typedef struct image2d
{
	Transform2D* p_transform;	
	struct private_variables* p_vars;	
} Image2D;

Image2D* Image2D_new();
void Image2D_release(Image2D* const);
void Image2D_load(Image2D* const, const char*);
void Image2D_put(const Image2D*, const View*);
void Image2D_get_info(const Image2D*, pngInfo* const);
void Image2D_get_id(const Image2D*, GLuint* const);
void Image2D_get_size(const Image2D*, Vector2D* const);

#endif
