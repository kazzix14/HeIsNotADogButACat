/* 
 * view.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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
	Vector3D_set_zero(&(v->position));
	
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
	gluOrtho2D(p_this->position.x, p_this->position.x+p_this->window_width,
		   p_this->position.y+p_this->window_height, p_this->position.y);

	//glScaled(1, -1, 1);
	//glTranslated(0, -h, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void View_begin()
{
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void View_end()
{
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void View_end_2d()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
/*
char View_is_visible_2d(const View* this, const Vector2D* vec1)
{
	Vector2D vec2;
       	Vector3D_to_Vector2D(&this->position, &vec2);
	if(Vector2D_compare(vec1, &vec2) == 0x2){ 				// LB 0010
		if(Vector2D_compare(vec1, &vec2) == 0x2){ 			// 0010

			if(Vector2D_compare(vec1, &vec2) == 0x2){ 		// 0010

				if(Vector2D_compare(vec1, &vec2) == 0x2){ 	// 0010

	}	}	}	}			
	return 1;
}*/
