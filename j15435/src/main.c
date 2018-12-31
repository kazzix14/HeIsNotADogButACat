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

#include <math.h>
#include "../include/AL/al.h"
#include "../include/AL/alure.h"

#include "transform2d.h"
#include "image2d.h"
#include "vector2d.h"
#include "view.h"
#include "animation2d.h"
#include "animation_controller2d.h"
#include "block2d.h"
#include "grid3d.h"
#include "object.h"

void display(void);
void reshape(int, int);
void timer(int);
void change_color();
void change_backcolor();
void getWindowSize(int *x, int *y);
void put_num();
void set_num_pos(int, int, double, double);
void init();
void fps();
void init_gl(int*, char**, GLuint, GLuint, char*);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);

View* view;
Object *obj;
AnimationController2D* anmcnt;
Animation2D* run_anim;
Animation2D* num_anim;
int count;
Grid3D* grid;

volatile int isdone = 0;
static void eos_callback(void *unused, ALuint unused2)
{
    isdone = 1;
    (void)unused;
    (void)unused2;
}

int main(int argc, char **argv)
{

	init_gl(&argc, argv, 720, 640, "Handwritten Clock");
	init();
	view = View_new();

	run_anim = Animation2D_new();
	num_anim = Animation2D_new();
	Animation2D_load(run_anim, "resource/animation/run_test", 4);
	Animation2D_load(num_anim, "resource/animation/test", 2);
	Animation2D_set_frame_length(run_anim, 4);
	Animation2D_set_frame_length(num_anim, 10);
	Transform2D_set_default(run_anim->transform);
	Transform2D_set_default(num_anim->transform);
	run_anim->transform->position.x = 200;
	run_anim->transform->position.y = 200;
	num_anim->transform->position.x = 200;
	num_anim->transform->position.y = 200;

	Image2D_new();

	Block2D* dirtblk = Block2D_new();
	Block2D* stoneblk = Block2D_new();
	Block2D_load_Image2D(dirtblk, "resource/image/block/environment/dirt/dirt.png");
	Block2D_load_Image2D(stoneblk, "resource/image/block/environment/stone/stone.png");

	unsigned int w = 5000,
		     h = 5000,
		     d = 1;


	grid = Grid3D_new(w, h, d);
	printf("initializing grid\n");
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			for (int k = 0; k < d; k++)
				Grid3D_set_Block2D(grid, i, j, k, (rand()%2 ? dirtblk : stoneblk));
	printf("done\n\n");
	anmcnt = AnimationController2D_new();
	AnimationController2D_add_animation(anmcnt, run_anim, "run");
	AnimationController2D_add_animation(anmcnt, num_anim, "num");
	AnimationController2D_switch(anmcnt, "run");
	obj = Object_new();
	Transform2D_set_default(obj->transform);
	Object_set_AnimationController2D(obj, anmcnt);

	glutTimerFunc(100, timer, 0);
	glutMainLoop();

	return(0);
}

void display(void)
{
	int ww, wh;
	getWindowSize(&ww, &wh);

	glClear(GL_COLOR_BUFFER_BIT);

	View_begin_2d(view);
	//Animation2D_play(anim);
	if(count++ == 700){
		AnimationController2D_switch(anmcnt, "num");
	}
	//AnimationController2D_play(anmcnt);
	Grid3D_put(grid, 0, view);
	Object_play_AnimationController2D(obj);

	View_end();

	fps();

	glFlush();
	glutSwapBuffers();
}

void timer(int value)
{
	// 62.5 fps
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
	glutKeyboardFunc(keyboard);
}

void init()
{
ALuint src, buf;

    if(!alureInitDevice(NULL, NULL))
    {
        fprintf(stderr, "Failed to open OpenAL device: %s\n", alureGetErrorString());
        return ;
    }

    alGenSources(1, &src);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Failed to create OpenAL source!\n");
        alureShutdownDevice();
        return ;
    }

    buf = alureCreateBufferFromFile("test.wav");
    if(!buf)
    {
        fprintf(stderr, "Could not load : %s\n", alureGetErrorString());
        alDeleteSources(1, &src);

        alureShutdownDevice();
        return ;
    }

    alSourcei(src, AL_BUFFER, buf);
    if(alurePlaySource(src, eos_callback, NULL) == AL_FALSE)
    {
        fprintf(stderr, "Failed to start source!\n");
        alDeleteSources(1, &src);
        alDeleteBuffers(1, &buf);

        alureShutdownDevice();
        return ;
    }

    while(!isdone)
    {
        alureSleep(0.125);
        alureUpdate();
    }

    alDeleteSources(1, &src);
    alDeleteBuffers(1, &buf);

    alureShutdownDevice();
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
		printf("%d\n", count);
		ts = tc;
		count = 0;
	}

}

void mouse(int b, int s, int x, int y)
{

}

void keyboard(unsigned char key, int x, int y)
{
	// esc
	if((key == 27))
		exit(0);
	if((key == 'h'))
		obj->transform->position.x -= 1;
	if((key == 'l'))
		obj->transform->position.x += 1;
	if((key == 'w'))
		view->position.y -= 5;
	if((key == 'a'))
		view->position.x -= 5;
	if((key == 's'))
		view->position.y += 5;
	if((key == 'd'))
		view->position.x += 5;
}
