#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>

#include "transform2d.h"
#include "image2d.h"
#include "vector2d.h"
#include "view.h"

struct private_variables
{
	GLuint img;
	pngInfo info;
};

Image2D* Image2D_new()
{
	// allocate memories
	Image2D* p_img;
	p_img = (Image2D*)malloc(sizeof(Image2D));
	p_img->p_vars = (struct private_variables*)malloc(sizeof(struct private_variables));
	p_img->p_transform = Transform2D_new();

	if(p_img == NULL || p_img->p_vars == NULL || p_img->p_transform == NULL)
		return NULL;

	return p_img;
}

void Image2D_release(Image2D* const p_this)
{
	free(p_this->p_vars);
	free(p_this);
}

void Image2D_load(Image2D* const p_this, const char* path)
{
	p_this->p_vars->img = pngBind(path, 
					PNG_NOMIPMAP,
					PNG_ALPHA,
					&(p_this->p_vars->info),
					GL_CLAMP,
					GL_NEAREST,
					GL_NEAREST);
}

void Image2D_put(const Image2D* p_this, const View* view)
{
	int w = p_this->p_vars->info.Width,
	    h = p_this->p_vars->info.Height;

	View_begin_2d(view);

	Transform2D* t = p_this->p_transform;
	glTranslatef(t->position.x, t->position.y, 0);
	glRotatef(t->rotation.z, t->rotation.x, t->rotation.y, 0);
	glScalef(t->scale.x, t->scale.y, 1);

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

	View_end();
}

void Image2D_get_info(const Image2D* p_this, pngInfo* const p_info)
{
	*p_info = p_this->p_vars->info;
}

void Image2D_get_id(const Image2D* p_this, GLuint* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->img;
}

void Image2D_get_size(const Image2D* p_this, Vector2D* const p_rtrn){
	p_rtrn->x  = p_this->p_vars->info.Width;
	p_rtrn->y =  p_this->p_vars->info.Height;
}
