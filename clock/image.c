#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include "compoment.h"
#include "image.h"

struct private_varibles
{
	GLuint img;
	pngInfo img_info;
};

void Image_construct(Image* const p_this, char* const image_path, GLuint const pos_x, GLuint const pos_y)
{

	Compoment_construct(&(p_this->parent), pos_x, pos_y);

	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;
	
	// load image
	p_this->p_vars->img = pngBind(image_path, 
					PNG_NOMIPMAP,
					PNG_ALPHA,
					&p_this->p_vars->img_info,
					GL_CLAMP,
					GL_NEAREST,
					GL_NEAREST);

}

void Image_deconstruct(Image* const p_this)
{
	free(p_this->p_vars);
}
