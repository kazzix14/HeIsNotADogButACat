#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>

#include "component2d.h"
#include "image2d.h"
#include "vector2d.h"

struct private_varibles
{
	GLuint img;
	pngInfo info;
};

Image* Image2D_construct(Image2D* const p_this)
{

	Component2D_construct(&(p_this->parent));

	// allocate memory for private varibles
	return (struct private_varibles*)malloc(sizeof(struct private_varibles));
}

void Image2D_load_image(const char* path)
{
	p_this->p_vars->img = pngBind(image_path, 
					PNG_NOMIPMAP,
					PNG_ALPHA,
					&(p_this->p_vars->info),
					GL_CLAMP,
					GL_NEAREST,
					GL_NEAREST);
}
void Image2D_put(Image2D* const p_this)
{
	int w = p_this->p_vars->info.Width,
	    h = p_this->p_vars->info.Height,
	    x = p_this->,
	    y;

	Vector2D_get_x(&pos, &x),
        Vector2D_get_y(&pos, &y);

	Vector2D_release(&pos);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p_this->p_vars->img);
	
	glColor4ub(255, 255, 255, 255);
	
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); 
	glVertex2i(0, 0);
	
	glTexCoord2i(0, 1);
	glVertex2i(0, h);
	
	glTexCoord2i(1, 1);
	glVertex2i(w, h);
	
	glTexCoord2i(1, 0);
	glVertex2i(w, 0);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	//glPopMatrix();
}

void Image2D_get_info(Image2D* const p_this, pngInfo* const p_info)
{
	*p_info = p_this->p_vars->info;
}

void Image2D_get_id(Image2D* const p_this, GLuint* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->img;
}

void Image2D_get_size(Image2D* const p_this, Vector2D* const p_rtrn){
	Vector2D_set_x(p_rtrn, p_this->p_vars->info.Width);
	Vector2D_set_y(p_rtrn, p_this->p_vars->info.Height);
}
