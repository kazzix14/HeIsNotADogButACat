#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include "compoment.h"
#include "image.h"

struct private_varibles
{
	GLuint img;
	pngInfo info;
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
					&(p_this->p_vars->info),
					GL_CLAMP,
					GL_NEAREST,
					GL_NEAREST);

}

void Image_deconstruct(Image* const p_this)
{
	free(p_this->p_vars);
}

void Image_put(Image* const p_this)
{
	GLuint pos[2];
	GLuint w = p_this->p_vars->info.Width;
	GLuint h = p_this->p_vars->info.Height;

	Compoment_get_position(&(p_this->parent), pos);
		
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

void Image_get_info(Image* const p_this, pngInfo* p_info)
{
	p_info = &(p_this->p_vars->info);
}
