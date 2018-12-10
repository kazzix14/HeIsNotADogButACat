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

	Component_construct(&(p_this->parent), pos_x, pos_y);

	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;
	
	// load image
	p_this->p_vars->img = pngBind(image2d_path, 
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
	int w = p_this->p_vars->info.Width;
	int h = p_this->p_vars->info.Height;

	Component_get_position(&(p_this->parent), &pos);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p_this->p_vars->img);
	
	glColor4ub(255, 255, 255, 255);
	
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); 
	glVertex2i(pos[0], pos[1]);
	
	glTexCoord2i(0, 1);
	glVertex2i(pos[0], pos[1] + h);
	
	glTexCoord2i(1, 1);
	glVertex2i(pos[0] + w, pos[1] + h);
	
	glTexCoord2i(1, 0);
	glVertex2i(pos[0] + w, pos[1]);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
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
	p_rtrn[0] = p_this->p_vars->info.Width;
	p_rtrn[1] = p_this->p_vars->info.Height;
}
