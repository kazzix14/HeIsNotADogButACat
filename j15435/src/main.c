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
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

#include "transform2d.h"
#include "image2d.h"
#include "vector2d.h"
#include "view.h"
#include "animation2d.h"
#include "animation_controller2d.h"
#include "block2d.h"
#include "grid3d.h"
#include "object.h"
#include "audio.h"
#include "debug.h"
#include "keyboard.h"

void display(void);
void reshape(int, int);
void timer(int);
void change_color();
void change_backcolor();
void getWindowSize(int *x, int *y);
void put_num();
void set_num_pos(int, int, double, double);
void* init(void*);
void fps();
void init_gl(int*, char**, GLuint, GLuint, char*);
void mouse(int, int, int, int);
void keyboard();

View* view;

Object *obj;
Object *obj_child;
Image2D* img;

Animation2D* anm2d;

Audio* poop_audio;
int main(int argc, char **argv)
{
	init_gl(&argc, argv, 1600, 900, "UNTI!!");
	view = View_new();
	view->position.z = 1;

	img = Image2D_new();
	Image2D_load(img, "resource/animation/test/0.png");

	obj = Object_new();
	obj_child = Object_new();
	anm2d = Animation2D_new();

	Object_add_component(obj, obj_child);
	Object_add_component(obj, anm2d);
	Object_add_component(obj_child, img);

	Animation2D_add_frame(anm2d);
	Animation2D_set_frame_length(anm2d, 0, 1);
	// anm, frame index, varaddr, valaddr, sizeofval, smoothingType
	typeof(obj_child->transform->position.y) valtest = 50;
	Animation2D_add_animated_variable(anm2d, 0, &(obj_child->transform->position.y), &valtest, sizeof(obj_child->transform->position.y), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	valtest = 20;
	Animation2D_add_animated_variable(anm2d, 0, &(obj_child->transform->rotation.w), &valtest, sizeof(obj_child->transform->position.x), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	Animation2D_add_frame(anm2d);
	Animation2D_set_frame_length(anm2d, 1, 1);
	// anm, frame index, varaddr, valaddr, sizeofval, smoothingType
	valtest = 0;
	Animation2D_add_animated_variable(anm2d, 1, &(obj_child->transform->position.y), &valtest, sizeof(obj_child->transform->position.y), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	valtest = 120;
	Animation2D_add_animated_variable(anm2d, 1, &(obj_child->transform->rotation.w), &valtest, sizeof(obj_child->transform->position.x), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	obj->transform->rotation.z = 1;
	obj_child->transform->rotation.z = 1;

	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	
	return(0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	keyboard();

	View_begin_2d(view);

	Object_put(obj);

	View_end();

	fps();

	glFlush();
	glutSwapBuffers();
}

void timer(int value)
{
	// 60 fps
	glutTimerFunc(16, timer, 0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	view->window_width = w;
	view->window_height = h;
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
	Keyboard_init();
}

void *init(void* argp)
{
  return NULL;
}

void fps()
{
	static int count=0;
	static double ts=0;
	static double tc=0;
	struct timeval tv;

	count++;
	gettimeofday(&tv, NULL);
	tc = (double)tv.tv_sec + (double)tv.tv_usec * 1.e-6;
	if(tc - ts >= 1.0)
	{
		DP("fps:%2d\n", count);
		ts = tc;
		count = 0;
	}

}

void mouse(int b, int s, int x, int y)
{

}

void keyboard()
{
	if(Keyboard_is_pressed(27))
		exit(0);
	if(Keyboard_is_pressed('a'))
		obj->transform->rotation.w -= 5;
	if(Keyboard_is_pressed('d'))
		obj->transform->rotation.w += 5;

	if(Keyboard_is_pressed('h'))
		obj_child->transform->position.x -= 5;
	if(Keyboard_is_pressed('l'))
		obj_child->transform->position.x += 5;
}
