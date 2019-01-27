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
#include "collider2d.h"
#include "timer.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define PLAYER_CHARACTER_DEFAULT_SPEED 200
#define PLAYER_CHARACTER_BULLET_DEFAULT_SPEED 600
#define PLAYER_CHARACTER_BULLET_DEFAULT_INTERVAL 0.01
#define PLAYER_CHARACTER_BULLET_NUM 2

#define ENEMY_CHARACTER0_DEFAULT_NUM 32
#define ENEMY_CHARACTER0_DEFAULT_SPEED 200
#define ENEMY_CHARACTER0_DEFAULT_ACCELERATION 0
#define ENEMY_CHARACTER0_DEFAULT_INTERVAL 5
#define ENEMY_BULLET0_DEFAULT_NUM 64
#define ENEMY_CHARACTER_BULLET0_DEFAULT_SPEED 250

typedef struct playerbullet0 PlayerBullet0;
typedef struct enemybullet0 EnemyBullet0;

typedef struct playercharacter
{
	Object* object;
	Object* imageObject;
	Collider2D* collider;
	Image2D* image0;

	Timer* timer;
} PlayerCharacter;

typedef struct playerbullet0
{
	Object* masterObject;
	Object* object[PLAYER_CHARACTER_BULLET_NUM];
	Collider2D* collider[PLAYER_CHARACTER_BULLET_NUM];
	Image2D* image0;
	Audio* audioShot;

	double speed;
	double interval;

	Timer* timer;
} PlayerBullet0;

typedef struct enemycharacter0
{
	Object* masterObject;
	Object* object[ENEMY_CHARACTER0_DEFAULT_NUM];
	Object* childObject[ENEMY_CHARACTER0_DEFAULT_NUM];
	Object* imageObject[ENEMY_CHARACTER0_DEFAULT_NUM];
	Collider2D* collider[ENEMY_CHARACTER0_DEFAULT_NUM];
	Image2D* image0;
	Audio* audioDestroy;
	double speed;
	double acceleration;
	double interval;

	Timer* masterTimer;
	Timer* timer[ENEMY_CHARACTER0_DEFAULT_NUM];
} EnemyCharacter0;

typedef struct enemybullet0
{
	Object* masterObject;
	Object* object[ENEMY_BULLET0_DEFAULT_NUM];
	Collider2D* collider[ENEMY_BULLET0_DEFAULT_NUM];
	Vector2D direction[ENEMY_BULLET0_DEFAULT_NUM];
	Image2D* image0;
	Audio* audioShot;

	double speed;

	Timer* masterTimer;
	Timer* timer[ENEMY_BULLET0_DEFAULT_NUM];
} EnemyBullet0;

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

void setColliderLayerMatrix();
void initPlayerCharacter();
void initPlayerBullet0();
void movePlayerCharacter();
void movePlayerBullet0();

void initEnemyCharacter0();
void initEnemyBullet0();
void moveEnemyCharacter0();
void moveEnemyBullet0();

void addEnemyCharacter0(Vector2D* const pos);
void shotEnemyBullet0(Vector2D* const src, Vector2D* const dict);

void updateTimer();

void stage0_init();
void stage0_initWave0childAnimation();
void stage0_initWave0animation();
void stage0_update();


    // // // // // // // // // // // // // // // // // // // // // // // // // // //
   // // // // // // // // // // // // // // // // // // // // // // // // // // // 
  // // // // // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // 


View* view;

double totaltime = 0;

PlayerCharacter playerCharacter;
PlayerBullet0 playerBullet0;
EnemyCharacter0 enemyCharacter0;
EnemyBullet0 enemyBullet0;

Timer* stage0Timer;

int main(int argc, char **argv)
{
	srand(1);
	init_gl(&argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT, "UNTI!!");
	setColliderLayerMatrix();
	
	view = View_new();
	view->position.z = 1;

	initPlayerCharacter();
	initPlayerBullet0();
	initEnemyCharacter0();
	initEnemyBullet0();

	stage0_init();

	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	
	return(0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	keyboard();

	View_begin_2d(view);
		Object_put(playerCharacter.object);
		Object_put(playerBullet0.masterObject);
		Object_put(enemyCharacter0.masterObject);
		Object_put(enemyBullet0.masterObject);
		Collider2D_judge_all();
	View_end_2d();

	glFlush();
	glutSwapBuffers();
}

void timer(int value)
{
	// 60 fps
	glutTimerFunc(16, timer, 0);
	glutPostRedisplay();

	updateTimer();

	stage0_update();

	movePlayerBullet0();
	movePlayerCharacter();
	moveEnemyCharacter0();
	moveEnemyBullet0();

	fps();
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
	if(Keyboard_is_down(27))
		exit(0);
	/*
	if(rand() % 70)
	{
		double count;
		Timer_get_count(enemyCharacter0.masterTimer, &count);
		if(count > 0.5)
		{
			DPIF(false, "count:%f\n", count);
			Timer_reset_count(enemyCharacter0.masterTimer);
			// add enemy
			
			for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
			{
				bool isValid;
				Object_is_valid(enemyCharacter0.object[i], &isValid);
				if(isValid == false)
				{
					DPIF(false, "i:%d\n", i);
					Object_set_valid(enemyCharacter0.object[i]);
					enemyCharacter0.collider[i]->isValid = true;
					enemyCharacter0.object[i]->transform->position.y = rand()%view->window_height;
					enemyCharacter0.object[i]->transform->position.x = view->window_width;
					Timer_set_count(enemyCharacter0.timer[i], enemyCharacter0.interval);
					break;
				}
			}
			
			//
		}
	}
	*/
	
	// enemy shot
	/*
	for(int ei = 0; ei < ENEMY_CHARACTER0_DEFAULT_NUM; ei++)
	{
		bool isValid;
		Object_is_valid(enemyCharacter0.object[ei], &isValid);
		if(isValid == false) continue;

		if(rand() % 50 == 0)
		{
			double count;
			Timer_get_count(enemyCharacter0.timer[ei], &count);

			if(count > enemyCharacter0.interval)
			{
				DPIF(false, "count:%f\n", count);
				Timer_reset_count(enemyCharacter0.timer[ei]);

				//
				for(int i = 0; i < ENEMY_BULLET0_DEFAULT_NUM; i++)
				{
					bool isValid;
					Object_is_valid(enemyBullet0.object[i], &isValid);
					if(isValid == false)
					{
						DPIF(false, "i:%d\n", i);
						Object_set_valid(enemyBullet0.object[i]);
						enemyBullet0.collider[i]->isValid = true;
						enemyBullet0.object[i]->transform->position.x = enemyCharacter0.object[ei]->transform->position.x;
						enemyBullet0.object[i]->transform->position.y = enemyCharacter0.object[ei]->transform->position.y;

						// direction
						Vector2D_set(&(enemyBullet0.direction[i]), &(playerCharacter.object->transform->position));
						Vector2D_sub(&(enemyBullet0.direction[i]), &(enemyBullet0.object[i]->transform->position));

						enemyBullet0.direction[i].x += rand()%300-150;
						enemyBullet0.direction[i].y += rand()%300-150;

						Vector2D_normalize(&(enemyBullet0.direction[i]));

						Audio_play(enemyBullet0.audioShot);
						break;
					}
				}
				//
			}
		}
	}
	*/

	Keyboard_update();

}

void setColliderLayerMatrix()
{
	/*
	 *   layernum = 5
	 *
	 *	l0 l1 l2 l3 l4
	 *   l4	0  1  2  3  4
	 *   l3	5  6  7  8
	 *   l2	9  10 11
	 *   l1	12 13
	 *   l0	14
	 */
	
	// 16 -> pc
	// 15 -> pc bullets
	// 24 -> ec
	// 23 -> ec bullets

	char lm[] = {
	//	0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31	
	
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 
		1, 1, 1, 1, 
		1, 1, 1, 
		1, 1, 
		1
	};

	Collider2D_set_layer_matrix(lm);

}

void initPlayerCharacter()
{
	playerCharacter.object = Object_new();
	playerCharacter.imageObject = Object_new();
	playerCharacter.collider = Collider2D_new();
	playerCharacter.image0 = Image2D_new();
	playerCharacter.timer = Timer_new();

	playerCharacter.imageObject->transform->position.x = -4;
	playerCharacter.imageObject->transform->position.y = -7;

	playerCharacter.object->transform->position.x = 300;
	playerCharacter.object->transform->position.y = 300;

	Collider2D_set_collider_object(playerCharacter.collider, COLLIDER2D_COLLIDER_RECT);
	Collider2D_register_collider(playerCharacter.collider, 16);
	RectCollider* rc = (RectCollider*)(playerCharacter.collider->colObj);
	Vector2D_set_zero(&(rc->position));
	rc->size.x = 49;
	rc->size.y = 10;

	Image2D_load(playerCharacter.image0, "resource/image/player/plane/0.png");

	Object_add_component(playerCharacter.imageObject, playerCharacter.image0);
	Object_add_component(playerCharacter.object, playerCharacter.imageObject);
	Object_add_component(playerCharacter.object, playerCharacter.collider);

}

void initPlayerBullet0()
{
	playerBullet0.timer = Timer_new();
	playerBullet0.masterObject = Object_new();
	playerBullet0.image0 = Image2D_new();
	playerBullet0.audioShot = Audio_new(PLAYER_CHARACTER_BULLET_NUM);
	
	Image2D_load(playerBullet0.image0, "resource/image/player/bullet/0.png");
	Audio_load(playerBullet0.audioShot, "resource/audio/player/bullet/shot0.wav");

	Timer_reset_count(playerBullet0.timer);
	playerBullet0.speed = PLAYER_CHARACTER_BULLET_DEFAULT_SPEED;
	playerBullet0.interval = PLAYER_CHARACTER_BULLET_DEFAULT_INTERVAL;

	for(int i = 0; i < PLAYER_CHARACTER_BULLET_NUM; i++)
	{
		Object* pbo;
		Collider2D* pbc;
		RectCollider* rc;

		playerBullet0.object[i] = Object_new();
		playerBullet0.collider[i] = Collider2D_new();
		pbo = playerBullet0.object[i];
		pbc = playerBullet0.collider[i];

		Collider2D_set_collider_object(pbc, COLLIDER2D_COLLIDER_RECT );
		Collider2D_register_collider(pbc, 15);
		rc = (RectCollider*)(pbc->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = 20;
		rc->size.y = 4;
		pbo->transform->scale.y = 0.5;

		Object_add_component(pbo, playerBullet0.image0);
		Object_add_component(pbo, pbc);
		Object_add_component(playerBullet0.masterObject, pbo);

		Object_set_invalid(pbo);
	}
}

void movePlayerCharacter()
{
	if(Keyboard_is_down('a'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.x -= spf * PLAYER_CHARACTER_DEFAULT_SPEED;
	}
	if(Keyboard_is_down('d'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.x += spf * PLAYER_CHARACTER_DEFAULT_SPEED;
	}
	if(Keyboard_is_down('w'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.y -= spf * PLAYER_CHARACTER_DEFAULT_SPEED;
	}
	if(Keyboard_is_down('s'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.y += spf * PLAYER_CHARACTER_DEFAULT_SPEED;
	}

	if(Keyboard_is_pressed('j'))
	{
		double count;
		Timer_get_count(playerBullet0.timer, &count);
		if(count > playerBullet0.interval)
		{
			DPIF(false, "count:%f\n", count);
			Timer_reset_count(playerBullet0.timer);
			for(int i = 0; i < PLAYER_CHARACTER_BULLET_NUM; i++)
			{
				bool isValid;
				Object_is_valid(playerBullet0.object[i], &isValid);
				if(isValid == false)
				{
					DPIF(false, "i:%d\n", i);
					Object_set_valid(playerBullet0.object[i]);
					playerBullet0.collider[i]->isValid = true;
					Audio_play(playerBullet0.audioShot);
					playerBullet0.object[i]->transform->position.x = playerCharacter.object->transform->position.x + 60;
					playerBullet0.object[i]->transform->position.y = playerCharacter.object->transform->position.y + 8;
					break;
				}
			}
		}
	}

	////
	//
	

	if(playerCharacter.collider->hits[0] != NULL)
	{
		DP(
		   "---------------------------------------------------------\n"
		   "You Died!!\n"
		   "---------------------------------------------------------\n"
		);
		//exit(0);
	}

	//
	////
}

void movePlayerBullet0()
{
	double spf;
	Timer_get_spf(playerBullet0.timer, &spf);
	for(int i = 0; i < PLAYER_CHARACTER_BULLET_NUM; i++)
	{
		Object* pbo = playerBullet0.object[i];
		bool isValid;
		Object_is_valid(pbo, &isValid);

		if(playerBullet0.collider[i]->hits[0] != NULL)
		{
			Object_set_invalid(pbo);
			playerBullet0.collider[i]->isValid = false;
		}

		if(isValid == true)
		{
			pbo->transform->position.x += playerBullet0.speed * spf;
			if(pbo->transform->position.x > view->window_width)
			{
				Object_set_invalid(pbo);
				playerBullet0.collider[i]->isValid = false;
			}
		}
		else
		{
			//pbo->transform->position.x = playerCharacter.object->transform->position.x + 60;
			//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

		}
	}
}

void initEnemyCharacter0()
{
	enemyCharacter0.masterObject = Object_new();
	enemyCharacter0.image0 = Image2D_new();
	enemyCharacter0.masterTimer = Timer_new();
	enemyCharacter0.audioDestroy = Audio_new(ENEMY_CHARACTER0_DEFAULT_NUM);
	enemyCharacter0.speed = ENEMY_CHARACTER0_DEFAULT_SPEED;
	enemyCharacter0.acceleration = ENEMY_CHARACTER0_DEFAULT_ACCELERATION;
	enemyCharacter0.interval = ENEMY_CHARACTER0_DEFAULT_INTERVAL;

	Image2D_load(enemyCharacter0.image0, "resource/image/enemy/plane/0.png");
	Audio_load(enemyCharacter0.audioDestroy, "resource/audio/enemy/plane/destroy/0.wav");

	for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
	{
		enemyCharacter0.object[i] = Object_new();
		enemyCharacter0.childObject[i] = Object_new();
		enemyCharacter0.imageObject[i] = Object_new();
		enemyCharacter0.collider[i] = Collider2D_new();
		enemyCharacter0.timer[i] = Timer_new();

		enemyCharacter0.imageObject[i]->transform->position.x = 0;
		enemyCharacter0.imageObject[i]->transform->position.y = -1;

		enemyCharacter0.imageObject[i]->transform->scale.x = 0.7;
		enemyCharacter0.imageObject[i]->transform->scale.y = 0.7;

		Collider2D_set_collider_object(enemyCharacter0.collider[i], COLLIDER2D_COLLIDER_RECT);
		Collider2D_register_collider(enemyCharacter0.collider[i], 24);
		RectCollider* rc = (RectCollider*)(enemyCharacter0.collider[i]->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = 27;
		rc->size.y = 25;
		//enemyCharacter0.enemyBullet0[i] = (EnemyBullet0*)malloc(sizeof(EnemyBullet0));
		//initEnemyBullet0(enemyCharacter0.enemyBullet0[i]);
		Object_add_component(enemyCharacter0.imageObject[i], enemyCharacter0.image0);
		Object_add_component(enemyCharacter0.childObject[i], enemyCharacter0.imageObject[i]);
		Object_add_component(enemyCharacter0.childObject[i], enemyCharacter0.collider[i]);
		Object_add_component(enemyCharacter0.object[i], enemyCharacter0.childObject[i]);
		//Object_add_component(enemyCharacter0.object[i], enemyCharacter0.enemyBullet0[i]->masterObject);
		Object_add_component(enemyCharacter0.masterObject, enemyCharacter0.object[i]);
	}



}

void initEnemyBullet0()
{
	static Image2D* eb0img;
	if(eb0img == NULL)
	{
		eb0img = Image2D_new();
		Image2D_load(eb0img, "resource/image/enemy/bullet/0.png");
	}

	EnemyBullet0* eb = &enemyBullet0;

	eb->masterObject = Object_new();
	eb->image0 = Image2D_new();
	eb->audioShot = Audio_new(ENEMY_BULLET0_DEFAULT_NUM);
	eb->masterTimer = Timer_new();
	
	eb->image0 = eb0img;
	Audio_load(eb->audioShot, "resource/audio/enemy/bullet/shot0.wav");

	eb->speed = ENEMY_CHARACTER_BULLET0_DEFAULT_SPEED;

	for(int i = 0; i < ENEMY_BULLET0_DEFAULT_NUM; i++)
	{
		Object* pbo;
		Collider2D* pbc;
		RectCollider* rc;

		eb->object[i] = Object_new();
		eb->collider[i] = Collider2D_new();
		pbo = eb->object[i];
		pbc = eb->collider[i];

		eb->timer[i] = Timer_new();
		Timer_reset_count(eb->timer[i]);

		Collider2D_set_collider_object(pbc, COLLIDER2D_COLLIDER_RECT );
		Collider2D_register_collider(pbc, 23);
		rc = (RectCollider*)(pbc->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = 8;
		rc->size.y = 8;

		Object_add_component(pbo, eb->image0);
		Object_add_component(pbo, pbc);
		Object_add_component(eb->masterObject, pbo);

		Object_set_invalid(pbo);
	}
}

void moveEnemyCharacter0()
{
	const int shotChance = 100;
	for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
	{
		double spf;
		Timer_get_spf(enemyCharacter0.timer[i], &spf);
		Object* pbo = enemyCharacter0.object[i];
		if(enemyCharacter0.collider[i]->hits[0] != NULL)
		{
			DPIF(false, "%p is destroyed by %p\n", enemyCharacter0.collider[i], enemyCharacter0.collider[i]->hits[0]);
			Object_set_invalid(pbo);
			enemyCharacter0.collider[i]->isValid = false;
			Audio_play(enemyCharacter0.audioDestroy);
		}
		bool isValid;
		Object_is_valid(pbo, &isValid);

		if(isValid == true)
		{

			// shot randomly
			double t;
			Timer_get_count(enemyCharacter0.timer[i], &t);
			if(t > enemyCharacter0.interval)
			{
				if(rand()%shotChance == 0)
				{
					Vector2D v;
					Timer_reset_count(enemyCharacter0.timer[i]);
					Vector2D_set(&v, &(playerCharacter.object->transform->position));
					Vector2D_sub(&v, &(enemyCharacter0.object[i]->transform->position));
					if(rand() % 3 == 0)
					{
						v.x += (rand() % 11 - 5 ) * 3;
						v.y += (rand() % 11 - 5 ) * 3;
					}
					else
					{
						v.x += (rand() % 11 - 5 ) * 150;
						v.y += (rand() % 11 - 5 ) * 150;
					}
					Vector2D_normalize(&v);
					shotEnemyBullet0(&(enemyCharacter0.object[i]->transform->position), &v);
				}
			}



			//enemyCharacter0.speed += enemyCharacter0.acceleration * spf;
			//pbo->transform->position.x -= enemyCharacter0.speed * spf;
			if(pbo->transform->position.x < -100)
			{
				Object_set_invalid(pbo);
				enemyCharacter0.collider[i]->isValid = false;
			}
		}
		else
		{
			//pbo->transform->position.x = view->window_width;
			//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

		}
	}
}

void moveEnemyBullet0()
{
	EnemyBullet0* eb = &enemyBullet0;
	for(int i = 0; i < ENEMY_BULLET0_DEFAULT_NUM; i++)
	{
		double spf;
		Timer_get_spf(eb->timer[i], &spf);

		Object* pbo = eb->object[i];
		if(eb->collider[i]->hits[0] != NULL)
		{
			DPIF(false, "hit\n");
			Object_set_invalid(pbo);
			eb->collider[i]->isValid = false;
		}
		bool isValid;
		Object_is_valid(pbo, &isValid);

		if(isValid == true)
		{
			pbo->transform->position.x += eb->speed * spf * eb->direction[i].x;
			pbo->transform->position.y += eb->speed * spf * eb->direction[i].y;


			if(pbo->transform->position.x < -100 || view->window_width < pbo->transform->position.x || pbo->transform->position.y < 0 || view->window_height+8 < pbo->transform->position.y)
			{
				Object_set_invalid(pbo);
				eb->collider[i]->isValid = false;
			}
		}
		else
		{
			//pbo->transform->position.x = view->window_width;
			//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

		}
	}
}


void addEnemyCharacter0(Vector2D* const pos)
{
	for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
	{
		bool isValid;
		Object_is_valid(enemyCharacter0.object[i], &isValid);
		if(isValid == false)
		{
			DPIF(false, "i:%d\n", i);
			Object_set_valid(enemyCharacter0.object[i]);
			enemyCharacter0.collider[i]->isValid = true;
			Vector2D_set(&(enemyCharacter0.object[i]->transform->position), pos);
			Timer_set_count(enemyCharacter0.timer[i], enemyCharacter0.interval);
			break;
		}
	}
}

void shotEnemyBullet0(Vector2D* const src, Vector2D* const dict)
{
	for(int i = 0; i < ENEMY_BULLET0_DEFAULT_NUM; i++)
	{
		bool isValid;
		Object_is_valid(enemyBullet0.object[i], &isValid);
		if(isValid == false)
		{
			DPIF(false, "i:%d\n", i);
			Object_set_valid(enemyBullet0.object[i]);
			enemyBullet0.collider[i]->isValid = true;

			Vector2D_set(&(enemyBullet0.object[i]->transform->position), src);
			Vector2D_set(&(enemyBullet0.direction[i]), dict);

			DDO
			(
				Vector2D v;
				Vector2D_set(&v, &(enemyBullet0.direction[i]));
				Vector2D_normalize(&(enemyBullet0.direction[i]));
				if( fabs(v.x - enemyBullet0.direction[i].x) > 0.01  || fabs(v.y - enemyBullet0.direction[i].y) > 0.01)
				{
					// fg red
					DE("\x1b[41m");

					DE("vec2 dict is not normalized");

					// fg black
					DE("\x1b[40m\n");
				}
			)

			Audio_play(enemyBullet0.audioShot);
			break;
		}
	}
}

void updateTimer()
{
	Timer_count(playerCharacter.timer);
	Timer_count(playerBullet0.timer);

	Timer_count(enemyCharacter0.masterTimer);
	Timer_count(enemyBullet0.masterTimer);

	for(int i = 0; i < ENEMY_BULLET0_DEFAULT_NUM; i++)
	{
		Timer_count(enemyBullet0.timer[i]);
	}
	for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
	{
		Timer_count(enemyCharacter0.timer[i]);
	}

	Timer_count(stage0Timer);
}


    // // // // // // // // // // // // // // // // // // // // // // // // // // //
   // // // // // // // // // // // // // // // // // // // // // // // // // // // 
  // // // // // // // // // //  S  T  A  G  E  0 // // // // // // // // // // //
 // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // 


Animation2D* wave0childAnimation;
Animation2D* wave0animation;

void stage0_init()
{
	stage0Timer = Timer_new();
	wave0childAnimation = Animation2D_new();
	wave0animation = Animation2D_new();

	stage0_initWave0childAnimation();
	stage0_initWave0animation();
}

void stage0_update()
{
	double st;
	Timer_get_count(stage0Timer, &st);

	const double enemyWaveInterval = 50.0;
	const double enemyWaveWidth = 60.0;
	const int enemyWave0num = 8;
	static bool enemyWave0 = false;

	Vector2D v;

	if(enemyWave0 == true)
	{
		Animation2D_play(wave0childAnimation);
		Animation2D_play(wave0animation);
	}

	if(enemyWave0 == false && st > 5.0)
	{
		v.x = 2000;
		v.y = 0;
		for(int i = 0; i < enemyWave0num; i++)
		{
			addEnemyCharacter0(&v);
		}
		enemyWave0 = true;
	}		
	if(st > 13+5)
	{
		enemyWave0 = false;
	}
}

// wave
void stage0_initWave0animation()
{
	const int wave0interval = 50;
	Vector2D v;
	// frame 0

	for(int f = 0; f < 64; f++) // 64 * 0.2 : 12.8
	{
		Animation2D_add_frame(wave0animation);
		Animation2D_set_frame_length(wave0animation, f, 0.2);// every 0.2 second
		// anm, frame index, varaddr, valaddr, sizeofval, smoothingType
		for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
		{
			// calc here       (ex. x = cos(f)*10, y = sin(f)*20
			// !!! (f+i) !!!
			
			double t = ( f - 32.0 ) / 2.0 + ( i / 2.0 );

			v.x = WINDOW_WIDTH / 2.0 -
				(
				 	pow(t, 3.0) - 32.0*t
				);

			t = (f-10)/64.0 + i/10.0;

			v.y = 300.0 -
				(
					(-2.0*pow(t, 3.0) + 3.0*pow(t, 2.0)) * 200.0
				);

			Animation2D_add_animated_variable(wave0animation, f, &(enemyCharacter0.object[i]->transform->position), &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_VECTOR2D);
		}
	}
}

// circle
void stage0_initWave0childAnimation()
{
	const double radius = 15;
	Vector2D v;
	// frame 0

	for(int f = 0; f < 16; f++) // 16 * 0.2 : 1.6
	{
		Animation2D_add_frame(wave0childAnimation);
		Animation2D_set_frame_length(wave0childAnimation, f, 0.1);// every 0.2 second
		// anm, frame index, varaddr, valaddr, sizeofval, smoothingType
		for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
		{
			// calc here       (ex. x = cos(f)*10, y = sin(f)*20
			// !!! (f+i) !!!
			
			double t = (f / 8.0 * M_PI) + ( i / 2.0 );

			v.x =
				(
				 	cos(t) * radius
				);

			v.y =
				(
					sin(t) * radius
				);

			Animation2D_add_animated_variable(wave0childAnimation, f, &(enemyCharacter0.childObject[i]->transform->position), &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_VECTOR2D);
		}
	}
}

