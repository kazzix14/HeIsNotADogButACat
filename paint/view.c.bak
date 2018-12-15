#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>

#include "view.h"

struct private_varibles
{
	Vector3D position;
	Vector3D traget_position;
	Vector3D up_vetor;
};

void View_construct(View* const p_this)
{
	// allocate memory for private varibles
	struct private_varibles* p;
	p = (struct private_varibles*)malloc(sizeof(struct private_varibles));
	p_this->p_vars = p;

	View3D_construct(&(p_this->p_vars->position));
}

void View_deconstruct(View* const p_this)
{
	free(p_this->p_vars);
}

void View_begin_2d(View* const p_this, int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);

	//glScaled(1, -1, 1);
	//glTranslated(0, -h, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void View_begin_3d(View* const p_this, int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 10000.0);

	glScaled(1, -1, 1);
	glTranslated(0, -h, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	Vector3D_get(&p_this);
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

void View_set_position(View* const p_this, Vector* const p_vec)
{
	Vector3D_equal(p_this->position, p_vec):
}

void View_set_target_position(View* const p_this, Vector* const p_vec)
{
	Vector3D_equal(p_this->target_position, p_vec):
}

void View_set_up_vector(View* const p_this, Vector* const p_vec)
{
	Vector3D_equal(p_this->up_vector, p_vec):
}
