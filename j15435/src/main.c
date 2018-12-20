/* 
 * main.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "transform2d.h"
#include "image2d.h"
#include "vector2d.h"
#include "view.h"
#include "animation2d.h"
#include "animation_controller2d.h"

void display(void);
void reshape(int, int);
void timer(int);
void change_color();
void change_backcolor();
void getWindowSize(int *x, int *y);
void put_num();
void set_num_pos(int, int, double, double);
void init();
void init_gl(int*, char**, GLuint, GLuint, char*);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);

View* view;
Animation2D* anim;
AnimationController2D* anmcnt;

int main(int argc, char **argv)
{
	init_gl(&argc, argv, 400, 400, "Handwritten Clock");
	init();
	view = View_new();

	anim = Animation2D_new();
	Animation2D_load(anim, "resource/animation/test", 2);
	Transform2D_set_default(anim->transform);

	anmcnt = AnimationController2D_new();
	AnimationController2D_add_animation(anmcnt, anim, "test");

	glutTimerFunc(100, timer, 0);
	glutMainLoop();

	return(0);
}

void display(void)
{
	int ww, wh;
	getWindowSize(&ww, &wh);

	glClear(GL_COLOR_BUFFER_BIT);

	Animation2D_play(anim, view);

	glFlush();
	glutSwapBuffers();
}

void timer(int value)
{
	// 30 fps
	glutTimerFunc(33, timer, 0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	view->screen_width = w;
	view->screen_height = h;

}


void getWindowSize(int *x, int *y)
{
	*x = glutGet(GLUT_WINDOW_WIDTH);
	*y = glutGet(GLUT_WINDOW_HEIGHT);
}

void init_gl(int *argc, char **argv, GLuint width, GLuint height, char *title)
{
	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
	glutPositionWindow(100, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GL_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
}

void init()
{

}

void mouse(int b, int s, int x, int y)
{

}

void keyboard(unsigned char key, int x, int y)
{
	//q esc
	if((key == 'q') || (key == 27))
		exit(0);
}
