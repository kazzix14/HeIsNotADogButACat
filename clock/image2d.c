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

void Image2D_construct(Image2D* const p_this, char* const image_path)
{

	Component2D_construct(&(p_this->parent));

	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;
	
	// load image
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
	Vector2D pos;
	Vector2D_construct(&pos);
	Component2D_get_position(&(p_this->parent), &pos);

	int w = p_this->p_vars->info.Width,
	    h = p_this->p_vars->info.Height,
	    x,
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
