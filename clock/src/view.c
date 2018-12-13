#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>

#include "view.h"
#include "vector3d.h"

struct private_variables
{
};

View* View_new()
{
	// allocate memory for private varibles
	View *v;
	v = (View*)malloc(sizeof(View));
	v->p_vars = (struct private_variables*)malloc(sizeof(struct private_variables));
	
	return v;
}

void View_release(View* const p_this)
{
	free(p_this->p_vars);
	free(p_this);
}

void View_begin_2d(const View* p_this)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, p_this->screen_width, p_this->screen_height, 0);

	//glScaled(1, -1, 1);
	//glTranslated(0, -h, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void View_begin_3d(const View* p_this)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(30.0, (double)p_this->screen_width / (double)p_this->screen_height, 1.0, 10000.0);

	glScaled(1, -1, 1);
	glTranslated(0, -p_this->screen_height, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	gluLookAt(0.0, 0.0, 2000.0,
		  0.0, -500.0, 0.0,
		  0.0, 1.0, 0.0);
}

void View_end()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
