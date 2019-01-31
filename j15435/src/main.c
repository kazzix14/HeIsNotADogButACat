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
#include <string.h>
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

#define PLAYER_CHARACTER_DEFAULT_SPEED 130
#define PLAYER_CHARACTER_BULLET_DEFAULT_SPEED 1200
#define PLAYER_CHARACTER_BULLET_DEFAULT_INTERVAL 0.01
#define PLAYER_CHARACTER_BULLET_NUM 2

#define ENEMY_CHARACTER0_DEFAULT_NUM 32
#define ENEMY_CHARACTER0_DEFAULT_SPEED 200
#define ENEMY_CHARACTER0_DEFAULT_ACCELERATION 0
#define ENEMY_CHARACTER0_DEFAULT_INTERVAL 2
#define ENEMY_BULLET0_DEFAULT_NUM 64
#define ENEMY_CHARACTER_BULLET0_DEFAULT_SPEED 350

#define ENEMY_CHARACTER1_DEFAULT_NUM 16
#define ENEMY_CHARACTER1_DEFAULT_SPEED 160
#define ENEMY_CHARACTER1_DEFAULT_ACCELERATION 0
#define ENEMY_CHARACTER1_DEFAULT_INTERVAL 3

#define ENEMY_CHARACTER2_DEFAULT_NUM 16
#define ENEMY_CHARACTER2_DEFAULT_SPEED 160
#define ENEMY_CHARACTER2_DEFAULT_ACCELERATION 0
#define ENEMY_CHARACTER2_DEFAULT_INTERVAL 3

#define POWERUPCORE_DEFAULT_NUM 16
#define POWERUPCORE_DEFAULT_SPEED 100
#define POWERUPCORE_OBTAIN_SPEED 20
#define POWERUPCORE_SPEEDUP_TAG "pusp"

#define BACKGROUND1_SPEED 30
#define BACKGROUND2_SPEED 60
#define BACKGROUND3_SPEED 100

#define DESTROY_EFFECT_FADETIME 0.15
#define DESTROY_EFFECT_NUM 64

#define SPHINX_DEFAULT_HP1 127
#define SPHINX_DEFAULT_HP2 127

typedef struct playerbullet0 PlayerBullet0;
typedef struct enemybullet0 EnemyBullet0;

typedef struct playercharacter
{
	Object* object;
	Object* imageObject;
	Collider2D* collider;
	Image2D* image0;
	double speed;

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

typedef struct enemycharacter2 // vehicle
{
	Object* masterObject;
	Object* object[ENEMY_CHARACTER2_DEFAULT_NUM];
	Object* cannonObject[ENEMY_CHARACTER2_DEFAULT_NUM];
	Object* tireObject[ENEMY_CHARACTER2_DEFAULT_NUM];
	Collider2D* collider[ENEMY_CHARACTER2_DEFAULT_NUM];
	Image2D* image0; // tire
	Image2D* image1; // cannon
	Audio* audioDestroy;
	double speed[ENEMY_CHARACTER2_DEFAULT_SPEED];
	double acceleration;
	double interval;

	Timer* masterTimer;
	Timer* timer[ENEMY_CHARACTER0_DEFAULT_NUM];
} EnemyCharacter2;

typedef struct boss0
{
	Object* masterObject;
	Object* childObject;
	Object* body;
	Object* uleg;
	Object* lleg;
	Object* uarm;
	Object* larm;
	Object* head;
	Object* headp;
	Object* jaw;
	Object* eye;

	Object* lazer;
	Object* lazerFX;


	Collider2D* bdcol;
	Collider2D* ulcol;
	Collider2D* llcol;
	Collider2D* uacol;
	Collider2D* lacol;
	Collider2D* hdcol;
	Collider2D* jwcol;
	Collider2D* eycol;
	Collider2D* blcol;

	Collider2D* lzcol;

	Timer* timer;

	Audio* aPunch;
	Audio* aMao;
	Audio* aLazer;
	Audio* aGass;
	Audio* aBump;

	double pint; //punch interval
	double mint; //mao
	double lint; //lazer
	double gint; //gass
	double bint; //bump

	double hp1;
	double hp2;
} Boss0;

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

typedef struct powerupcore
{
	Object* masterObject;
	Object* object[ENEMY_BULLET0_DEFAULT_NUM];
	Object* imageObject[ENEMY_BULLET0_DEFAULT_NUM];
	Collider2D* collider[ENEMY_BULLET0_DEFAULT_NUM];
	Image2D* image0;
	Audio* audioGet;
	Timer* masterTimer;
} PowerupCore;

typedef struct destroyeffect
{
	Object* masterObject;
	Object* object[DESTROY_EFFECT_NUM];
	Image2D* image0;
	double fadeTime;

	Timer* masterTimer;
	Timer* timer[DESTROY_EFFECT_NUM];
} DestroyEffect;

struct waveargstruct
{
	double exponent;
	double stopRatio;
	double xCenter;
	double yStart;
	double yLength;
	double indexoffset;
};

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

void initSphinx();
void moveSphinx();

void initEnemyCharacter0();
void initEnemyBullet0();
void moveEnemyCharacter0();
void moveEnemyBullet0();

void initEnemyCharacter1();
void moveEnemyCharacter1();

void initEnemyCharacter2();

void addEnemyCharacter0(Vector2D* const pos, const int start, const int end);
void addEnemyCharacter1(Vector2D* const pos, const double offset, const int start, const int end);
void shotEnemyBullet0(Vector2D* const src, Vector2D* const dict);

void updateTimer();

void initPowerupCore();
void movePowerupCore();
void addPowerupCore(Vector2D* const pos, const char tag[COLLIDER2D_TAG_LENGTH]);

void initDestroyEffect();
void addDestroyEffect(Vector2D* const pos);
void moveDestroyEffect();

void initBackground();
void moveBackground();

void stage0_init();
void stage0_initWave0childAnimation();
void stage0_initWave0animation();
void stage0_update();

void setStageAnimation(Animation2D* const anm, const int fnum, Object** const object, const double framelength, const int istart, const int iend, void func(Vector2D*, int, int, int, int, int , double, void*), void* arg);
void moveStopMove(Vector2D* const ret, const int f, const int i, const int fmax, const int imin, const int imax, const double framelength, void* waveArgStruct);
void circleCircleCircle(Vector2D* const ret, const int f, const int i, const int fmax, const int imin, const int imax, const double framelength, void* radiusd);
void moveStopMoveStraight(Vector2D* const ret, const int f, const int i, const int fmax, const int imin, const int imax, const double framelength, void* waveArgStruct);

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
EnemyCharacter0 enemyCharacter1;
EnemyCharacter2 enemyCharacter2;
EnemyBullet0 enemyBullet0;
PowerupCore powerupCore;
DestroyEffect destroyEffect0;;
Boss0 sphinx;


Object* backbase;
Object* back1;
Object* back2;
Object* back3;
Object* back1s;
Object* back2s;
Object* back3s;
Timer* backTimer;
int backgroundWidth;

Object* stage0;
double stage0_moving_speed = 120;
Timer* stage0Timer;

long long score;

int main(int argc, char **argv)
{
	srand(1);
	init_gl(&argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT, "UNTI!!");
	setColliderLayerMatrix();
	
	view = View_new();
	view->position.z = 1;
	view->window_width = WINDOW_WIDTH;
	view->window_height = WINDOW_HEIGHT;

	initPlayerCharacter();
	initPlayerBullet0();
	initEnemyCharacter0();
	initEnemyBullet0();
	initEnemyCharacter1();
	initSphinx();

	initDestroyEffect();

	initPowerupCore();

	initBackground();
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
		Object_put(backbase);
		Object_put(stage0);
		Object_put(sphinx.masterObject);
		Object_put(playerCharacter.object);
		Object_put(enemyCharacter0.masterObject);
		Object_put(enemyCharacter1.masterObject);
		Object_put(destroyEffect0.masterObject);
		Object_put(playerBullet0.masterObject);
		Object_put(enemyBullet0.masterObject);
		Object_put(powerupCore.masterObject);
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

	moveBackground();

	stage0_update();

	movePlayerBullet0();
	movePlayerCharacter();
	moveEnemyCharacter0();
	moveEnemyBullet0();
	moveEnemyCharacter1();
	movePowerupCore();
	moveDestroyEffect();
	moveSphinx();

	fps();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
//	view->window_width = w;
//	view->window_height = h;
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

	if(Keyboard_is_down(','))
		sphinx.uarm->transform->rotation.w += 3;
	if(Keyboard_is_down('m'))
		sphinx.larm->transform->rotation.w += 3;
	if(Keyboard_is_down('n'))
		sphinx.body->transform->rotation.w += 3;
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
	// 10 -> powercore
	// 20 -> terrain
	// 6 -> sphinx
	// 5 -> sphinx lazer

	char lm[] = {
	//	0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31	
	
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
		1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 0, 0, 
		1, 1, 1, 1, 1, 0, 
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
	playerCharacter.speed = PLAYER_CHARACTER_DEFAULT_SPEED;

	playerCharacter.imageObject->transform->position.x = -4;
	playerCharacter.imageObject->transform->position.y = -7;

	playerCharacter.object->transform->position.x = 300;
	playerCharacter.object->transform->position.y = 300;
	strncpy(playerCharacter.collider->tag, "plch", 4);

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
	playerBullet0.audioShot = Audio_new(PLAYER_CHARACTER_BULLET_NUM*2);
	
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
		strncpy(pbc->tag, "pb00", 4);

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
		playerCharacter.object->transform->position.x -= spf * playerCharacter.speed;
	}
	if(Keyboard_is_down('d'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.x += spf * playerCharacter.speed;
	}
	if(Keyboard_is_down('w'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.y -= spf * playerCharacter.speed;
	}
	if(Keyboard_is_down('s'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.y += spf * playerCharacter.speed;
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
		bool pucore = false;
		for(int i = 0; i < COLLIDER2D_HITS_NUM; i++)
		{
			if(playerCharacter.collider->hits[i] != NULL)
			{
				if(strncmp(playerCharacter.collider->hits[i]->tag, POWERUPCORE_SPEEDUP_TAG, COLLIDER2D_TAG_LENGTH) == 0)
				{
					pucore = true;
					break;
				}
			}
		}

		if(pucore == true)
		{
			playerCharacter.speed += POWERUPCORE_OBTAIN_SPEED;
		}
		else
		{
			DP(
			   "---------------------------------------------------------\n"
			   "You Died!!\n"
			   "---------------------------------------------------------\n"
			);
		}
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
			for(int j = 0; j < COLLIDER2D_HITS_NUM; j++)
			{
				if(playerBullet0.collider[i]->hits[j] != NULL)
				{
					if(strncmp(playerBullet0.collider[i]->hits[j]->tag, "enc0", COLLIDER2D_TAG_LENGTH) == 0)
					{
						score += 100;
						DP("score : %lld\n", score);
					}else if(strncmp(playerBullet0.collider[i]->hits[j]->tag, "enc1", COLLIDER2D_TAG_LENGTH) == 0)
					{
						score += 200;
						DP("score : %lld\n", score);

					}
				}
			}
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

Animation2D* bossStandUpAnim;
Animation2D* bossPunch;
Animation2D* bossHead;
Animation2D* bossLazer;

void initSphinx()
{

	sphinx.hp1 = SPHINX_DEFAULT_HP1;
	sphinx.hp2 = SPHINX_DEFAULT_HP2;

	sphinx.masterObject = Object_new();
	sphinx.childObject = Object_new();
	
	sphinx.body = Object_new();
	sphinx.uleg = Object_new();
	sphinx.lleg = Object_new();
	sphinx.uarm = Object_new();
	sphinx.larm = Object_new();
	sphinx.head = Object_new();
	sphinx.headp= Object_new();
	sphinx.jaw = Object_new();
	sphinx.eye = Object_new();
	sphinx.lazer = Object_new();

	Object* bd;
	Object* ul;
	Object* ll;
	Object* ua;
	Object* la;
	Object* hd;
	Object* jw;
	Object* ey;
	Object* bl;

	Object* lz;

	bd = Object_new();
	ul = Object_new();
	ll = Object_new();
	ua = Object_new();
	la = Object_new();
	hd = Object_new();
	hd = Object_new();
	jw = Object_new();
	ey = Object_new();
	bl = Object_new();
	lz = Object_new();


	sphinx.timer = Timer_new();

	sphinx.aPunch = Audio_new(2);
	sphinx.aMao = Audio_new(2);
	sphinx.aLazer = Audio_new(16);
	sphinx.aGass = Audio_new(16);
	sphinx.aBump = Audio_new(2);

	sphinx.pint = ENEMY_CHARACTER0_DEFAULT_INTERVAL;
	sphinx.mint = ENEMY_CHARACTER0_DEFAULT_INTERVAL;
	sphinx.lint = ENEMY_CHARACTER0_DEFAULT_INTERVAL;
	sphinx.gint = ENEMY_CHARACTER0_DEFAULT_INTERVAL;
	sphinx.bint = ENEMY_CHARACTER0_DEFAULT_INTERVAL;

	Image2D* ibd;
	Image2D* iul;
	Image2D* ill;
	Image2D* iua;
	Image2D* ila;
	Image2D* ihd;
	Image2D* ijw;
	Image2D* iey;
	Image2D* ilz;

	ibd = Image2D_new();
	iul = Image2D_new();
	ill = Image2D_new();
	iua = Image2D_new();
	ila = Image2D_new();
	ihd = Image2D_new();
	ijw = Image2D_new();
	iey = Image2D_new();
	ilz = Image2D_new();

	Image2D_load(ibd, "resource/image/enemy/boss/body.png");
	Image2D_load(iul, "resource/image/enemy/boss/uleg.png");
	Image2D_load(ill, "resource/image/enemy/boss/lleg.png");
	Image2D_load(iua, "resource/image/enemy/boss/uarm.png");
	Image2D_load(ila, "resource/image/enemy/boss/larm.png");
	Image2D_load(ihd, "resource/image/enemy/boss/head.png");
	Image2D_load(ijw, "resource/image/enemy/boss/jaw.png");
	Image2D_load(iey, "resource/image/enemy/boss/eye.png");
	Image2D_load(ilz, "resource/image/enemy/boss/lazer.png");

	//Audio_load(aPunch, "resource/audio/enemy/boss/0.wav");
	//Audio_load(aMao, "resource/audio/enemy/boss/0.wav");
	//Audio_load(aLazer, "resource/audio/enemy/boss/0.wav");
	//Audio_load(aGass, "resource/audio/enemy/boss/0.wav");
	//Audio_load(aBump, "resource/audio/enemy/boss/0.wav");
	
	sphinx.bdcol = Collider2D_new();
	sphinx.ulcol = Collider2D_new();
	sphinx.llcol = Collider2D_new();
	sphinx.uacol = Collider2D_new();
	sphinx.lacol = Collider2D_new();
	sphinx.hdcol = Collider2D_new();
	sphinx.jwcol = Collider2D_new();
	sphinx.eycol = Collider2D_new();
	sphinx.blcol = Collider2D_new();
	sphinx.lzcol = Collider2D_new();

	strncpy(sphinx.bdcol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.ulcol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.llcol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.uacol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.lacol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.hdcol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.jwcol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.eycol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.blcol->tag, "spnx", COLLIDER2D_TAG_LENGTH);
	strncpy(sphinx.lzcol->tag, "spxl", COLLIDER2D_TAG_LENGTH);

	Collider2D_set_collider_object(sphinx.bdcol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.ulcol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.llcol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.uacol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.lacol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.hdcol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.jwcol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.eycol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.blcol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(sphinx.lzcol, COLLIDER2D_COLLIDER_RECT);

	Collider2D_register_collider(sphinx.bdcol, 6);
	Collider2D_register_collider(sphinx.ulcol, 6);
	Collider2D_register_collider(sphinx.llcol, 6);
	Collider2D_register_collider(sphinx.uacol, 6);
	Collider2D_register_collider(sphinx.lacol, 6);
	Collider2D_register_collider(sphinx.hdcol, 6);
	Collider2D_register_collider(sphinx.jwcol, 6);
	Collider2D_register_collider(sphinx.eycol, 6);
	Collider2D_register_collider(sphinx.blcol, 6);
	Collider2D_register_collider(sphinx.lzcol, 5);

	RectCollider* bdrc = (RectCollider*)(sphinx.bdcol->colObj);
	RectCollider* ulrc = (RectCollider*)(sphinx.ulcol->colObj);
	RectCollider* llrc = (RectCollider*)(sphinx.llcol->colObj);
	RectCollider* uarc = (RectCollider*)(sphinx.uacol->colObj);
	RectCollider* larc = (RectCollider*)(sphinx.lacol->colObj);
	RectCollider* hdrc = (RectCollider*)(sphinx.hdcol->colObj);
	RectCollider* jwrc = (RectCollider*)(sphinx.jwcol->colObj);
	RectCollider* eyrc = (RectCollider*)(sphinx.eycol->colObj);
	RectCollider* blrc = (RectCollider*)(sphinx.blcol->colObj);
	RectCollider* lzrc = (RectCollider*)(sphinx.lzcol->colObj);

	Vector2D_set_zero(&(bdrc->position));
	Vector2D_set_zero(&(ulrc->position));
	Vector2D_set_zero(&(llrc->position));
	Vector2D_set_zero(&(uarc->position));
	Vector2D_set_zero(&(larc->position));
	Vector2D_set_zero(&(hdrc->position));
	Vector2D_set_zero(&(jwrc->position));
	Vector2D_set_zero(&(eyrc->position));
	Vector2D_set_zero(&(blrc->position));
	Vector2D_set_zero(&(lzrc->position));


	bdrc->size.x = 189;
	bdrc->size.y = 74;
	sphinx.body->transform->rotation.z = 1;
	ibd->p_transform->position.x = -260;
	ibd->p_transform->position.y = -170;
	sphinx.body->transform->position.x = 0;
	sphinx.body->transform->position.y = -32;
	sphinx.body->transform->rotation.z = 1;
	bd->transform->position.x = -180;
	bd->transform->position.y = -65;

	ulrc->size.x = 25;
	ulrc->size.y = 45;
	sphinx.uleg->transform->position.x = 2;
	sphinx.uleg->transform->position.y = -40;
	sphinx.uleg->transform->rotation.z = 1;
	iul->p_transform->rotation.z = 1;
	iul->p_transform->rotation.w = 210;
	iul->p_transform->position.x = 130;
	iul->p_transform->position.y = 250;
	ul->transform->position.x = -12;
	ul->transform->position.y = -32;

	llrc->size.x = 25;
	llrc->size.y = 49;
	//sphinx.lleg->transform->position.x = 37;
	//sphinx.lleg->transform->position.y = 0;
	sphinx.lleg->transform->rotation.z = 1;
	ill->p_transform->position.x = -165;
	ill->p_transform->position.y = -250;
	ill->p_transform->rotation.z = 1;
	ill->p_transform->rotation.w = 20;
	ll->transform->position.x = -10;
	ll->transform->position.y = -39;

	uarc->size.x = 70;
	uarc->size.y = 25;
	sphinx.uarm->transform->rotation.z = 1;
	sphinx.uarm->transform->position.x = 98-260;
	sphinx.uarm->transform->position.y = 170-170;
	ua->transform->position.x = -12;
	ua->transform->position.y = -12;
	iua->p_transform->position.x = -153;
	iua->p_transform->position.y = -105;
	iua->p_transform->rotation.z = 1;
	iua->p_transform->rotation.w = -22;

	larc->size.x = 130;
	larc->size.y = 25;
	sphinx.larm->transform->rotation.z = 1-260;
	sphinx.larm->transform->position.x = 50;
	sphinx.larm->transform->position.y = 0;
	ila->p_transform->position.x = -147;
	ila->p_transform->position.y = -172;
	la->transform->position.x = -122;
	la->transform->position.y = -5;
	
	hdrc->size.x = 61;
	hdrc->size.y = 72;
	sphinx.head->transform->rotation.z = 1;
	sphinx.headp->transform->rotation.z = 1;
	ihd->p_transform->position.x = -90;
	ihd->p_transform->position.y = -110;
	sphinx.headp->transform->position.x = 90-260;
	sphinx.headp->transform->position.y = 110-170;
	hd->transform->position.x = -46;
	hd->transform->position.y = -80;
	hd->transform->rotation.z = 1;
	hd->transform->rotation.w = 8;
	
	jwrc->size.x = 30;
	jwrc->size.y = 10;
	sphinx.jaw->transform->rotation.z = 1;
	ijw->p_transform->position.x = -90;
	ijw->p_transform->position.y = -110;
	jw->transform->position.x = -55;
	jw->transform->position.y = -11;
	jw->transform->rotation.z = 1;
	jw->transform->rotation.w = -25;

	eyrc->size.x = 10;
	eyrc->size.y = 9;
	iey->p_transform->position.x = -89.5;
	iey->p_transform->position.y = -110.5;
	ey->transform->position.x = -51;
	ey->transform->position.y = -54;

	blrc->size.x = 25;
	blrc->size.y = 25;
	bl->transform->position.x = -20;
	bl->transform->position.y = -6;

	lzrc->size.x = WINDOW_WIDTH;
	lzrc->size.y = 5;
	lz->transform->rotation.z = 1;
	lz->transform->rotation.w = 180;
	lz->transform->position.y = -47;
	ilz->p_transform->scale.x = 10;
	ilz->p_transform->rotation.z = 1;
	ilz->p_transform->rotation.w = 180;
	ilz->p_transform->position.y = 27;
	ilz->p_transform->position.x = -50;

	sphinx.body->transform->rotation.w = -150;
	sphinx.uleg->transform->rotation.w = 180;
	sphinx.lleg->transform->rotation.w = -30;

	Object_add_component(sphinx.body, ibd);
	Object_add_component(sphinx.uarm, iua);
	Object_add_component(sphinx.larm, ila);
	Object_add_component(sphinx.jaw, ijw);
	Object_add_component(sphinx.eye, iey);
	Object_add_component(sphinx.lazer, ilz);

	Object_add_component(bd, sphinx.bdcol);
	Object_add_component(bl, sphinx.blcol);
	Object_add_component(ul, sphinx.ulcol);
	Object_add_component(ll, sphinx.llcol);
	Object_add_component(ua, sphinx.uacol);
	Object_add_component(la, sphinx.lacol);
	Object_add_component(hd, sphinx.hdcol);
	Object_add_component(jw, sphinx.jwcol);
	Object_add_component(ey, sphinx.eycol);
	Object_add_component(lz, sphinx.lzcol);

	Object_add_component(sphinx.body, bd);
	Object_add_component(sphinx.body, bl);
	Object_add_component(sphinx.uarm, ua);
	Object_add_component(sphinx.larm, la);
	Object_add_component(sphinx.jaw,  jw);
	Object_add_component(sphinx.eye,  ey);
	Object_add_component(sphinx.lazer,  lz);

	Object_add_component(sphinx.uleg, sphinx.body);
	Object_add_component(sphinx.lleg, sphinx.uleg);

	Object_add_component(sphinx.body, sphinx.uarm);
	Object_add_component(sphinx.uarm, sphinx.larm);

	Object_add_component(sphinx.headp, sphinx.head);
	Object_add_component(sphinx.body, sphinx.headp);
	Object_add_component(sphinx.head, sphinx.jaw);
	Object_add_component(sphinx.head, sphinx.eye);
	Object_add_component(sphinx.head, ihd);
	Object_add_component(sphinx.head, hd);
	Object_add_component(sphinx.head, sphinx.lazer);
	Object_add_component(sphinx.uleg, iul);
	Object_add_component(sphinx.lleg, ill);
	Object_add_component(sphinx.uleg, ul);
	Object_add_component(sphinx.lleg, ll);

	Object_add_component(sphinx.childObject, sphinx.lleg);
	Object_add_component(sphinx.masterObject, sphinx.childObject);



	double neww;
	Vector2D v;

	Object_set_invalid(sphinx.lazer);

	sphinx.childObject->transform->position.x = 850;
	sphinx.childObject->transform->position.y = WINDOW_HEIGHT - 50;

	bossStandUpAnim = Animation2D_new();
	bossPunch = Animation2D_new();
	bossHead = Animation2D_new();
	bossLazer = Animation2D_new();

	/////////////////////////////////// lazer
	// frame 0 init
	Animation2D_add_frame(bossLazer);
	Animation2D_set_frame_length(bossLazer, 0, 0);
	Animation2D_add_animated_variable(bossLazer, 0, &Object_set_invalid, &sphinx.lazer, sizeof(void *), ANIMATION_USE_FUNCCTION);

	//////////////////////////////////// head rot
	// frame 0
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 0, 1);
	neww = 0;
	Animation2D_add_animated_variable(bossHead, 0, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	// frame 1
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 1, 1);
	neww = 8;
	Animation2D_add_animated_variable(bossHead, 1, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	// frame 2
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 2, 1);
	neww = -5;
	Animation2D_add_animated_variable(bossHead, 2, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	//////////////////////////////////// standup
	// frame 0 init
	Animation2D_add_frame(bossStandUpAnim);
	Animation2D_set_frame_length(bossStandUpAnim, 0, 0);
	Animation2D_add_animated_variable(bossStandUpAnim, 0, &(sphinx.uleg->transform->rotation.w), &(sphinx.uleg->transform->rotation.w), sizeof(double), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossStandUpAnim, 0, &(sphinx.body->transform->rotation.w), &(sphinx.body->transform->rotation.w), sizeof(double), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossStandUpAnim, 0, &(sphinx.uarm->transform->rotation.w), &(sphinx.uarm->transform->rotation.w), sizeof(double), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossStandUpAnim, 0, &(sphinx.larm->transform->rotation.w), &(sphinx.larm->transform->rotation.w), sizeof(double), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossStandUpAnim, 0, &(sphinx.headp->transform->rotation.w), &(sphinx.headp->transform->rotation.w), sizeof(double), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossStandUpAnim, 0, &(sphinx.headp->transform->position), &(sphinx.headp->transform->position), sizeof(Vector2D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossStandUpAnim, 0, &(sphinx.childObject->transform->scale), &(sphinx.childObject->transform->scale), sizeof(Vector2D), ANIMATION_NO_SMOOTHING);

	// frame 1
	Animation2D_add_frame(bossStandUpAnim);
	Animation2D_set_frame_length(bossStandUpAnim, 1, 3);


       	neww = 60;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.uleg->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 50;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.body->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 50;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 70;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = -80;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.headp->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	v.x = sphinx.headp->transform->position.x - 20 ;
	v.y = sphinx.headp->transform->position.y + 12;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.headp->transform->position), &(v), sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	v.x = 1.9;
	v.y = 1.9;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.childObject->transform->scale), &(v), sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);

	////////////////////////////////////// punch
	// frame 0 init
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 0, 0.3);
	neww = 50;
	Animation2D_add_animated_variable(bossPunch, 0, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 70;
	Animation2D_add_animated_variable(bossPunch, 0, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 0, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 1 ready
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 1, 0.8);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 1, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 1, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 1, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	// frame 2 punch 1
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 2, 0.1);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 2, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 90;
	Animation2D_add_animated_variable(bossPunch, 2, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 3.5;
	Animation2D_add_animated_variable(bossPunch, 2, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 3 punch 1 end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 3, 0.1);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 3, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 3, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 3, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 4 ready
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 4, 0.1);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 4, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 4, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 4, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	// frame 5 punch 2
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 5, 0.1);
	neww = 00;
	Animation2D_add_animated_variable(bossPunch, 5, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 90;
	Animation2D_add_animated_variable(bossPunch, 5, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 3.5;
	Animation2D_add_animated_variable(bossPunch, 5, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 6 punch 2 end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 6, 0.1);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 6, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 6, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 6, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 7 ready
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 7, 0.1);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 7, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 7, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 7, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	// frame 8 punch 3
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 8, 0.1);
	neww = 40;
	Animation2D_add_animated_variable(bossPunch, 8, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 90;
	Animation2D_add_animated_variable(bossPunch, 8, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 3.2;
	Animation2D_add_animated_variable(bossPunch, 8, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 9 punch 3 end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 9, 0.1);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 9, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 9, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 9, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 10, 0.3);
	neww = 50;
	Animation2D_add_animated_variable(bossPunch, 10, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 70;
	Animation2D_add_animated_variable(bossPunch, 10, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 10, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

}


void moveSphinx()
{
	static bool hp10 = false;
	static bool punch = false;
	Vector2D v;

	if(hp10 == true)
	{
		double cnt;
		double len;
		Timer_get_count(sphinx.timer, &cnt);
		Animation2D_get_length(bossStandUpAnim, &len);
		if(cnt <= len)
		{
			Animation2D_play(bossStandUpAnim);
		}
		else
		{
			hp10 = false;
		}
	}

	if(punch == true)
	{
		double cnt;
		double len;
		Timer_get_count(sphinx.timer, &cnt);
		Animation2D_get_length(bossPunch, &len);
		if(cnt <= len)
		{
			Animation2D_play(bossPunch);
		}
		else
		{
			punch = false;
		}
	}
	
	if(sphinx.hp1 > 0)
	{
		Vector2D_set(&v, &playerCharacter.object->transform->position);
		Vector2D_sub(&v, &sphinx.childObject->transform->position);
		Vector2D_sub(&v, &sphinx.lleg->transform->position);
		Vector2D_sub(&v, &sphinx.uleg->transform->position);
		Vector2D_sub(&v, &sphinx.body->transform->position);
		Vector2D_sub(&v, &sphinx.headp->transform->position);
		Vector2D_sub(&v, &sphinx.head->transform->position);
		Vector2D_sub(&v, &sphinx.eye->transform->position);
		sphinx.headp->transform->rotation.w = atan(v.y/v.x)/M_PI*180;

		if(rand()%60 == 0)
		{
			Vector2D v2;
			Vector2D_set(&v2, &sphinx.childObject->transform->position);
			Vector2D_add(&v2, &sphinx.lleg->transform->position);
			Vector2D_add(&v2, &sphinx.uleg->transform->position);
			Vector2D_add(&v2, &sphinx.body->transform->position);
			Vector2D_add(&v2, &sphinx.headp->transform->position);
			Vector2D_add(&v2, &sphinx.head->transform->position);
			Vector2D_add(&v2, &sphinx.eye->transform->position);
			//v2.y -= 20;
			Vector2D_normalize(&v);
			shotEnemyBullet0(&v2, &v);
		}

		bool flag = false;
		bool flag2 = false;

		if(sphinx.bdcol->hits[0] != NULL) flag = true;
		if(sphinx.ulcol->hits[0] != NULL) flag = true;
		if(sphinx.llcol->hits[0] != NULL) flag = true;
		if(sphinx.uacol->hits[0] != NULL) flag = true;
		if(sphinx.lacol->hits[0] != NULL) flag = true;
		if(sphinx.hdcol->hits[0] != NULL) flag = true;
		if(sphinx.jwcol->hits[0] != NULL) flag = true;

		if(sphinx.eycol->hits[0] != NULL) flag2 = true;
		if(sphinx.blcol->hits[0] != NULL) flag2 = true;

		if(flag2 == true)
		{
			sphinx.hp1 -= 10;
			DP("sphinx.hp1 : %f!!!!!!\n", sphinx.hp1);
			if(sphinx.hp1 <= 0)
			{
			Timer_reset_count(sphinx.timer);
			hp10 = true;
			}
		}
		else if(flag == true)
		{
			sphinx.hp1 -= 1;
			DP("sphinx.hp1 : %f\n", sphinx.hp1);
			if(sphinx.hp1 <= 0)
			{
			Timer_reset_count(sphinx.timer);
			hp10 = true;
			}
		}
	}
	else if(hp10 == false)// 2nd 
	{
		if(punch == false)
		{
			if(rand()%60 == 0)
			{
				Animation2D_reset(bossPunch);
				Timer_reset_count(sphinx.timer);
				punch = true;
			}
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

		enemyCharacter0.object[i]->transform->position.x = -1000;

		enemyCharacter0.imageObject[i]->transform->position.x = 0;
		enemyCharacter0.imageObject[i]->transform->position.y = -1;

		enemyCharacter0.imageObject[i]->transform->scale.x = 0.7;
		enemyCharacter0.imageObject[i]->transform->scale.y = 0.7;

		strncpy(enemyCharacter0.collider[i]->tag, "enc0", COLLIDER2D_TAG_LENGTH);
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
		pbo->transform->position.x = -1000;

		eb->timer[i] = Timer_new();
		Timer_reset_count(eb->timer[i]);

		strncpy(pbc->tag, "enb0", COLLIDER2D_TAG_LENGTH);
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

void initEnemyCharacter1()
{
	enemyCharacter1.masterObject = Object_new();
	enemyCharacter1.image0 = Image2D_new();
	enemyCharacter1.masterTimer = Timer_new();
	enemyCharacter1.audioDestroy = Audio_new(ENEMY_CHARACTER1_DEFAULT_NUM);
	enemyCharacter1.speed = ENEMY_CHARACTER1_DEFAULT_SPEED;
	enemyCharacter1.acceleration = ENEMY_CHARACTER1_DEFAULT_ACCELERATION;
	enemyCharacter1.interval = ENEMY_CHARACTER1_DEFAULT_INTERVAL;

	Image2D_load(enemyCharacter1.image0, "resource/image/enemy/plane/1.png");
	Audio_load(enemyCharacter1.audioDestroy, "resource/audio/enemy/plane/destroy/0.wav");

	for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
	{
		enemyCharacter1.object[i] = Object_new();
		enemyCharacter1.childObject[i] = Object_new();
		enemyCharacter1.imageObject[i] = Object_new();
		enemyCharacter1.collider[i] = Collider2D_new();
		enemyCharacter1.timer[i] = Timer_new();

		enemyCharacter1.object[i]->transform->position.x = -1000;
		enemyCharacter1.object[i]->transform->rotation.z = 1;

		enemyCharacter1.imageObject[i]->transform->position.x = 24;
		enemyCharacter1.imageObject[i]->transform->position.y = -1;

		enemyCharacter1.imageObject[i]->transform->scale.x = -0.7;
		enemyCharacter1.imageObject[i]->transform->scale.y = 0.7;

		strncpy(enemyCharacter1.collider[i]->tag, "enc1", 4);
		Collider2D_set_collider_object(enemyCharacter1.collider[i], COLLIDER2D_COLLIDER_RECT);
		Collider2D_register_collider(enemyCharacter1.collider[i], 24);
		RectCollider* rc = (RectCollider*)(enemyCharacter1.collider[i]->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = 13;
		rc->size.y = 25;
		//enemyCharacter1.enemyBullet0[i] = (EnemyBullet0*)malloc(sizeof(EnemyBullet0));
		//initEnemyBullet0(enemyCharacter1.enemyBullet0[i]);
		Object_add_component(enemyCharacter1.imageObject[i], enemyCharacter1.image0);
		Object_add_component(enemyCharacter1.childObject[i], enemyCharacter1.imageObject[i]);
		Object_add_component(enemyCharacter1.childObject[i], enemyCharacter1.collider[i]);
		Object_add_component(enemyCharacter1.object[i], enemyCharacter1.childObject[i]);
		//Object_add_component(enemyCharacter1.object[i], enemyCharacter1.enemyBullet0[i]->masterObject);
		Object_add_component(enemyCharacter1.masterObject, enemyCharacter1.object[i]);
	}



}

void initEnemyCharacter2()
{
	enemyCharacter2.masterObject = Object_new();
	enemyCharacter2.image0 = Image2D_new();
	enemyCharacter2.image1 = Image2D_new();
	enemyCharacter2.masterTimer = Timer_new();
	enemyCharacter2.audioDestroy = Audio_new(ENEMY_CHARACTER2_DEFAULT_NUM);
	enemyCharacter2.acceleration = ENEMY_CHARACTER2_DEFAULT_ACCELERATION;
	enemyCharacter2.interval = ENEMY_CHARACTER2_DEFAULT_INTERVAL;

	Image2D_load(enemyCharacter2.image0, "resource/image/enemy/vehicle/0t.png");
	Image2D_load(enemyCharacter2.image1, "resource/image/enemy/vehicle/0c.png");
	Audio_load(enemyCharacter2.audioDestroy, "resource/audio/enemy/plane/destroy/0.wav");

	for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
	{
		enemyCharacter2.object[i] = Object_new();
		enemyCharacter2.cannonObject[i] = Object_new();
		enemyCharacter2.tireObject[i] = Object_new();
		enemyCharacter2.collider[i] = Collider2D_new();
		enemyCharacter2.timer[i] = Timer_new();
		enemyCharacter2.speed[i] = ENEMY_CHARACTER2_DEFAULT_SPEED;

		enemyCharacter2.object[i]->transform->position.x = -1000;
		enemyCharacter2.object[i]->transform->rotation.z = 1;

		strncpy(enemyCharacter2.collider[i]->tag, "enc2", 4);
		Collider2D_set_collider_object(enemyCharacter2.collider[i], COLLIDER2D_COLLIDER_RECT);
		Collider2D_register_collider(enemyCharacter2.collider[i], 24);
		RectCollider* rc = (RectCollider*)(enemyCharacter2.collider[i]->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = 13;
		rc->size.y = 25;
		//enemyCharacter2.enemyBullet0[i] = (EnemyBullet0*)malloc(sizeof(EnemyBullet0));
		//initEnemyBullet0(enemyCharacter2.enemyBullet0[i]);
		Object_add_component(enemyCharacter2.tireObject[i], enemyCharacter2.image0);
		Object_add_component(enemyCharacter2.cannonObject[i], enemyCharacter2.image1);
		Object_add_component(enemyCharacter2.cannonObject[i], enemyCharacter2.collider[i]);
		Object_add_component(enemyCharacter2.object[i], enemyCharacter2.cannonObject[i]);
		Object_add_component(enemyCharacter2.object[i], enemyCharacter2.tireObject[i]);
		//Object_add_component(enemyCharacter2.object[i], enemyCharacter2.enemyBullet0[i]->masterObject);
		Object_add_component(enemyCharacter2.masterObject, enemyCharacter2.object[i]);
	}
}

void moveEnemyCharacter0()
{
	int shotChance = 150;
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

				Vector2D v;
				v.x = pbo->transform->position.x + 13;
				v.y = pbo->transform->position.y + 13;
				addDestroyEffect(&v);

			}
			bool isValid;
			Object_is_valid(pbo, &isValid);

			if(isValid == true)
			{
				// shot if player is in the front
				double t;
				Timer_get_count(enemyCharacter0.timer[i], &t);
				if(t > enemyCharacter0.interval && rand()%shotChance == 0)
				{
					double diff = playerCharacter.object->transform->position.y - enemyCharacter0.object[i]->transform->position.y;
					if(diff / ((diff > 0) - (0 > diff)) < 100)
					{
						if(WINDOW_WIDTH > pbo->transform->position.x && pbo->transform->position.y > 0 && view->window_height+50 > pbo->transform->position.y && pbo->transform->position.x > -50)
						{
							Timer_reset_count(enemyCharacter0.timer[i]);
							Vector2D v;
							v.x = -3.0; // 7 : 1
							v.y = rand()/(double)RAND_MAX;
							Vector2D_normalize(&v);
							shotEnemyBullet0(&(enemyCharacter0.object[i]->transform->position), &v);
						}
					}
				}
			}
		if(pbo->transform->position.x < -50)
		{
			Object_set_invalid(pbo);
			enemyCharacter0.collider[i]->isValid = false;
			//pbo->transform->position.x = view->window_width;
			//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

		}
	}
}
void moveEnemyCharacter1()
{
	int shotChance = 200;
	for(int i = 0; i < ENEMY_CHARACTER1_DEFAULT_NUM; i++)
	{
			double spf;
			Timer_get_spf(enemyCharacter1.timer[i], &spf);
			Object* pbo = enemyCharacter1.object[i];
			// destroyed ?
			if(enemyCharacter1.collider[i]->hits[0] != NULL)
			{
				DPIF(false, "%p is destroyed by %p\n", enemyCharacter1.collider[i], enemyCharacter1.collider[i]->hits[0]);
				Object_set_invalid(pbo);
				enemyCharacter1.collider[i]->isValid = false;
				Audio_play(enemyCharacter1.audioDestroy);

				Vector2D v;
				v.x = pbo->transform->position.x + 8;
				v.y = pbo->transform->position.y + 8;
				addDestroyEffect(&v);
			}
			bool isValid;
			Object_is_valid(pbo, &isValid);

			if(isValid == true)
			{
				double t;
				Timer_get_count(enemyCharacter1.timer[i], &t);

				// just move forward
				enemyCharacter1.object[i]->transform->position.x += cos(enemyCharacter1.object[i]->transform->rotation.w/180.0*M_PI) * spf * enemyCharacter1.speed;
				enemyCharacter1.object[i]->transform->position.y += sin(enemyCharacter1.object[i]->transform->rotation.w/180.0*M_PI) * spf * enemyCharacter1.speed;

				Vector2D v;
				Vector2D_set(&v, &(playerCharacter.object->transform->position));
				Vector2D_sub(&v, &(enemyCharacter1.object[i]->transform->position));
				Vector2D_normalize(&v);
				if(fabs(v.x - cos(enemyCharacter1.object[i]->transform->rotation.w/180.0*M_PI)) > 0.05 || fabs(v.y - sin(enemyCharacter1.object[i]->transform->rotation.w/180.0*M_PI)) > 0.05)
				{
					// point to player
					if(t > enemyCharacter1.interval*0.4)
					{
				       		enemyCharacter1.object[i]->transform->rotation.w += 6;
					}
					else
					{
				       		enemyCharacter1.object[i]->transform->rotation.w -= 6;
					}
				}
				else
				{
					// shot if player is in the front
					if(t > enemyCharacter1.interval)
					{
						if(WINDOW_WIDTH > pbo->transform->position.x && pbo->transform->position.y > 0 && view->window_height+50 > pbo->transform->position.y && pbo->transform->position.x > -50)
						{
							Timer_reset_count(enemyCharacter1.timer[i]);
							Vector2D v2;
							Vector2D_set(&v2, &(enemyCharacter1.object[i]->transform->position));
							v2.y += 10;
							shotEnemyBullet0(&v2, &v);
						}
					}
				}
			}
		if(pbo->transform->position.x < -50)
		{
			Object_set_invalid(pbo);
			enemyCharacter1.collider[i]->isValid = false;
			//pbo->transform->position.x = view->window_width;
			//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

		}
	}
}

void moveEnemyCharacter2()
{
	int shotChance = 150;
	for(int i = 0; i < ENEMY_CHARACTER2_DEFAULT_NUM; i++)
	{
			double spf;
			bool isTerrain = false;
			Timer_get_spf(enemyCharacter2.timer[i], &spf);
			Object* pbo = enemyCharacter2.object[i];
			if(enemyCharacter2.collider[i]->hits[0] != NULL)
			{
				for(int ci = 0; ci < COLLIDER2D_HITS_NUM; ci++)
				{
					if(strncmp(enemyCharacter2.collider[i]->hits[ci]->tag, "trrn", 4) == 0)
					{
						enemyCharacter2.speed[i] *= -1;
						isTerrain = true;
					}
				}
				if(isTerrain == false)
				{
					DPIF(false, "%p is destroyed by %p\n", enemyCharacter2.collider[i], enemyCharacter2.collider[i]->hits[0]);
					Object_set_invalid(pbo);
					enemyCharacter2.collider[i]->isValid = false;
					Audio_play(enemyCharacter2.audioDestroy);
				}
			}
			bool isValid;
			Object_is_valid(pbo, &isValid);

			enemyCharacter1.object[i]->transform->position.x += spf * enemyCharacter1.speed;

			if(isValid == true)
			{
				// shot randomly
				double t;
				Timer_get_count(enemyCharacter2.timer[i], &t);
				if(t > enemyCharacter2.interval && rand()%shotChance == 0)
				{
					Timer_reset_count(enemyCharacter2.timer[i]);
					Vector2D v;
					v.x = cos(enemyCharacter2.cannonObject[i]->transform->rotation.w/180.0*M_PI);
					v.y = sin(enemyCharacter2.cannonObject[i]->transform->rotation.w/180.0*M_PI);
					Vector2D_normalize(&v);
					shotEnemyBullet0(&(enemyCharacter2.cannonObject[i]->transform->position), &v);
				}
			}
		if(pbo->transform->position.x < -50)
		{
			Object_set_invalid(pbo);
			enemyCharacter2.collider[i]->isValid = false;
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


void addEnemyCharacter0(Vector2D* const pos, const int start, const int end)
{
	for(int i = start; i < end; i++)
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
			//break;
		}
		else
		{
			DEIF(true, "couldnt add enemy %d\n", i);
			break;
		}
	}
}
void addEnemyCharacter1(Vector2D* const pos, const double offset, const int start, const int end)
{
	for(int i = start; i < end; i++)
	{
		bool isValid;
		Object_is_valid(enemyCharacter1.object[i], &isValid);
		if(isValid == false)
		{
			DPIF(false, "i:%d\n", i);
			Object_set_valid(enemyCharacter1.object[i]);
			enemyCharacter1.collider[i]->isValid = true;
			pos->y += (i-start)*offset;
			Vector2D_set(&(enemyCharacter1.object[i]->transform->position), pos);
			Timer_set_count(enemyCharacter1.timer[i], enemyCharacter1.interval);
			//break;
		}
		else
		{
			DEIF(true, "couldnt add an enemy %d\n", i);
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

	Timer_count(enemyCharacter1.masterTimer);

	Timer_count(powerupCore.masterTimer);
	Timer_count(destroyEffect0.masterTimer);

	for(int i = 0; i < ENEMY_BULLET0_DEFAULT_NUM; i++)
	{
		Timer_count(enemyBullet0.timer[i]);
	}

	for(int i = 0; i < ENEMY_CHARACTER0_DEFAULT_NUM; i++)
	{
		Timer_count(enemyCharacter0.timer[i]);
	}

	for(int i = 0; i < ENEMY_CHARACTER1_DEFAULT_NUM; i++)
	{
		Timer_count(enemyCharacter1.timer[i]);
	}

	for(int i = 0; i < DESTROY_EFFECT_NUM; i++)
	{
		Timer_count(destroyEffect0.timer[i]);
	}

	Timer_count(backTimer);
	Timer_count(stage0Timer);
	Timer_count(sphinx.timer);
}

void initPowerupCore()
{

	PowerupCore* eb = &powerupCore;

	eb->masterObject = Object_new();
	eb->image0 = Image2D_new();
	eb->audioGet = Audio_new(POWERUPCORE_DEFAULT_NUM);
	eb->masterTimer = Timer_new();
	
	Image2D_load(eb->image0, "resource/image/core/powerupcore/0.png");
	Audio_load(eb->audioGet, "resource/audio/enemy/bullet/shot0.wav");

	for(int i = 0; i < POWERUPCORE_DEFAULT_NUM; i++)
	{
		Object* pbo;
		Collider2D* pbc;
		RectCollider* rc;

		eb->object[i] = Object_new();
		eb->imageObject[i] = Object_new();
		eb->collider[i] = Collider2D_new();
		pbo = eb->object[i];
		pbc = eb->collider[i];
		pbo->transform->position.x = -1000;
		eb->imageObject[i]->transform->scale.x = 0.5;
		eb->imageObject[i]->transform->scale.y = 0.5;


		strncpy(pbc->tag, "pwcr", COLLIDER2D_TAG_LENGTH);
		Collider2D_set_collider_object(pbc, COLLIDER2D_COLLIDER_RECT );
		Collider2D_register_collider(pbc, 23);
		rc = (RectCollider*)(pbc->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = 20;
		rc->size.y = 20;

		Object_add_component(eb->imageObject[i], eb->image0);
		Object_add_component(pbo, eb->imageObject[i]);
		Object_add_component(pbo, pbc);
		Object_add_component(eb->masterObject, pbo);

		Object_set_invalid(pbo);
	}
}

void addPowerupCore(Vector2D* const pos, const char tag[COLLIDER2D_TAG_LENGTH])
{
	for(int i = 0; i < POWERUPCORE_DEFAULT_NUM; i++)
	{
		bool isValid;
		Object_is_valid(powerupCore.object[i], &isValid);
		if(isValid == false)
		{
			DPIF(false, "i:%d\n", i);
			Object_set_valid(powerupCore.object[i]);
			powerupCore.collider[i]->isValid = true;
			strncpy(powerupCore.collider[i]->tag, tag, COLLIDER2D_TAG_LENGTH);
			Vector2D_set(&(powerupCore.object[i]->transform->position), pos);
			return;
		}
	}
	DEIF(true, "couldnt add a powercore\n");
}

void movePowerupCore()
{
	double spf;
	Timer_get_spf(powerupCore.masterTimer, &spf);
	PowerupCore* eb = &powerupCore;
	for(int i = 0; i < POWERUPCORE_DEFAULT_NUM; i++)
	{

		Object* pbo = eb->object[i];
		if(eb->collider[i]->hits[0] != NULL)
		{
			DPIF(false, "hit\n");
			Object_set_invalid(pbo);
			eb->collider[i]->isValid = false;
			Audio_play(powerupCore.audioGet);
		}
		bool isValid;
		Object_is_valid(pbo, &isValid);

		if(isValid == true)
		{
			pbo->transform->position.x -= spf * POWERUPCORE_DEFAULT_SPEED;

			if(pbo->transform->position.x < -100 || pbo->transform->position.y < 0 || view->window_height+8 < pbo->transform->position.y)
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


void initBackground()
{
	backbase = Object_new();
	back1 = Object_new();
	back2 = Object_new();
	back3 = Object_new();
	back1s = Object_new();
	back2s = Object_new();
	back3s = Object_new();
	backTimer = Timer_new();

	Image2D* bbi;
	Image2D* b1i;
	Image2D* b2i;
	Image2D* b3i;

	bbi = Image2D_new();
	b1i = Image2D_new();
	b2i = Image2D_new();
	b3i = Image2D_new();

	bbi->option = IMAGE2D_TOP_LEFT;
	b1i->option = IMAGE2D_TOP_LEFT;
	b2i->option = IMAGE2D_TOP_LEFT;
	b3i->option = IMAGE2D_TOP_LEFT;
	
	Image2D_load(bbi, "resource/image/stage/back/backbase.png");
	Image2D_load(b1i, "resource/image/stage/back/back1.png");
	Image2D_load(b2i, "resource/image/stage/back/back2.png");
	Image2D_load(b3i, "resource/image/stage/back/back3.png");
	
	Object_add_component(backbase, bbi);
	Object_add_component(back1, b1i);
	Object_add_component(back2, b2i);
	Object_add_component(back3, b3i);

	Object_add_component(back1s, b1i);
	Object_add_component(back2s, b2i);
	Object_add_component(back3s, b3i);

	Object_add_component(backbase, back1);
	Object_add_component(backbase, back2);
	Object_add_component(backbase, back3);

	Object_add_component(backbase, back1s);
	Object_add_component(backbase, back2s);
	Object_add_component(backbase, back3s);

	bbi->p_transform->scale.x = WINDOW_WIDTH/1600.0;
	bbi->p_transform->scale.y = WINDOW_HEIGHT/900.0;

	b1i->p_transform->scale.x = 1.2;
	b1i->p_transform->scale.y = 1.2;

	b2i->p_transform->scale.x = 1.2;
	b2i->p_transform->scale.y = 1.2;

	b3i->p_transform->scale.x = 1.2;
	b3i->p_transform->scale.y = 1.2;

	Image2D_get_width(b1i, &backgroundWidth);
	back1s->transform->position.x = backgroundWidth;
	back2s->transform->position.x = backgroundWidth;
	back3s->transform->position.x = backgroundWidth;
}

void moveBackground()
{
	double spf;
	Timer_get_spf(backTimer, &spf);
	
	back1->transform->position.x -= spf * BACKGROUND1_SPEED;
	back2->transform->position.x -= spf * BACKGROUND2_SPEED;
	back3->transform->position.x -= spf * BACKGROUND3_SPEED;

	back1s->transform->position.x -= spf * BACKGROUND1_SPEED;
	back2s->transform->position.x -= spf * BACKGROUND2_SPEED;
	back3s->transform->position.x -= spf * BACKGROUND3_SPEED;

	double* px;
	
	for(int i = 0; i < 6; i++)
	{
		switch(i)
		{
			case 0: px = &(back1->transform->position.x);break;
			case 1:	px = &(back2->transform->position.x);break;
			case 2:	px = &(back3->transform->position.x);break;
			case 3:	px = &(back1s->transform->position.x);break;
			case 4:	px = &(back2s->transform->position.x);break;
			case 5:	px = &(back3s->transform->position.x);break;
		}
		if(*px < -backgroundWidth*1.2)
		{
			*px = backgroundWidth*1.2;
		}
	}

}


void initDestroyEffect()
{
	DestroyEffect* eb = &destroyEffect0;

	eb->masterObject = Object_new();
	eb->image0 = Image2D_new();
	eb->masterTimer = Timer_new();
	eb->fadeTime = DESTROY_EFFECT_FADETIME;
	
	Image2D_load(eb->image0, "resource/image/enemy/plane/destroy.png");
	eb->image0->option = IMAGE2D_CENTER;

	for(int i = 0; i < DESTROY_EFFECT_NUM; i++)
	{
		Object* pbo;

		eb->object[i] = Object_new();
		pbo = eb->object[i];
		pbo->transform->position.x = -1000;

		eb->timer[i] = Timer_new();

		Object_add_component(pbo, eb->image0);
		Object_add_component(eb->masterObject, pbo);

		Object_set_invalid(pbo);
	}
}

void addDestroyEffect(Vector2D* const pos)
{
	for(int i = 0; i < DESTROY_EFFECT_NUM; i++)
	{
		bool isValid;
		Object_is_valid(destroyEffect0.object[i], &isValid);
		if(isValid == false)
		{
			DPIF(false, "i:%d\n", i);
			Timer_reset_count(destroyEffect0.timer[i]);
			Object_set_valid(destroyEffect0.object[i]);
			Vector2D_set(&(destroyEffect0.object[i]->transform->position), pos);
			return;
		}
	}
	DEIF(true, "couldnt add a destroy effect\n");
}

void moveDestroyEffect()
{
	DestroyEffect* eb = &destroyEffect0;
	for(int i = 0; i < DESTROY_EFFECT_NUM; i++)
	{
		Object* pbo = eb->object[i];

		bool isValid;
		Object_is_valid(pbo, &isValid);

		if(isValid == true)
		{
			double count;
			Timer_get_count(eb->timer[i], &count);
			if(count > eb->fadeTime)
			{
				Object_set_invalid(pbo);
			}
			else
			{
				pbo->transform->scale.x = (DESTROY_EFFECT_FADETIME*DESTROY_EFFECT_FADETIME - pow(DESTROY_EFFECT_FADETIME - count, 2))*60;
				pbo->transform->scale.y = (DESTROY_EFFECT_FADETIME*DESTROY_EFFECT_FADETIME - pow(DESTROY_EFFECT_FADETIME - count, 2))*60;
			}

		}
		else
		{
			//pbo->transform->position.x = view->window_width;
			//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

		}
	}
}

    // // // // // // // // // // // // // // // // // // // // // // // // // // //
   // // // // // // // // // // // // // // // // // // // // // // // // // // // 
  // // // // // // // // // //  S  T  A  G  E  0 // // // // // // // // // // //
 // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // 


Animation2D* wave0childAnimation;
Animation2D* wave0animation;

Animation2D* wave1childAnimation;
Animation2D* wave1animation;

Animation2D* wave2childAnimation;
Animation2D* wave2animation;

Animation2D* wave3childAnimation;
Animation2D* wave3animation;

Animation2D* wave4childAnimation;
Animation2D* wave4animation;

void stage0_init()
{
	Object* pyramidObj0;
	Object* groundObj0;
	Image2D* pyramidImg;
	Image2D* groundImg;
	Collider2D* pyramidCol;
	Collider2D* groundCol;

        pyramidObj0 = Object_new();
	groundObj0 = Object_new();
	pyramidImg = Image2D_new();
	groundImg = Image2D_new();
	pyramidCol = Collider2D_new();
	groundCol= Collider2D_new();

	stage0 = Object_new();
	Object_set_invalid(stage0);

	Object_add_component(pyramidObj0, pyramidImg);
	Object_add_component(pyramidObj0, pyramidCol);

	Object_add_component(groundObj0, groundImg);
	Object_add_component(groundObj0, groundCol);

	Object_add_component(stage0, pyramidObj0);
	Object_add_component(stage0, groundObj0);

	Image2D_load(groundImg, "resource/image/stage/stage0/ground.png");
	Image2D_load(pyramidImg, "resource/image/stage/stage0/pyramid.png");

	Collider2D_set_collider_object(groundCol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(pyramidCol, COLLIDER2D_COLLIDER_RECT);

	RectCollider* grc = (RectCollider*)(groundCol->colObj);
	RectCollider* prc = (RectCollider*)(pyramidCol->colObj);

	Vector2D_set_zero(&(grc->position));
	Vector2D_set_zero(&(prc->position));

	grc->size.x = 60;
	grc->size.y = 60;

	prc->size.x = 60;
	prc->size.y = 60;

	groundObj0->transform->position.x = 5000;
	groundObj0->transform->position.y = WINDOW_HEIGHT-50;
	groundObj0->transform->scale.x = 10000;
	groundObj0->transform->scale.y = 10000;

	pyramidObj0->transform->scale.x = 4;
	pyramidObj0->transform->scale.y = 4;
	pyramidObj0->transform->position.x =6000;
	pyramidObj0->transform->position.y = WINDOW_HEIGHT-50-30*sqrt(pow(pyramidObj0->transform->scale.x,2) + pow(pyramidObj0->transform->scale.y,2));
	pyramidObj0->transform->rotation.z = 1;
	pyramidObj0->transform->rotation.w = 45;

	Collider2D_register_collider(groundCol, 20);
	Collider2D_register_collider(pyramidCol, 20);


	stage0Timer = Timer_new();

	wave0childAnimation = Animation2D_new();
	wave0animation = Animation2D_new();

	wave1childAnimation = Animation2D_new();
	wave1animation = Animation2D_new();

	wave2childAnimation = Animation2D_new();
	wave2animation = Animation2D_new();

	wave3childAnimation = Animation2D_new();
	wave3animation = Animation2D_new();

	wave4childAnimation = Animation2D_new();
	wave4animation = Animation2D_new();

	struct waveargstruct was;
	was.exponent = 3.0;
	was.stopRatio= 27.0;
	was.xCenter  = WINDOW_WIDTH/2.0+100;
	was.yStart   = -70.0;
	was.yLength  = 800.0;
	was.indexoffset = 4.0;

	double radius = 8;

	setStageAnimation(wave0animation, 64, enemyCharacter0.object, 0.2, 0, 6, moveStopMove, &was);
	setStageAnimation(wave0childAnimation, 16, enemyCharacter0.childObject, 0.1, 0, 6, circleCircleCircle, &radius);


	was.exponent = 3.0;
	was.stopRatio= 27.0;
	was.xCenter  = WINDOW_WIDTH/2.0+100;
	was.yStart   = WINDOW_HEIGHT+70.0;
	was.yLength  = -800.0;
	was.indexoffset = 3.0;

	setStageAnimation(wave1animation, 64, enemyCharacter0.object, 0.2, 6, 12, moveStopMove, &was);
	setStageAnimation(wave1childAnimation, 16, enemyCharacter0.childObject, 0.1, 6, 12, circleCircleCircle, &radius);

	was.exponent = 3.0;
	was.stopRatio= 16.0;
	was.xCenter  = WINDOW_WIDTH/2.0+200;
	was.yStart   = WINDOW_HEIGHT/2.0;
	was.yLength  = 300.0;
	was.indexoffset = 1.6;

	setStageAnimation(wave2animation, 64, enemyCharacter0.object, 0.2, 0, 8, moveStopMove, &was);
	setStageAnimation(wave2childAnimation, 16, enemyCharacter0.childObject, 0.1, 0, 8, circleCircleCircle, &radius);

	was.exponent = 3.0;
	was.stopRatio= 50.0;
	was.xCenter  = WINDOW_WIDTH/2.0+300;
	was.yStart   = -400;
	was.yLength  = 800.0;
	was.indexoffset = 2;

	setStageAnimation(wave3animation, 64, enemyCharacter0.object, 0.2, 8, 16, moveStopMove, &was);
	setStageAnimation(wave3childAnimation, 16, enemyCharacter0.childObject, 0.1, 8, 16, circleCircleCircle, &radius);

	was.exponent = 3.0;
	was.stopRatio= 16.0;
	was.xCenter  = WINDOW_WIDTH/2.0+200;
	was.yStart   = WINDOW_HEIGHT+500;
	was.yLength  = -600.0;
	was.indexoffset = 0.5;

	radius = 20;

	setStageAnimation(wave4animation, 64, enemyCharacter0.object, 0.2, 16, 24, moveStopMoveStraight, &was);
	setStageAnimation(wave4childAnimation, 16, enemyCharacter0.childObject, 0.1, 16, 24, circleCircleCircle, &radius);
}

void stage0_update()
{
	double st;
	double spf;
	Timer_get_count(stage0Timer, &st);
	Timer_get_spf(stage0Timer, &spf);

	const double stage0appear = 0.01;
	static bool stage0v = false;

	const double enemyWave0start = 1.0;
	const double enemyWave0length = 12.8;
	static bool enemyWave0 = false;

	const double enemyWave1start = 6.0;
	const double enemyWave1length = 12.8;
	static bool enemyWave1 = false;

	const double enemyFollow0start = 10.0;
	const double enemyFollow0length = 100.0;
	static bool enemyFollow0 = false;

	const double enemyFollow1start = 13.0;
	const double enemyFollow1length = 100.0;
	static bool enemyFollow1 = false;

	const double enemyWave2start = 20.0;
	const double enemyWave2length = 12.8;
	static bool enemyWave2 = false;

	const double enemyWave3start = 25.0;
	const double enemyWave3length = 12.8;
	static bool enemyWave3 = false;

	const double enemyFollow2start = 30.0;
	const double enemyFollow2length = 100.0;
	static bool enemyFollow2 = false;

	const double enemyWave4start = 35.0;
	const double enemyWave4length = 12.0;
	static bool enemyWave4 = false;


	Vector2D v;
	v.x = WINDOW_WIDTH + 200;
	v.y = WINDOW_HEIGHT / 2;
	if(Keyboard_is_pressed('u'))
		addPowerupCore(&v, POWERUPCORE_SPEEDUP_TAG);

	if(enemyWave0 == true){Animation2D_play(wave0childAnimation);
			       Animation2D_play(wave0animation);}

	if(enemyWave1 == true){Animation2D_play(wave1childAnimation);
			       Animation2D_play(wave1animation);}

	if(enemyWave2 == true){Animation2D_play(wave2childAnimation);
			       Animation2D_play(wave2animation);}

	if(enemyWave3 == true){Animation2D_play(wave3childAnimation);
			       Animation2D_play(wave3animation);}

	if(enemyWave4 == true){Animation2D_play(wave4childAnimation);
			       Animation2D_play(wave4animation);}

	// wave 0 
	if(enemyWave0 == false && st > enemyWave0start && st < enemyWave0start + enemyWave0length) {addEnemyCharacter0(&v, 0, 6); enemyWave0 = true;}		
	if(st > enemyWave0start + enemyWave0length){enemyWave0 = false;Animation2D_reset(wave0animation);Animation2D_reset(wave0childAnimation);}

	// wave 1 
	if(enemyWave1 == false && st > enemyWave1start && st < enemyWave1start + enemyWave1length) {addEnemyCharacter0(&v, 6, 12); enemyWave1 = true;}		
	if(st > enemyWave1start + enemyWave1length){enemyWave1 = false;Animation2D_reset(wave1animation);Animation2D_reset(wave1childAnimation);}

	// follow 0 
	v.y = 100;
	if(enemyFollow0 == false && st > enemyFollow0start && st < enemyFollow0start + enemyFollow0length) {addEnemyCharacter1(&v, 100, 0, 2); enemyFollow0 = true;}		
	if(st > enemyFollow0start + enemyFollow0length){enemyFollow0 = false;}

	// follow 1
	v.y = WINDOW_HEIGHT - 100;
	if(enemyFollow1 == false && st > enemyFollow1start && st < enemyFollow1start + enemyFollow1length) {addEnemyCharacter1(&v, -100, 2, 4); enemyFollow1 = true;}		
	if(st > enemyFollow1start + enemyFollow1length){enemyFollow1 = false;}

	v.x = WINDOW_WIDTH + 200;
	v.y = WINDOW_HEIGHT / 2;
	// wave 2 
	if(enemyWave2 == false && st > enemyWave2start && st < enemyWave2start + enemyWave2length) {addEnemyCharacter0(&v, 0, 8); enemyWave2 = true;DPIF(false,"w2\n");}		
	if(st > enemyWave2start + enemyWave2length){enemyWave2 = false;Animation2D_reset(wave2animation);Animation2D_reset(wave2childAnimation);}

	// wave 3 
	if(enemyWave3 == false && st > enemyWave3start && st < enemyWave3start + enemyWave3length) {addEnemyCharacter0(&v, 8, 16); enemyWave3 = true;DPIF(false,"w3\n");}		
	if(st > enemyWave3start + enemyWave3length){enemyWave3 = false;Animation2D_reset(wave3animation);Animation2D_reset(wave3childAnimation);}

	// stage 
	stage0->transform->position.x -= stage0_moving_speed*spf;

	if(st > stage0appear && stage0v == false){Object_set_valid(stage0); stage0v = true;}

	// follow 2
	v.y = 0;
	if(enemyFollow2 == false && st > enemyFollow2start && st < enemyFollow2start + enemyFollow2length) {addEnemyCharacter1(&v, 50, 4, 6); enemyFollow2 = true;}		
	if(st > enemyFollow2start + enemyFollow2length){enemyFollow2 = false;}

	v.y = WINDOW_HEIGHT / 2;
	// wave 4 
	if(enemyWave4 == false && st > enemyWave4start && st < enemyWave4start + enemyWave4length) {addEnemyCharacter0(&v, 16, 24); enemyWave4 = true;DPIF(true,"w4\n");}		
	if(st > enemyWave4start + enemyWave4length){enemyWave4 = false;Animation2D_reset(wave4animation);Animation2D_reset(wave4childAnimation);}

}


void moveStopMove(Vector2D* const ret, const int f, const int i, const int fmax, const int imin, const int imax, const double framelength, void* waveArgStruct)
{
	Vector2D v;
	struct waveargstruct* was = (struct waveargstruct *)waveArgStruct;

	// calc here       (ex. x = cos(f)*10, y = sin(f)*20
	// !!! (f+i) !!!
	
	double t = (( f - fmax/2.0 ) + ( (i+6) - imax/2.0)/was->indexoffset) * 0.5;

	v.x = was->xCenter -
		(
		 	pow(t, was->exponent) - was->stopRatio*t
		);

	t = (((double)f/fmax) + ((double)i/imax)/was->indexoffset);

	v.y = was->yStart +
		(
		 	// !!!!! 0 < t < 1 !!!!!
			(-2.0*pow(t, 3.0) + 3.0*pow(t, 2.0)) * was->yLength
			//sin(t) * was->yLength
		);

	Vector2D_set(ret, &v);
}

void circleCircleCircle(Vector2D* const ret, const int f, const int i, const int fmax, const int imin, const int imax, const double framelength, void* radiusd)
{
	// calc here       (ex. x = cos(f)*10, y = sin(f)*20
	// !!! (f+i) !!!
	Vector2D v;
	double t = (f / 8.0 * M_PI) + ( i / 2.0 );

	v.x =
		(
		 	cos(t) * *(double*)radiusd
		);

	v.y =
		(
			sin(t) * *(double*)radiusd
		);
	Vector2D_set(ret, &v);
}

void moveStopMoveStraight(Vector2D* const ret, const int f, const int i, const int fmax, const int imin, const int imax, const double framelength, void* waveArgStruct)
{
	Vector2D v;
	struct waveargstruct* was = (struct waveargstruct *)waveArgStruct;

	// calc here       (ex. x = cos(f)*10, y = sin(f)*20
	// !!! (f+i) !!!
	
	double t = (( f - fmax/2.0 )) * 0.4;

	v.x = was->xCenter -
		(
		 	pow(t, was->exponent) - was->stopRatio*t
		);

	t = (((double)i/imax)/was->indexoffset);

	v.y = was->yStart +
		(
		 	// !!!!! 0 < t < 1 !!!!!
			(double)was->yLength*t
			//sin(t) * was->yLength
		);

	Vector2D_set(ret, &v);
}


void setStageAnimation(Animation2D* const anm, const int fnum, Object** const object, const double framelength, const int istart, const int iend, void func(Vector2D*, int, int, int, int, int , double, void*), void* arg)
{
	for(int f = 0; f < fnum; f++) // 64 * 0.2 : 12.8
	{
		Animation2D_add_frame(anm);
		Animation2D_set_frame_length(anm, f, framelength);// every 0.2 second
		// anm, frame index, varaddr, valaddr, sizeofval, smoothingType
		for(int i = istart; i < iend; i++)
		{
			// calc here       (ex. x = cos(f)*10, y = sin(f)*20
			// !!! (f+i) !!!
		
			Vector2D v;
			func(&v, f, i, fnum, istart, iend, framelength, arg);
			
			Animation2D_add_animated_variable(anm, f, &(object[i]->transform->position), &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_VECTOR2D);
		}
	}

}
