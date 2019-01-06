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
void keyboardUp(unsigned char, int, int);
void keyboardDown(unsigned char, int, int);

View* view;
Object *man;
Object *poop;
AnimationController2D* manAnimCnt;
AnimationController2D* poopAnimCnt;
Animation2D* up_anim;
Animation2D* down_anim;
Animation2D* left_anim;
Animation2D* right_anim;
Animation2D* idle_anim;
Animation2D* poopmode_anim;
Animation2D* poop_anim;
int count;
Grid3D* grid;

Audio* poop_audio;
int main(int argc, char **argv)
{

	Audio* bgm = Audio_new(1);
	Audio_load(bgm, "cfg.wav");
	Audio_play(bgm);
	init_gl(&argc, argv, 2560, 1440, "UNTI!!");
	view = View_new();

	Vector3D_set_zero(&(view->position));

	up_anim = Animation2D_new();
	down_anim = Animation2D_new();
	left_anim = Animation2D_new();
	right_anim = Animation2D_new();
	idle_anim = Animation2D_new();
	poopmode_anim = Animation2D_new();

	poop_anim = Animation2D_new();
	
	Animation2D_load(up_anim, "resource/animation/poopgame/up", 4);
	Animation2D_load(down_anim, "resource/animation/poopgame/down", 4);
	Animation2D_load(left_anim, "resource/animation/poopgame/left", 8);
	Animation2D_load(right_anim, "resource/animation/poopgame/left", 8);
	right_anim->transform->scale.x = -1;
	Animation2D_load(idle_anim, "resource/animation/poopgame/idle", 1);
	Animation2D_load(poopmode_anim, "resource/animation/poopgame/poopmode", 1);
	Animation2D_load(poop_anim, "resource/animation/poopgame/poop", 1);

	Animation2D_set_frame_length(up_anim, 8);
	Animation2D_set_frame_length(down_anim, 8);
	Animation2D_set_frame_length(left_anim, 8);
	Animation2D_set_frame_length(right_anim, 8);
	Animation2D_set_frame_length(idle_anim, 8);
	Animation2D_set_frame_length(poopmode_anim, 40);
	Animation2D_set_frame_length(poop_anim, 10);

	Block2D* dirtblk = Block2D_new();
	Block2D* grassblk = Block2D_new();
	Block2D* stoneblk = Block2D_new();
	Block2D_load_Image2D(dirtblk, "resource/image/block/environment/dirt/dirt.png");
	Block2D_load_Image2D(grassblk, "resource/image/block/environment/grass/grass.png");
	Block2D_load_Image2D(stoneblk, "resource/image/block/environment/stone/stone.png");

	unsigned int w = 500,
		     h = 500,
		     d = 1;


	grid = Grid3D_new(w, h, d);
	printf("initializing grid\n");
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			for (int k = 0; k < d; k++)
				Grid3D_set_Block2D(grid, i, j, k, (rand()%3 ? rand()%2 ? stoneblk : dirtblk : grassblk));
	printf("done\n\n");

	manAnimCnt = AnimationController2D_new();
	poopAnimCnt = AnimationController2D_new();

	AnimationController2D_add_animation(manAnimCnt, up_anim, "up");
	AnimationController2D_add_animation(manAnimCnt, down_anim, "down");
	AnimationController2D_add_animation(manAnimCnt, left_anim, "left");
	AnimationController2D_add_animation(manAnimCnt, right_anim, "right");
	AnimationController2D_add_animation(manAnimCnt, poopmode_anim, "poop");
	AnimationController2D_add_animation(manAnimCnt, idle_anim, "idle");
	AnimationController2D_switch(manAnimCnt, "idle");

	AnimationController2D_add_animation(poopAnimCnt, poop_anim, "go");
	AnimationController2D_switch(poopAnimCnt, "go");

	man = Object_new();
	poop = Object_new();
	man->transform->scale.x = 0.3;
	man->transform->scale.y = 0.3;
	poop->transform->position.x = 10000;
	poop->transform->position.y = 10000;
	poop->transform->scale.x = 0.1;
	poop->transform->scale.y = 0.1;
	Object_set_AnimationController2D(man, manAnimCnt);
	Object_set_AnimationController2D(poop, poopAnimCnt);

	poop_audio = Audio_new(10);
	Audio_load(poop_audio, "unko.wav");

	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	
	return(0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	View_begin_2d(view);
			Grid3D_put(grid, 0, view);
			Object_play_AnimationController2D(man);
			Object_play_AnimationController2D(poop);
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
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
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
		printf("%d\n", count);
		ts = tc;
		count = 0;
	}

}

void mouse(int b, int s, int x, int y)
{

}

void keyboardDown(unsigned char key, int x, int y)
{
	// camera
	if((key == 'k'))
		view->position.y -= 10;
	if((key == 'h'))
		view->position.x -= 10;
	if((key == 'j'))
		view->position.y += 10;
	if((key == 'l'))
		view->position.x += 10;
	// esc
	if((key == 27))
		exit(0);
	// man
	if((key == 'f'))
	{
		AnimationController2D_switch(manAnimCnt, "poop");
		Audio_play(poop_audio);
		poop->transform->position.x = man->transform->position.x+40;
		poop->transform->position.y = man->transform->position.y+60;
	}
	else if((key == 'w'))
	{
		AnimationController2D_switch(manAnimCnt, "up");
		man->transform->position.y -= 5;
	}
	else if((key == 'a'))
	{
		AnimationController2D_switch(manAnimCnt, "left");
		man->transform->position.x -= 5;
	}
	else if((key == 's'))
	{
		AnimationController2D_switch(manAnimCnt, "down");
		man->transform->position.y += 5;
	}
	else if((key == 'd'))
	{
		AnimationController2D_switch(manAnimCnt, "right");
		man->transform->position.x += 5;
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	// man
	if((key == 'f'))
	{
		Sleep(600);
		AnimationController2D_switch(manAnimCnt, "idle");
		poop->transform->position.x = view->position.x + 10000;
		poop->transform->position.y = view->position.y + 10000;
	}
	else if((key == 'w'))
	{
		AnimationController2D_switch(manAnimCnt, "idle");
	}
	else if((key == 'a'))
	{
		AnimationController2D_switch(manAnimCnt, "idle");
	}
	else if((key == 's'))
	{
		AnimationController2D_switch(manAnimCnt, "idle");
	}
	else if((key == 'd'))
	{
		AnimationController2D_switch(manAnimCnt, "idle");
	}
}