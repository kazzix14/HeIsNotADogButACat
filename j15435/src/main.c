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

#define PLAYER_CHARACTER_LIMIT_DOUBLE 8
#define PLAYER_CHARACTER_DEFAULT_SPEED 130
#define PLAYER_CHARACTER_LIMIT_SPEED 800
#define PLAYER_CHARACTER_DEFAULT_HP 100
#define PLAYER_CHARACTER_BULLET_DEFAULT_SPEED 800
#define PLAYER_CHARACTER_BULLET_LIMIT_SPEED 1600
#define PLAYER_CHARACTER_BULLET_LIMIT_SCALE 5.3
#define PLAYER_CHARACTER_BULLET_DEFAULT_INTERVAL 0.01
#define PLAYER_CHARACTER_DEFAULT_BULLET_NUM 1
#define PLAYER_CHARACTER_BULLET_NUM_LIMIT 64

#define ENEMY_CHARACTER0_DEFAULT_NUM 32
#define ENEMY_CHARACTER0_DEFAULT_SPEED 200
#define ENEMY_CHARACTER0_DEFAULT_ACCELERATION 0
#define ENEMY_CHARACTER0_DEFAULT_INTERVAL 6
#define ENEMY_BULLET0_DEFAULT_NUM 82
#define ENEMY_CHARACTER_BULLET0_DEFAULT_SPEED 350

#define ENEMY_CHARACTER1_DEFAULT_NUM 32
#define ENEMY_CHARACTER1_DEFAULT_SPEED 100
#define ENEMY_CHARACTER1_DEFAULT_ACCELERATION 0
#define ENEMY_CHARACTER1_DEFAULT_INTERVAL 6

#define ENEMY_CHARACTER2_DEFAULT_NUM 64
#define ENEMY_CHARACTER2_DEFAULT_SPEED 30
#define ENEMY_CHARACTER2_DEFAULT_ACCELERATION 0
#define ENEMY_CHARACTER2_DEFAULT_INTERVAL 3 

#define POWERUPCORE_DEFAULT_NUM 16
#define POWERUPCORE_DEFAULT_SPEED 100
#define POWERUPCORE_OBTAIN_SPEED 30
#define POWERUPCORE_BULLETSCALE 1.54
#define POWERUPCORE_OBTAIN_BULLETSPEED 200
#define POWERUPCORE_SPEEDUP_TAG "pusp"
#define POWERUPCORE_BULLETSPEEDUP_TAG "pubs"
#define POWERUPCORE_BULLETNUMINCREASE_TAG "pubn"

#define BACKGROUND1_SPEED 10
#define BACKGROUND2_SPEED 20
#define BACKGROUND3_SPEED 30

#define DESTROY_EFFECT_FADETIME 0.15
#define DESTROY_EFFECT_NUM 64

#define SPHINX_DEFAULT_HP1 127
#define SPHINX_DEFAULT_HP2 127

#define SCORE_STRING_LENGTH 21

#define DAMAGE_TERRAIN_BASE 84
#define DAMAGE_ENEMY_CHARACTER0_BASE 71
#define DAMAGE_ENEMY_CHARACTER1_BASE 61
#define DAMAGE_ENEMY_CHARACTER2_BASE 53
#define DAMAGE_ENEMY_BULLET0_BASE 11
#define DAMAGE_SPHINX_BASE 14
#define DAMAGE_SPHINX_LAZER_BASE 13

typedef struct playerbullet0 PlayerBullet0;
typedef struct enemybullet0 EnemyBullet0;

// struct definitions
// {{{
typedef struct playercharacter
{
	Object* object;
	Object* imageObject;
	Collider2D* collider;
	Image2D* image0;
	double speed;
	double hp;

	Timer* timer;
} PlayerCharacter;

typedef struct playerbullet0
{
	Object* masterObject;
	Object* object[PLAYER_CHARACTER_BULLET_NUM_LIMIT];
	Collider2D* collider[PLAYER_CHARACTER_BULLET_NUM_LIMIT];
	Image2D* image0;
	Audio* audioShot;
	Vector2D direction[PLAYER_CHARACTER_BULLET_NUM_LIMIT];

	double speed;
	double interval;
	int bulletNum;

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
	Collider2D* collidert[ENEMY_CHARACTER2_DEFAULT_NUM];
	Image2D* image0; // tire
	Image2D* image1; // cannon
	double speed[ENEMY_CHARACTER2_DEFAULT_NUM];
	double acceleration;
	double interval;

	Timer* masterTimer;
	Timer* timer[ENEMY_CHARACTER2_DEFAULT_NUM];
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
	Timer* lazerTimer;

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
	Audio* audioDestroy;
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
// }}}

// prototypes
// {{{
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
void setSphinxColInvalid();
void setSphinxColValid();

void initEnemyCharacter0();
void initEnemyBullet0();
void moveEnemyCharacter0();
void moveEnemyBullet0();

void initEnemyCharacter1();
void moveEnemyCharacter1();

void initEnemyCharacter2();
void moveEnemyCharacter2();

void addEnemyCharacter0(Vector2D* const pos, const int start, const int end);
void addEnemyCharacter1(Vector2D* const pos, const double offset, const int start, const int end);
void addEnemyCharacter2(Vector2D* const pos);
void shotEnemyBullet0(Vector2D* const src, Vector2D* const dict);

void updateTimer();

void initPowerupCore();
void movePowerupCore();
void addPowerupCore(Vector2D* const pos);

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

void updateScoreObj();
void incScore(unsigned long long);
// }}}

    // // // // // // // // // // // // // // // // // // // // // // // // // // //
   // // // // // // // // // // // // // // // // // // // // // // // // // // // 
  // // // // // // // // // // // // // // // // // // // // // // // // // // //
 // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // 

// variable
// {{{
View* view;

double totaltime = 0;


Object* dfxObj;
Image2D* damageEffect;


PlayerCharacter playerCharacter;
PlayerBullet0 playerBullet0;
long long numDestroy = 0;
int pbDouble;

EnemyCharacter0 enemyCharacter0;
EnemyCharacter0 enemyCharacter1;
EnemyCharacter2 enemyCharacter2;

EnemyBullet0 enemyBullet0;

PowerupCore powerupCore;
int powerupSelecter;

DestroyEffect destroyEffect0;;

Boss0 sphinx;
bool sphinxNow;
double timeSphinxApper = 80;
int sphinxShotChance = 100;
double sphinxLazerInterval = 0.6;
Object* scoreObject;

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
double stage0ground0Offset;
Timer* stage0Timer;

unsigned long long currentLap;
unsigned long long score;
const unsigned long long enemy0score = 100;
const unsigned long long enemy1score = 200;
const unsigned long long enemy2score = 150;
const unsigned long long powerupCoreScore = 500;
const unsigned long long sphinxScore = 10000;


int enc0shotChance = 250;
int enc1shotChance = 20;
int enc2shotChance = 70;

// }}}

// main
// {{{
int main(int argc, char **argv)
{
	srand(1);
	init_gl(&argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT, "UNTI!!");
	setColliderLayerMatrix();
	
	view = View_new();
	view->position.z = 1;

	view->window_width = WINDOW_WIDTH;
	view->window_height = WINDOW_HEIGHT;

	currentLap = 1;

	initPlayerCharacter();
	initPlayerBullet0();
	initEnemyCharacter0();
	initEnemyBullet0();
	initEnemyCharacter1();
	initEnemyCharacter2();
	initSphinx();

	initDestroyEffect();

	initPowerupCore();

	initBackground();
	stage0_init();

	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	
	return(0);
}
// }}}

// display
// {{{
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
		Object_put(enemyCharacter2.masterObject);
		Object_put(destroyEffect0.masterObject);
		Object_put(playerBullet0.masterObject);
		Object_put(enemyBullet0.masterObject);
		Object_put(powerupCore.masterObject);
		Object_put(scoreObject);
		Collider2D_judge_all();
	View_end_2d();

	glFlush();
	glutSwapBuffers();
}
// }}}

/// timer
// {{{
void timer(int value)
{
	// 60 fps
	glutTimerFunc(16, timer, 0);
	glutPostRedisplay();

	updateTimer();

	updateScoreObj();

	moveBackground();

	stage0_update();

	movePlayerBullet0();
	movePlayerCharacter();
	moveEnemyCharacter0();
	moveEnemyCharacter1();
	moveEnemyCharacter2();
	moveEnemyBullet0();
	movePowerupCore();
	moveDestroyEffect();
	moveSphinx();


	fps();
}
// }}}

// reshape 
// {{{
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
//	view->window_width = w;
//	view->window_height = h;
} 
// }}}

// init_gl
// {{{
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
// }}}

// fps
// {{{
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
		DPIF(false, "fps:%2d\n", count);
		ts = tc;
		count = 0;
	}

}
// }}}

// mouse
// {{{
void mouse(int b, int s, int x, int y)
{

}
// }}}

// keyboard
// {{{
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

	Keyboard_update();
}
// }}}

// set collider layer matrix
// {{{
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
// }}}

// init pc pb0
// {{{
void initPlayerCharacter()
{
	pbDouble = 0;
	powerupSelecter = 0;
	playerCharacter.object = Object_new();
	playerCharacter.imageObject = Object_new();
	playerCharacter.collider = Collider2D_new();
	playerCharacter.image0 = Image2D_new();
	playerCharacter.timer = Timer_new();
	playerCharacter.speed = PLAYER_CHARACTER_DEFAULT_SPEED;
	playerCharacter.hp = PLAYER_CHARACTER_DEFAULT_HP;

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
	playerBullet0.audioShot = Audio_new(PLAYER_CHARACTER_BULLET_NUM_LIMIT+16);
	playerBullet0.bulletNum = PLAYER_CHARACTER_DEFAULT_BULLET_NUM;
	
	Image2D_load(playerBullet0.image0, "resource/image/player/bullet/0.png");
	Audio_load(playerBullet0.audioShot, "resource/audio/player/bullet/shot0.wav");

	Timer_reset_count(playerBullet0.timer);
	playerBullet0.speed = PLAYER_CHARACTER_BULLET_DEFAULT_SPEED;
	playerBullet0.interval = PLAYER_CHARACTER_BULLET_DEFAULT_INTERVAL;

	for(int i = 0; i < PLAYER_CHARACTER_BULLET_NUM_LIMIT; i++)
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
		rc->size.y = 8;
		pbo->transform->scale.y = 0.5;

		Object_add_component(pbo, playerBullet0.image0);
		Object_add_component(pbo, pbc);
		Object_add_component(playerBullet0.masterObject, pbo);

		Object_set_invalid(pbo);
	}
}
// }}}

// move pc
// {{{
void movePlayerCharacter()
{
	if(Keyboard_is_pressed('u') || Keyboard_is_pressed('e'))
	{
		switch(powerupSelecter)
		{
			case 1:// speed
				if(playerCharacter.speed < PLAYER_CHARACTER_LIMIT_SPEED)
				{
					playerCharacter.speed += POWERUPCORE_OBTAIN_SPEED;
					powerupSelecter = 0;
				}
				break;
			case 2:// bspeed
				if(playerBullet0.speed < PLAYER_CHARACTER_BULLET_LIMIT_SPEED)
				{
					playerBullet0.speed += POWERUPCORE_OBTAIN_BULLETSPEED;
					powerupSelecter = 0;
				}
				break;
			case 3://bscale
				if(playerBullet0.object[0]->transform->scale.x < PLAYER_CHARACTER_BULLET_LIMIT_SCALE)
				{
					for(int i = 0; i < PLAYER_CHARACTER_BULLET_NUM_LIMIT; i++)
					{
						playerBullet0.object[i]->transform->scale.x *= POWERUPCORE_BULLETSCALE;
						playerBullet0.object[i]->transform->scale.y *= POWERUPCORE_BULLETSCALE;
						powerupSelecter = 0;
					}
				}
				break;
			case 4://bdouble
				if(pbDouble < PLAYER_CHARACTER_LIMIT_DOUBLE)
				{
					pbDouble++;
					powerupSelecter = 0;
				}
				break;
			case 5:// bnum
				if(playerBullet0.bulletNum < PLAYER_CHARACTER_BULLET_NUM_LIMIT)
				{
					playerBullet0.bulletNum++;
					powerupSelecter = 0;
				}
				break;

		}

	}
	
	DDO(
			Vector2D v;
			v.x = 1000;
			v.y = 500;
		if(Keyboard_is_pressed('n'))
			addPowerupCore(&v);
	   )

	if(Keyboard_is_down('a') || Keyboard_is_down('k'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.x -= spf * playerCharacter.speed;
	}
	if(Keyboard_is_down('d') || Keyboard_is_down(';'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.x += spf * playerCharacter.speed;
	}
	if(Keyboard_is_down('w') || Keyboard_is_down('o'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.y -= spf * playerCharacter.speed;
	}
	if(Keyboard_is_down('s') || Keyboard_is_down('l'))
	{
		double spf;
		Timer_get_spf(playerCharacter.timer, &spf);
		playerCharacter.object->transform->position.y += spf * playerCharacter.speed;
	}

	if(Keyboard_is_pressed('j') || Keyboard_is_pressed('f'))
	{
		double count;
		Timer_get_count(playerBullet0.timer, &count);
		if(count > playerBullet0.interval)
		{
			DPIF(false, "count:%f\n", count);
			Timer_reset_count(playerBullet0.timer);
			for(int i = 0; i < playerBullet0.bulletNum; i++)
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
					playerBullet0.direction[i].x = 1.0;
					playerBullet0.direction[i].y = 0.0;
					if(pbDouble > 0)
					{
						for(int a = 0; a < pbDouble; a++)
						{
							for(int i = PLAYER_CHARACTER_BULLET_NUM_LIMIT-1; i >= 0; i--)
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
									playerBullet0.direction[i].x = 1.0;
									playerBullet0.direction[i].y = (((double)rand()/(double)RAND_MAX)-0.5)*pbDouble/PLAYER_CHARACTER_LIMIT_DOUBLE;
									Vector2D_normalize(&playerBullet0.direction[i]);
									break;
								}
							}
						}
					}
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
		bool damage = false;
		char tag[COLLIDER2D_HITS_NUM][COLLIDER2D_TAG_LENGTH];
		int tagnum = 0;
		for(int i = 0; i < COLLIDER2D_HITS_NUM; i++)
		{
			if(playerCharacter.collider->hits[i] != NULL)
			{
				if(strncmp(playerCharacter.collider->hits[i]->tag, "pu", 2) == 0)
				{
					pucore = true;
					memcpy(tag[tagnum], playerCharacter.collider->hits[i]->tag, COLLIDER2D_TAG_LENGTH);
					tagnum++;
				}
				else
				{
					damage = true;
					memcpy(tag[tagnum], playerCharacter.collider->hits[i]->tag, COLLIDER2D_TAG_LENGTH);
					tagnum++;
				}
			}
		}

		if(pucore == true)
		{
			/*
			for(int i = 0; i < tagnum; i++)
			{
			
				if(strncmp(tag[i], POWERUPCORE_SPEEDUP_TAG, COLLIDER2D_TAG_LENGTH) == 0)
				{
					playerCharacter.speed += POWERUPCORE_OBTAIN_SPEED;
					DPIF(true, "pc speed up\n");
				}
				else if(strncmp(tag[i], POWERUPCORE_BULLETSPEEDUP_TAG, COLLIDER2D_TAG_LENGTH) == 0)
				{
					if(playerBullet0.speed < PLAYER_CHARACTER_BULLET_LIMIT_SPEED)
					{	
						playerBullet0.speed += POWERUPCORE_OBTAIN_BULLETSPEED;
						DPIF(true, "pb speed up\n");
					}
				}
				else if(strncmp(tag[i], POWERUPCORE_BULLETNUMINCREASE_TAG, COLLIDER2D_TAG_LENGTH) == 0)
				{
					if(playerBullet0.bulletNum < PLAYER_CHARACTER_BULLET_NUM_LIMIT-1)
					{
						playerBullet0.bulletNum += 1;
						DPIF(true, "pb num inc\n");
					}
				}
				
			}
			*/
			powerupSelecter = ( powerupSelecter + 1 ) % 6;

		}
		if(damage == true)
		{
			Object_set_valid(dfxObj);
			for(int i = 0; i < tagnum; i++)
			{
				if(strncmp(tag[i], "enc0", COLLIDER2D_TAG_LENGTH) == 0)
				{
					playerCharacter.hp -= DAMAGE_ENEMY_CHARACTER0_BASE * currentLap;
				}
				else if(strncmp(tag[i], "enc1", COLLIDER2D_TAG_LENGTH) == 0)
				{
					playerCharacter.hp -= DAMAGE_ENEMY_CHARACTER1_BASE * currentLap;
				}
				else if(strncmp(tag[i], "enc2", COLLIDER2D_TAG_LENGTH) == 0)
				{
					playerCharacter.hp -= DAMAGE_ENEMY_CHARACTER2_BASE * currentLap;
				}
				else if(strncmp(tag[i], "enb0", COLLIDER2D_TAG_LENGTH) == 0)
				{
					playerCharacter.hp -= DAMAGE_ENEMY_BULLET0_BASE* currentLap;
				}
				else if(strncmp(tag[i], "trrn", COLLIDER2D_TAG_LENGTH) == 0)//terrain
				{
					playerCharacter.hp -= DAMAGE_TERRAIN_BASE* currentLap;
				}
				else if(strncmp(tag[i], "spnx", COLLIDER2D_TAG_LENGTH) == 0)//sphinx
				{
					playerCharacter.hp -= DAMAGE_SPHINX_BASE* currentLap;
				}
				else if(strncmp(tag[i], "spxl", COLLIDER2D_TAG_LENGTH) == 0)//sphinx lazer
				{
					playerCharacter.hp -= DAMAGE_SPHINX_LAZER_BASE* currentLap;
				}
				else
				{
					DE("what is this????\n")
				}
			}
			addDestroyEffect(&playerCharacter.object->transform->position);
			if(playerCharacter.hp <= 0)
			{
				DDO(
					DPIF(true,
					   "\x1b[41m---------------------------------------------------------\n"
					   "You Died!!\n"
					   "---------------------------------------------------------\n\x1b[40m"
					);
					playerCharacter.hp = PLAYER_CHARACTER_DEFAULT_HP;
					return;
				)
				exit(0);
			}
		}
		//exit(0);
	}
	else
	{
		Object_set_invalid(dfxObj);
	}

	//
	////
}
// }}}

// move pb0
// {{{
void movePlayerBullet0()
{
	double spf;
	Timer_get_spf(playerBullet0.timer, &spf);
	for(int i = 0; i < PLAYER_CHARACTER_BULLET_NUM_LIMIT; i++)
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
						incScore(enemy0score);
						numDestroy++;
						if(sphinxNow == false && numDestroy %8 == 0)
						{
							addPowerupCore(&playerBullet0.object[i]->transform->position);
						}
					}else if(strncmp(playerBullet0.collider[i]->hits[j]->tag, "enc1", COLLIDER2D_TAG_LENGTH) == 0)
					{
						if(sphinxNow == false)
						{
							incScore(enemy1score);
						}
						numDestroy++;
						if(sphinxNow == false && numDestroy %8 == 0)
						{
							addPowerupCore(&playerBullet0.object[i]->transform->position);
						}
					}
					else if(strncmp(playerBullet0.collider[i]->hits[j]->tag, "enc2", COLLIDER2D_TAG_LENGTH) == 0)
					{
						incScore(enemy2score);
						numDestroy++;
						if(sphinxNow == false && numDestroy %8 == 0)
						{
							addPowerupCore(&playerBullet0.object[i]->transform->position);
						}
					}
				}
			}
			DP("score : %lld\n", score);
		}

		if(isValid == true)
		{
			pbo->transform->position.x += playerBullet0.speed * spf * playerBullet0.direction[i].x;
			pbo->transform->position.y += playerBullet0.speed * spf * playerBullet0.direction[i].y;
			if(pbo->transform->position.x > view->window_width || pbo->transform->position.y > WINDOW_HEIGHT || pbo->transform->position.y < 0-10)
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
// }}}

// sphinx init
//  {{{

Animation2D* bossStandUpAnim;
Animation2D* bossPunch;
Animation2D* bossSweep;
Animation2D* bossHead;
Animation2D* bossJaw;
Animation2D* bossReset;

Animation2D* bossPreLazer0;
Object* prering;
Object* prewhite;
Object* preblack;

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
	sphinx.lazerTimer = Timer_new();

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

	Audio_load(sphinx.aMao, "resource/audio/sphinx/mew1.wav"); // bye
	Audio_load(sphinx.aPunch, "resource/audio/sphinx/mew2.wav"); // punch
	Audio_load(sphinx.aLazer, "resource/audio/sphinx/prelazer.wav"); // lazer
	Audio_load(sphinx.aGass, "resource/audio/sphinx/mew3.wav");  // jaw
	Audio_load(sphinx.aBump, "resource/audio/sphinx/mew4.wav"); // sweep
	
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



	sphinxNow = false;
	double neww;
	Vector2D v;
	bool b;

	Object_set_invalid(sphinx.lazer);

	sphinx.childObject->transform->position.x = stage0ground0Offset + 850 + stage0_moving_speed * timeSphinxApper; // 60s
	DPIF(false, "sphinx child obj x : %f\n",sphinx.childObject->transform->position.x);
	sphinx.childObject->transform->position.y = WINDOW_HEIGHT - 50;

	//Object_add_component(stage0, sphinx.masterObject);

	bossStandUpAnim = Animation2D_new();
	bossPunch = Animation2D_new();
	bossSweep = Animation2D_new();
	bossHead = Animation2D_new();
	bossJaw = Animation2D_new();
	bossReset = Animation2D_new();


	bossPreLazer0 = Animation2D_new();
	Image2D* ipreblack;
	Image2D* iprewhite;
	Image2D* iprering;
	preblack = Object_new();
	prewhite = Object_new();
	prering = Object_new();
	ipreblack = Image2D_new();
	iprewhite = Image2D_new();
	iprering = Image2D_new();
	ipreblack->option = IMAGE2D_CENTER;
	iprewhite->option = IMAGE2D_CENTER;
	iprering->option = IMAGE2D_CENTER;

	Image2D_load(iprering, "resource/image/enemy/boss/prelazerring.png");
	Image2D_load(iprewhite, "resource/image/enemy/boss/prelazerwhite.png");
	Image2D_load(ipreblack, "resource/image/enemy/boss/prelazerblack.png");

	Object_add_component(prering, iprering);
	Object_add_component(prewhite, iprewhite);
	Object_add_component(preblack, ipreblack);

	Object_add_component(sphinx.head, prering);
	Object_add_component(sphinx.head, prewhite);
	Object_add_component(sphinx.head, preblack);

	prering->transform->position.x = -89.5/2-3;
	prering->transform->position.y = -110.5/2+3;
	prering->transform->rotation.z = 1;
	prewhite->transform->position.x = -89.5/2-3;
	prewhite->transform->position.y = -110.5/2+3;
	prewhite->transform->rotation.z = 1;
	preblack->transform->position.x = -89.5/2-3;
	preblack->transform->position.y = -110.5/2+3;
	preblack->transform->rotation.z = 1;

	Object_set_invalid(preblack);
	Object_set_invalid(prewhite);
	Object_set_invalid(prering);

	////////////////////////////////////// pre lazer0;
	
	// 0
	Animation2D_add_frame(bossPreLazer0);
	Animation2D_set_frame_length(bossPreLazer0, 0, 0.0);
	v.x = 0.0;
	v.y = 0.0;
	Animation2D_add_animated_variable(bossPreLazer0, 0, &preblack->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	Animation2D_add_animated_variable(bossPreLazer0, 0, &prewhite->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	v.x = 3.0;
	v.y = 3.0;
	Animation2D_add_animated_variable(bossPreLazer0, 0, &prering->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	neww = 0;
	Animation2D_add_animated_variable(bossPreLazer0, 0, &prering->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 0, &prewhite->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 0, &preblack->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// 1
	Animation2D_add_frame(bossPreLazer0);
	Animation2D_set_frame_length(bossPreLazer0, 1, 0.2);
	v.x = 0.0;
	v.y = 0.0;
	Animation2D_add_animated_variable(bossPreLazer0, 1, &preblack->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	Animation2D_add_animated_variable(bossPreLazer0, 1, &prewhite->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	v.x = 0.5;
	v.y = 0.5;
	Animation2D_add_animated_variable(bossPreLazer0, 1, &prering->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	neww += 20000;
	Animation2D_add_animated_variable(bossPreLazer0, 1, &prering->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 1, &prewhite->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 1, &preblack->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// 2
	Animation2D_add_frame(bossPreLazer0);
	Animation2D_set_frame_length(bossPreLazer0, 2, 0.04);
	v.x = 0.0;
	v.y = 0.0;
	Animation2D_add_animated_variable(bossPreLazer0, 2, &prewhite->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	Animation2D_add_animated_variable(bossPreLazer0, 2, &prering->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	v.x = 0.5;
	v.y = 0.5;
	Animation2D_add_animated_variable(bossPreLazer0, 2, &preblack->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	neww += 20000;
	Animation2D_add_animated_variable(bossPreLazer0, 2, &prering->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 2, &prewhite->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 2, &preblack->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// 3
	Animation2D_add_frame(bossPreLazer0);
	Animation2D_set_frame_length(bossPreLazer0, 3, 0.07);
	v.x = 0.0;
	v.y = 0.0;
	Animation2D_add_animated_variable(bossPreLazer0, 3, &prering->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	v.x = 1.0;
	v.y = 1.0;
	Animation2D_add_animated_variable(bossPreLazer0, 3, &prewhite->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	Animation2D_add_animated_variable(bossPreLazer0, 3, &preblack->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	neww += 20000;
	Animation2D_add_animated_variable(bossPreLazer0, 3, &prering->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 3, &prewhite->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 3, &preblack->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// 5 bye
	Animation2D_add_frame(bossPreLazer0);
	Animation2D_set_frame_length(bossPreLazer0, 4, 0);
	v.x = 0.0;
	v.y = 0.0;
	Animation2D_add_animated_variable(bossPreLazer0, 4, &prering->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	Animation2D_add_animated_variable(bossPreLazer0, 4, &prewhite->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	Animation2D_add_animated_variable(bossPreLazer0, 4, &preblack->transform->scale, &v, sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	neww += 20000;
	Animation2D_add_animated_variable(bossPreLazer0, 4, &prering->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 4, &prewhite->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	Animation2D_add_animated_variable(bossPreLazer0, 4, &preblack->transform->rotation.w, &v, sizeof(Vector2D), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	/////////////////////////////////// jaw
	/////////////////////////////////// jaw
	// frame 0 init
	Animation2D_add_frame(bossJaw);
	Animation2D_set_frame_length(bossJaw, 0, 0.0);
	neww = 0;
	Animation2D_add_animated_variable(bossJaw, 0, &(sphinx.jaw->transform->rotation.w), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	Animation2D_add_frame(bossJaw);
	Animation2D_set_frame_length(bossJaw, 1, 0.5);
	neww = -40;
	Animation2D_add_animated_variable(bossJaw, 1, &(sphinx.jaw->transform->rotation.w), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	Animation2D_add_frame(bossJaw);
	Animation2D_set_frame_length(bossJaw, 2, 0.4);
	neww = -40;
	Animation2D_add_animated_variable(bossJaw, 2, &(sphinx.jaw->transform->rotation.w), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	Animation2D_add_frame(bossJaw);
	Animation2D_set_frame_length(bossJaw, 3, 0.5);
	neww = 0;
	Animation2D_add_animated_variable(bossJaw, 3, &(sphinx.jaw->transform->rotation.w), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	//////////////////////////////////// head rot
	// frame 0
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 0, 2);
	neww = 0;
	Animation2D_add_animated_variable(bossHead, 0, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	// frame 1
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 1, 2);
	neww = -40;
	Animation2D_add_animated_variable(bossHead, 1, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	// frame 2
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 2, 4);
	neww = 0;
	Animation2D_add_animated_variable(bossHead, 2, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	// frame 3
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 3, 2);
	neww = 20;
	Animation2D_add_animated_variable(bossHead, 3, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	// frame 4
	Animation2D_add_frame(bossHead);
	Animation2D_set_frame_length(bossHead, 4, 2);
	neww = 0;
	Animation2D_add_animated_variable(bossHead, 4, &(sphinx.head->transform->rotation.w), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
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
	v.x = sphinx.headp->transform->position.x - 29 ;
	v.y = sphinx.headp->transform->position.y + 9;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.headp->transform->position), &(v), sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);
	v.x = 1.9;
	v.y = 1.9;
	Animation2D_add_animated_variable(bossStandUpAnim, 1, &(sphinx.childObject->transform->scale), &(v), sizeof(Vector2D), ANIMATION_EASEINOUT_SMOOTHING_FOR_VECTOR2D);

	////////////////////////////////////// sweep
	// frame 0 init
	Animation2D_add_frame(bossSweep);
	Animation2D_set_frame_length(bossSweep, 0, 0.3);
	neww = 50;
	Animation2D_add_animated_variable(bossSweep, 0, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 70;
	Animation2D_add_animated_variable(bossSweep, 0, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossSweep, 0, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossSweep, 0, &(sphinx.uarm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1/neww;
	Animation2D_add_animated_variable(bossSweep, 0, &(sphinx.larm->transform->scale.y), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 1 ready
	Animation2D_add_frame(bossSweep);
	Animation2D_set_frame_length(bossSweep, 1, 0.9);
	neww = 10;
	Animation2D_add_animated_variable(bossSweep, 1, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 140;
	Animation2D_add_animated_variable(bossSweep, 1, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.4;
	Animation2D_add_animated_variable(bossSweep, 1, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.6;
	Animation2D_add_animated_variable(bossSweep, 1, &(sphinx.uarm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1/neww;
	Animation2D_add_animated_variable(bossSweep, 1, &(sphinx.larm->transform->scale.y), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 2 sweep
	Animation2D_add_frame(bossSweep);
	Animation2D_set_frame_length(bossSweep, 2, 0.7);
	neww = -245;
	Animation2D_add_animated_variable(bossSweep, 2, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 271;
	Animation2D_add_animated_variable(bossSweep, 2, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.7;
	Animation2D_add_animated_variable(bossSweep, 2, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 8.0;
	Animation2D_add_animated_variable(bossSweep, 2, &(sphinx.uarm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1/neww;
	Animation2D_add_animated_variable(bossSweep, 2, &(sphinx.larm->transform->scale.y), &(neww), sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	// frame 3 end
	Animation2D_add_frame(bossSweep);
	Animation2D_set_frame_length(bossSweep, 3, 0.6);
	neww = 50;
	Animation2D_add_animated_variable(bossSweep, 3, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 70;
	Animation2D_add_animated_variable(bossSweep, 3, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossSweep, 3, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossSweep, 3, &(sphinx.uarm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1/neww;
	Animation2D_add_animated_variable(bossSweep, 3, &(sphinx.larm->transform->scale.y), &(neww), sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 1 ready

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
	Animation2D_set_frame_length(bossPunch, 1, 1.2);
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
	Animation2D_set_frame_length(bossPunch, 3, 0.04);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 3, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 3, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 3, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 4 ready
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 4, 0.04);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 4, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 4, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 4, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	// frame 5 punch 2
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 5, 0.1);
	neww = 0;
	Animation2D_add_animated_variable(bossPunch, 5, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 90;
	Animation2D_add_animated_variable(bossPunch, 5, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 3.5;
	Animation2D_add_animated_variable(bossPunch, 5, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 6 punch 2 end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 6, 0.01);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 6, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 6, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 6, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 7 ready
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 7, 0.01);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 7, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 7, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 7, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	// frame 8 punch 3
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 8, 0.06);
	neww = 32;
	Animation2D_add_animated_variable(bossPunch, 8, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 90;
	Animation2D_add_animated_variable(bossPunch, 8, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 3.3;
	Animation2D_add_animated_variable(bossPunch, 8, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 9 punch 3 end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 9, 0.01);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 9, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 9, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 9, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 10 ready
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 10, 0.01);
	neww = -40;
	Animation2D_add_animated_variable(bossPunch, 10, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 10, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 10, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);

	// frame 11 punch 4
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 11, 0.07);
	neww = 38;
	Animation2D_add_animated_variable(bossPunch, 11, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 90;
	Animation2D_add_animated_variable(bossPunch, 11, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 3.3;
	Animation2D_add_animated_variable(bossPunch, 11, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// frame 12 punch 4 end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 12, 0.1);
	neww = -38;
	Animation2D_add_animated_variable(bossPunch, 12, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 20;
	Animation2D_add_animated_variable(bossPunch, 12, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 12, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_LINER_SMOOTHING_FOR_DOUBLE);

	// end
	Animation2D_add_frame(bossPunch);
	Animation2D_set_frame_length(bossPunch, 13, 0.3);
	neww = 50;
	Animation2D_add_animated_variable(bossPunch, 13, &(sphinx.uarm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 70;
	Animation2D_add_animated_variable(bossPunch, 13, &(sphinx.larm->transform->rotation.w), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);
	neww = 1.0;
	Animation2D_add_animated_variable(bossPunch, 13, &(sphinx.larm->transform->scale.x), &neww, sizeof(double), ANIMATION_EASEINOUT_SMOOTHING_FOR_DOUBLE);


	////////////////////////////////////////////// reset
	Animation2D_add_frame(bossReset);
	Animation2D_set_frame_length(bossReset, 0, 0);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.uarm->transform->rotation), &(sphinx.uarm->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.larm->transform->rotation), &(sphinx.larm->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.uleg->transform->rotation), &(sphinx.uleg->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.lleg->transform->rotation), &(sphinx.lleg->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.body->transform->rotation), &(sphinx.body->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.headp->transform->rotation), &(sphinx.headp->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.head->transform->rotation), &(sphinx.head->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.jaw->transform->rotation), &(sphinx.jaw->transform->rotation), sizeof(Vector4D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.childObject->transform->scale), &(sphinx.childObject->transform->scale), sizeof(Vector2D), ANIMATION_NO_SMOOTHING);
	Animation2D_add_animated_variable(bossReset, 0, &(sphinx.headp->transform->position), &(sphinx.headp->transform->position), sizeof(Vector2D), ANIMATION_NO_SMOOTHING);

}
// }}}

// sphinx update
// {{{
void moveSphinx()
{
	static bool hp10 = false;
	static bool hp20 = false;
	static bool punch = false;
	static bool sweep = false;
	static bool lazer = false;
	static bool child = false;
	static bool go = false;

	Vector2D v;
	if(sphinx.childObject->transform->position.x < 850 && hp20 == false)
	{
		if(sphinxNow == false)
		{
			sphinxNow = true;	
			setSphinxColValid();
		}
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
				//sphinx.hp1 = SPHINX_DEFAULT_HP1;
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
		else if(lazer == true)
		{
			double cnt;
			double prelen;
			double len;
			Timer_get_count(sphinx.timer, &cnt);
			Animation2D_get_length(bossPreLazer0, &prelen);
			Animation2D_get_length(bossHead, &len);
			if(cnt < prelen)
			{
				Animation2D_play(bossPreLazer0);
			}
			else if(cnt < len+prelen)
			{
				Object_set_invalid(preblack);
				Object_set_invalid(prewhite);
				Object_set_invalid(prering);
				Animation2D_play(bossHead);
				double cnt2;
				Timer_get_count(sphinx.lazerTimer, &cnt2);
				if(cnt2 > sphinxLazerInterval) 
				{
					Timer_reset_count(sphinx.lazerTimer);
					bool isValid;
					Object_is_valid(sphinx.lazer, &isValid);
					
					if(isValid == true)
					{	
						sphinx.lzcol->isValid = false;
						Object_set_invalid(sphinx.lazer);
					}
					else
					{
						sphinx.lzcol->isValid = true;
						Object_set_valid(sphinx.lazer);
					}
				}
			}
			else
			{
				lazer = false;
				sphinx.lzcol->isValid = false;
				Object_set_invalid(sphinx.lazer);
			}
		}
		else if(child == true)
		{
			double cnt;
			double len;
			Timer_get_count(sphinx.timer, &cnt);
			Animation2D_get_length(bossJaw, &len);
			if(cnt <= len)
			{
				Animation2D_play(bossJaw);
				if(cnt > 1.0 && go == true)
				{
					Vector2D v2;
					v2.x = 784;
					v2.y = 196;
					addEnemyCharacter1(&v2, 10, 16, 16+currentLap);
					go = false;
				}
			}
			else
			{
				child = false;
			}
		}
		else if(sweep == true)
		{
			double cnt;
			double len;
			Timer_get_count(sphinx.timer, &cnt);
			Animation2D_get_length(bossSweep, &len);
			if(cnt <= len)
			{
				Animation2D_play(bossSweep);
			}
			else
			{
				sweep = false;
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

			if(rand()%sphinxShotChance == 0)
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
			//if(sphinx.blcol->hits[0] != NULL) flag2 = true;

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
			if(sweep == false && punch == false && lazer == false && child == false)
			{
				switch(rand() % 4)
				{
					case 0:
						Audio_play(sphinx.aPunch);
						Animation2D_reset(bossPunch);
						Timer_reset_count(sphinx.timer);
						punch = true;
						break;
					case 1:
						Audio_play(sphinx.aLazer);
						Animation2D_reset(bossHead);
						Object_set_valid(preblack);
						Object_set_valid(prewhite);
						Object_set_valid(prering);
						Animation2D_reset(bossPreLazer0);
						Timer_reset_count(sphinx.timer);
						lazer = true;
						break;
					
					case 2:
						Audio_play(sphinx.aGass);
						Animation2D_reset(bossJaw);
						Timer_reset_count(sphinx.timer);
						child = true;
						go = true;
						break;
					case 3:
						Audio_play(sphinx.aBump);
						Animation2D_reset(bossSweep);
						Timer_reset_count(sphinx.timer);
						sweep = true;
						break;
				}
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
			//if(sphinx.blcol->hits[0] != NULL) flag2 = true;

			if(flag2 == true)
			{
				sphinx.hp2 -= 10;
				DP("sphinx.hp2 : %f!!!!!!\n", sphinx.hp2);
				if(sphinx.hp2 <= 0)
				{
					Timer_reset_count(sphinx.timer);
					hp20 = true;
					incScore(sphinxScore);
				}
			}
		}
	}
	else
	{
		double spf;
		Timer_get_spf(sphinx.timer, &spf);
		sphinx.childObject->transform->position.x -= spf * stage0_moving_speed;
			child = false;
			sweep = false;
			punch = false;
			lazer = false;
			Object_set_invalid(sphinx.lazer);
			//Object_set_invalid(sphinx.masterObject);
			sphinxNow = false;
			//sphinx.hp2 = SPHINX_DEFAULT_HP2;
		if(sphinx.hp2 > 0)
		{
			setSphinxColInvalid();
			hp20 = false;
			hp10 = false;
		}
		if(sphinx.hp2 > 0)
		{
			if(sphinx.hp1 > 0)
			{
				if(sphinx.childObject->transform->position.x > 2000)
				{
					Animation2D_play(bossReset);
				}
			}
		}
	}

}
// }}}

// sphinx col
// {{{
void setSphinxColInvalid()
{
	sphinx.bdcol->isValid = false;
	sphinx.ulcol->isValid = false;
	sphinx.llcol->isValid = false;
	sphinx.uacol->isValid = false;
	sphinx.lacol->isValid = false;
	sphinx.hdcol->isValid = false;
	sphinx.jwcol->isValid = false;
	sphinx.eycol->isValid = false;
	sphinx.blcol->isValid = false;
	sphinx.lzcol->isValid = false;
}

void setSphinxColValid()
{
	sphinx.bdcol->isValid = true;
	sphinx.ulcol->isValid = true;
	sphinx.llcol->isValid = true;
	sphinx.uacol->isValid = true;
	sphinx.lacol->isValid = true;
	sphinx.hdcol->isValid = true;
	sphinx.jwcol->isValid = true;
	sphinx.eycol->isValid = true;
	sphinx.blcol->isValid = true;
}
// }}}

// init enc0 enb0
// {{{
void initEnemyCharacter0()
{
	enemyCharacter0.masterObject = Object_new();
	enemyCharacter0.image0 = Image2D_new();
	enemyCharacter0.masterTimer = Timer_new();
	enemyCharacter0.speed = ENEMY_CHARACTER0_DEFAULT_SPEED;
	enemyCharacter0.acceleration = ENEMY_CHARACTER0_DEFAULT_ACCELERATION;
	enemyCharacter0.interval = ENEMY_CHARACTER0_DEFAULT_INTERVAL;

	Image2D_load(enemyCharacter0.image0, "resource/image/enemy/plane/0.png");

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
// }}}

// init enc1 enc2
// {{{
void initEnemyCharacter1()
{
	enemyCharacter1.masterObject = Object_new();
	enemyCharacter1.image0 = Image2D_new();
	enemyCharacter1.masterTimer = Timer_new();
	enemyCharacter1.speed = ENEMY_CHARACTER1_DEFAULT_SPEED;
	enemyCharacter1.acceleration = ENEMY_CHARACTER1_DEFAULT_ACCELERATION;
	enemyCharacter1.interval = ENEMY_CHARACTER1_DEFAULT_INTERVAL;

	Image2D_load(enemyCharacter1.image0, "resource/image/enemy/plane/1.png");

	for(int i = 0; i < ENEMY_CHARACTER1_DEFAULT_NUM; i++)
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
		rc->size.x = 20;
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
	enemyCharacter2.acceleration = ENEMY_CHARACTER2_DEFAULT_ACCELERATION;
	enemyCharacter2.interval = ENEMY_CHARACTER2_DEFAULT_INTERVAL;

	Image2D_load(enemyCharacter2.image0, "resource/image/enemy/vehicle/0t.png");
	Image2D_load(enemyCharacter2.image1, "resource/image/enemy/vehicle/0c.png");

	int h;
	Image2D_get_height(enemyCharacter2.image1, &h);
	int w;
	Image2D_get_width(enemyCharacter2.image1, &w);

	for(int i = 0; i < ENEMY_CHARACTER2_DEFAULT_NUM; i++)
	{
		enemyCharacter2.object[i] = Object_new();
		enemyCharacter2.cannonObject[i] = Object_new();
		enemyCharacter2.tireObject[i] = Object_new();
		enemyCharacter2.collider[i] = Collider2D_new();
		enemyCharacter2.collidert[i] = Collider2D_new();
		enemyCharacter2.timer[i] = Timer_new();
		enemyCharacter2.speed[i] = ENEMY_CHARACTER2_DEFAULT_SPEED;

		enemyCharacter2.object[i]->transform->position.x = -1000;
		enemyCharacter2.object[i]->transform->rotation.z = 1;
		enemyCharacter2.cannonObject[i]->transform->position.x = -3;
		enemyCharacter2.cannonObject[i]->transform->position.y = -h/2.0/2.0-0.5;
		enemyCharacter2.cannonObject[i]->transform->scale.x = 1.3;

		enemyCharacter2.tireObject[i]->transform->position.x = -w/2.0;
		enemyCharacter2.tireObject[i]->transform->position.y = -h/2.0;

		strncpy(enemyCharacter2.collider[i]->tag, "enc2", 4);
		Collider2D_set_collider_object(enemyCharacter2.collider[i], COLLIDER2D_COLLIDER_RECT);
		Collider2D_register_collider(enemyCharacter2.collider[i], 24);
		RectCollider* rc = (RectCollider*)(enemyCharacter2.collider[i]->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = 18;
		rc->size.y = 10;
		strncpy(enemyCharacter2.collidert[i]->tag, "enc2", 4);
		Collider2D_set_collider_object(enemyCharacter2.collidert[i], COLLIDER2D_COLLIDER_RECT);
		Collider2D_register_collider(enemyCharacter2.collidert[i], 24);
		rc = (RectCollider*)(enemyCharacter2.collidert[i]->colObj);
		Vector2D_set_zero(&(rc->position));
		rc->size.x = w;
		rc->size.y = h;
		//enemyCharacter2.enemyBullet0[i] = (EnemyBullet0*)malloc(sizeof(EnemyBullet0));
		//initEnemyBullet0(enemyCharacter2.enemyBullet0[i]);
		Object_add_component(enemyCharacter2.tireObject[i], enemyCharacter2.image0);
		Object_add_component(enemyCharacter2.tireObject[i], enemyCharacter2.collidert[i]);
		Object_add_component(enemyCharacter2.cannonObject[i], enemyCharacter2.image1);
		Object_add_component(enemyCharacter2.cannonObject[i], enemyCharacter2.collider[i]);
		Object_add_component(enemyCharacter2.object[i], enemyCharacter2.cannonObject[i]);
		Object_add_component(enemyCharacter2.object[i], enemyCharacter2.tireObject[i]);
		//Object_add_component(enemyCharacter2.object[i], enemyCharacter2.enemyBullet0[i]->masterObject);
		Object_add_component(enemyCharacter2.masterObject, enemyCharacter2.object[i]);

		Object_set_invalid(enemyCharacter2.object[i]);
		enemyCharacter2.collider[i]->isValid = false;
		enemyCharacter2.collidert[i]->isValid = false;
	}
}
// }}}

// move enc 0 1 2
// {{{
void moveEnemyCharacter0()
{
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
				if(t > enemyCharacter0.interval && rand()%enc0shotChance == 0)
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
							if(rand()%20 < currentLap)
							{
								v.y = rand()/(double)RAND_MAX;
								Vector2D_normalize(&v);
								shotEnemyBullet0(&(enemyCharacter0.object[i]->transform->position), &v);
							}
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

			Vector2D v;
			v.x = pbo->transform->position.x ;
			v.y = pbo->transform->position.y;
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
			if((fabs(v.x - cos(enemyCharacter1.object[i]->transform->rotation.w/180.0*M_PI)) > 0.05 || fabs(v.y - sin(enemyCharacter1.object[i]->transform->rotation.w/180.0*M_PI)) > 0.05))
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
					if(rand()%enc1shotChance == 0)
					{
						if(WINDOW_WIDTH > pbo->transform->position.x && pbo->transform->position.y > 0 && view->window_height+50 > pbo->transform->position.y && pbo->transform->position.x > -50)
						{
							Timer_reset_count(enemyCharacter1.timer[i]);
							Vector2D v2;
							Vector2D_set(&v2, &(enemyCharacter1.object[i]->transform->position));
							shotEnemyBullet0(&v2, &v);
						}
					}
				}
			}
			if(pbo->transform->position.x < -100 || (pbo->transform->position.x < WINDOW_WIDTH &&(view->window_width < pbo->transform->position.x || pbo->transform->position.y < 0 || view->window_height+8 < pbo->transform->position.y)))
			{
				DPIF(false, "bye! enemy1\n");
				Object_set_invalid(pbo);
				enemyCharacter1.collider[i]->isValid = false;
				//pbo->transform->position.x = view->window_width;
				//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

			}
		}
	}
}

void moveEnemyCharacter2()
{
	for(int i = 0; i < ENEMY_CHARACTER2_DEFAULT_NUM; i++)
	{
		Object* pbo = enemyCharacter2.object[i];
		bool isValid;
		Object_is_valid(pbo, &isValid);
		if(isValid == true)
		{
			double spf;
			bool isTerrain = false;
			Timer_get_spf(enemyCharacter2.timer[i], &spf);
			if(enemyCharacter2.collider[i]->hits[0] != NULL)
			{
				for(int ci = 0; ci < COLLIDER2D_HITS_NUM; ci++)
				{
					if(enemyCharacter2.collider[i]->hits[ci] != NULL)
					{
						if(strncmp(enemyCharacter2.collider[i]->hits[ci]->tag, "trrn", COLLIDER2D_TAG_LENGTH) == 0)
						{
							isTerrain = true;
						}
					}
				}
				if(isTerrain == false)
				{
					DPIF(false, "%p is destroyed by %p\n", enemyCharacter2.collider[i], enemyCharacter2.collider[i]->hits[0]);
					Object_set_invalid(pbo);
					enemyCharacter2.collider[i]->isValid = false;
					Vector2D v;
					v.x = pbo->transform->position.x ;
					v.y = pbo->transform->position.y;
					addDestroyEffect(&v);
				}
			}

				pbo->transform->position.x -= spf * stage0_moving_speed;
				enemyCharacter2.object[i]->transform->rotation.w += spf*enemyCharacter2.speed[i];
				// shoot randomly
				double t;
				Timer_get_count(enemyCharacter2.timer[i], &t);
				if(t > enemyCharacter2.interval && rand()%enc2shotChance == 0)
				{
					if(WINDOW_WIDTH > pbo->transform->position.x && pbo->transform->position.y > 0 && view->window_height+50 > pbo->transform->position.y && pbo->transform->position.x > -50)
					{
						Timer_reset_count(enemyCharacter2.timer[i]);
						Vector2D v;
						Vector2D v2;
						v.x = cos(enemyCharacter2.object[i]->transform->rotation.w/180.0*M_PI);
						v.y = sin(enemyCharacter2.object[i]->transform->rotation.w/180.0*M_PI);
						Vector2D_normalize(&v);
						Vector2D_set(&v2, &enemyCharacter2.object[i]->transform->position);
						Vector2D_add(&v2, &enemyCharacter2.tireObject[i]->transform->position);
						shotEnemyBullet0(&(enemyCharacter2.object[i]->transform->position), &v);
						for(int a = 0; a < 10; a++)
						{
							if(rand()%(30+a) < currentLap)
							{
								v.x += rand()/(double)RAND_MAX - 0.5;
								v.y += rand()/(double)RAND_MAX - 0.5;
								Vector2D_normalize(&v);
								shotEnemyBullet0(&(enemyCharacter2.object[i]->transform->position), &v);
							}
						}
					}
				}
		
			if(pbo->transform->position.x < -100 || (pbo->transform->position.x < WINDOW_WIDTH &&(view->window_width < pbo->transform->position.x || pbo->transform->position.y < 0 || view->window_height+8 < pbo->transform->position.y)))
			{
				Object_set_invalid(pbo);
				enemyCharacter2.collider[i]->isValid = false;
				//pbo->transform->position.x = view->window_width;
				//pbo->transform->position.y = playerCharacter.object->transform->position.y + 8;

			}
		}
	}
}
// }}}

// move enb 0
// {{{
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


			if(pbo->transform->position.x < -100)
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
// }}}

// add enc 0 1 2
// {{{
void addEnemyCharacter0(Vector2D* const pos, const int start, const int e)
{
	int end = e;
	if(end >= ENEMY_CHARACTER0_DEFAULT_NUM)
	{
		end = ENEMY_CHARACTER0_DEFAULT_NUM-1;
	}
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
			DEIF(true, "couldnt add an enemy %d\n", i);
			//break;
		}
	}
}
void addEnemyCharacter1(Vector2D* const pos, const double offset, const int start, const int e)
{
	int end = e;
	if(end >= ENEMY_CHARACTER1_DEFAULT_NUM)
	{
		end = ENEMY_CHARACTER1_DEFAULT_NUM-1;
	}
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
			DEIF(true, "couldnt add an enemy1 %d\n", i);
			//break;
		}
	}
}

void addEnemyCharacter2(Vector2D* const pos)
{
	for(int i = 0; i < ENEMY_CHARACTER2_DEFAULT_NUM; i++)
	{
		bool isValid;
		Object_is_valid(enemyCharacter2.object[i], &isValid);
		if(isValid == false)
		{
			DPIF(false, "i:%d\n", i);
			Object_set_valid(enemyCharacter2.object[i]);
			enemyCharacter2.collider[i]->isValid = true;
			Vector2D_set(&(enemyCharacter2.object[i]->transform->position), pos);
			Timer_set_count(enemyCharacter2.timer[i], enemyCharacter2.interval);
			break;
		}
		else
		{
			DEIF(true, "couldnt add an enemy2 %d\n", i);
			//break;
		}
	}
}
// }}}

// add enb 0
// {{{
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
// }}}

// update timer
// {{{
void updateTimer()
{
	Timer_count(playerCharacter.timer);
	Timer_count(playerBullet0.timer);

	Timer_count(enemyCharacter0.masterTimer);
	Timer_count(enemyBullet0.masterTimer);

	Timer_count(enemyCharacter1.masterTimer);
	Timer_count(enemyCharacter2.masterTimer);

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

	for(int i = 0; i < ENEMY_CHARACTER2_DEFAULT_NUM; i++)
	{
		Timer_count(enemyCharacter2.timer[i]);
	}

	for(int i = 0; i < DESTROY_EFFECT_NUM; i++)
	{
		Timer_count(destroyEffect0.timer[i]);
	}

	Timer_count(backTimer);
	Timer_count(stage0Timer);
	Timer_count(sphinx.timer);
	Timer_count(sphinx.lazerTimer);
}
// }}}

// powerup core
// {{{
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


		strncpy(pbc->tag, "pucr", COLLIDER2D_TAG_LENGTH);
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

void addPowerupCore(Vector2D* const pos)
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
			incScore(powerupCoreScore);
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
// }}}

// back ground
// {{{
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
	if(sphinxNow == true)
	{
		return;
	}

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
// }}}

// score
// {{{
void updateScoreObj()
{
	
	static Image2D* inum[10];
	static Image2D* iscore;

	static Image2D* idouble;
	static Image2D* ibulspeed;
	static Image2D* ibulnum;
	static Image2D* ibulscale;
	static Image2D* ispeed;
	static Image2D* iselecter;

	static Image2D* ihpframe;
	static Image2D* ihpbar;

	static bool isInitialized = false;

	static Object* digiObj[SCORE_STRING_LENGTH];
	static Object* scoreObjChild;
	static Object* powerupObj;
	static Object* selecterObj;
	static Object* hpObj;

	if(isInitialized == false)
	{
		isInitialized = true;
		char path[64];

		scoreObject = Object_new();
		scoreObjChild = Object_new();
		powerupObj = Object_new();
		selecterObj = Object_new();
		hpObj = Object_new();

		for(int i = 0; i < SCORE_STRING_LENGTH; i++)
		{
			digiObj[i] = Object_new();
			digiObj[i]->transform->position.x = 216 + i*33;
			Object_add_component(scoreObjChild, digiObj[i]);
		}

		for(int i = 0; i < 10; i++)
		{
			snprintf(path, 64, "resource/image/text/number/%d.png", i);
			inum[i] = Image2D_new();
			Image2D_load(inum[i], path);
			Object_add_component(digiObj[i], inum[i]);
		}


		iscore = Image2D_new();
		idouble = Image2D_new();
		ibulnum = Image2D_new();
		ibulspeed = Image2D_new();
		ibulscale = Image2D_new();
		ispeed = Image2D_new();
		iselecter = Image2D_new();

		ihpframe = Image2D_new();
		ihpbar = Image2D_new();

		ibulnum = Image2D_new();
		ibulspeed = Image2D_new();
		ispeed = Image2D_new();
		iselecter = Image2D_new();

		// speed bspeed bnum double
		idouble->p_transform->scale.x = 0.4;
		idouble->p_transform->scale.y = 0.4;
		idouble->p_transform->position.x = 60*3;
		idouble->p_transform->position.y = WINDOW_HEIGHT-60;

		ibulnum->p_transform->scale.x = 0.4;
		ibulnum->p_transform->scale.y = 0.4;
		ibulnum->p_transform->position.x = 60*4;
		ibulnum->p_transform->position.y = WINDOW_HEIGHT-60;

		ibulspeed->p_transform->scale.x = 0.4;
		ibulspeed->p_transform->scale.y = 0.4;
		ibulspeed->p_transform->position.x = 60*1;
		ibulspeed->p_transform->position.y = WINDOW_HEIGHT-60;

		ibulscale->p_transform->scale.x = 0.4;
		ibulscale->p_transform->scale.y = 0.4;
		ibulscale->p_transform->position.x = 60*2;
		ibulscale->p_transform->position.y = WINDOW_HEIGHT-60;

		ispeed->p_transform->scale.x = 0.4;
		ispeed->p_transform->scale.y = 0.4;
		ispeed->p_transform->position.x = 60*0;
		ispeed->p_transform->position.y = WINDOW_HEIGHT-60;

		iselecter->p_transform->scale.x = 0.4;
		iselecter->p_transform->scale.y = 0.4;
		iselecter->p_transform->position.x = -60.0;
		iselecter->p_transform->position.y = WINDOW_HEIGHT-60;

		iscore->p_transform->position.y = 6;

		Image2D_load(iscore, "resource/image/text/score.png");
		Image2D_load(idouble, "resource/image/text/double.png");
		Image2D_load(ibulspeed, "resource/image/text/shotspeed.png");
		Image2D_load(ibulnum, "resource/image/text/shotnum.png");
		Image2D_load(ibulscale, "resource/image/text/bscale.png");
		Image2D_load(ispeed, "resource/image/text/speed.png");
		Image2D_load(iselecter, "resource/image/text/selecter.png");

		Image2D_load(ihpframe, "resource/image/text/hpframe.png");
		Image2D_load(ihpbar, "resource/image/text/hpbar.png");

		Object_add_component(powerupObj, idouble);
		Object_add_component(powerupObj, ibulspeed);
		Object_add_component(powerupObj, ibulscale);
		Object_add_component(powerupObj, ibulnum);
		Object_add_component(powerupObj, ispeed);
		Object_add_component(powerupObj, selecterObj);
		Object_add_component(hpObj, ihpframe);
		Object_add_component(hpObj, ihpbar);
		Object_add_component(selecterObj, iselecter);
		Object_add_component(scoreObject, hpObj);
		Object_add_component(scoreObject, powerupObj);

		Object_add_component(scoreObjChild, iscore);
		Object_add_component(scoreObject, scoreObjChild);

		scoreObjChild->transform->scale.x = 0.5;
		scoreObjChild->transform->scale.y = 0.5;

		hpObj->transform->scale.x = 4.48;
		hpObj->transform->scale.y = 1;
		hpObj->transform->position.x = 5;
		hpObj->transform->position.y = 30;

		// dfx
		dfxObj = Object_new();
		damageEffect = Image2D_new();
		Image2D_load(damageEffect, "resource/image/text/damageeffect.png");
		Object_add_component(dfxObj, damageEffect);
		Object_set_invalid(dfxObj);
		Object_add_component(scoreObject, dfxObj);
	}

	int d;
	int dd;
	for(int i = 0; i < SCORE_STRING_LENGTH; i++)
	{
		d = pow(10, i);
		dd = pow(10, i+1);
		Object_clear_component(digiObj[SCORE_STRING_LENGTH-1-i]);
		Object_add_component(digiObj[SCORE_STRING_LENGTH-1-i], inum[(score%dd - score%d)/d]);
	}

	hpObj->transform->scale.x = playerCharacter.hp/PLAYER_CHARACTER_DEFAULT_HP * 4.48;
	selecterObj->transform->position.x = powerupSelecter*60;

}

void incScore(unsigned long long scr)
{
	score += scr * (currentLap * currentLap)/2.0 * playerCharacter.hp/PLAYER_CHARACTER_DEFAULT_HP;
}
// }}}

// destroy fx
// {{{
void initDestroyEffect()
{

	DestroyEffect* eb = &destroyEffect0;

	eb->masterObject = Object_new();
	eb->image0 = Image2D_new();
	eb->masterTimer = Timer_new();
	eb->audioDestroy = Audio_new(DESTROY_EFFECT_NUM);
	eb->fadeTime = DESTROY_EFFECT_FADETIME;
	
	Image2D_load(eb->image0, "resource/image/enemy/plane/destroy.png");
	Audio_load(eb->audioDestroy, "resource/audio/enemy/plane/destroy/0.wav");
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
			Audio_play(destroyEffect0.audioDestroy);
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
// }}}

    // // // // // // // // // // // // // // // // // // // // // // // // // // //
   // // // // // // // // // // // // // // // // // // // // // // // // // // // 
  // // // // // // // // // //  S  T  A  G  E  0 // // // // // // // // // // //
 // // // // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // // // // 

// stage 0 init
// {{{

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

Object* pyramidObj0;
Object* pyramidObj1;
Object* pyramidObj2;
Object* pyramidObj3;
Object* pyramidObj4;
Object* groundObj0;

double stage0ground0length;
double stage0ground0time = 60;

void stage0_init()
{
	Image2D* pyramidImg;
	Image2D* groundImg;
	Collider2D* pyramidCol;
	Collider2D* pyramidCol1;
	Collider2D* pyramidCol2;
	Collider2D* pyramidCol3;
	Collider2D* pyramidCol4;
	Collider2D* groundCol;

        pyramidObj0 = Object_new();
        pyramidObj1 = Object_new();
        pyramidObj2 = Object_new();
        pyramidObj3 = Object_new();
        pyramidObj4 = Object_new();

	groundObj0 = Object_new();
	pyramidImg = Image2D_new();
	groundImg = Image2D_new();
	pyramidCol = Collider2D_new();
	pyramidCol1 = Collider2D_new();
	pyramidCol2 = Collider2D_new();
	pyramidCol3 = Collider2D_new();
	pyramidCol4 = Collider2D_new();
	groundCol= Collider2D_new();

	stage0 = Object_new();
	Object_set_invalid(stage0);

	Object_add_component(pyramidObj0, pyramidImg);
	Object_add_component(pyramidObj0, pyramidCol);

	Object_add_component(pyramidObj1, pyramidImg);
	Object_add_component(pyramidObj1, pyramidCol1);

	Object_add_component(pyramidObj2, pyramidImg);
	Object_add_component(pyramidObj2, pyramidCol2);

	Object_add_component(pyramidObj3, pyramidImg);
	Object_add_component(pyramidObj3, pyramidCol3);

	Object_add_component(pyramidObj4, pyramidImg);
	Object_add_component(pyramidObj4, pyramidCol4);

	Object_add_component(groundObj0, groundImg);
	Object_add_component(groundObj0, groundCol);

	Object_add_component(stage0, pyramidObj0);
	Object_add_component(stage0, pyramidObj1);
	Object_add_component(stage0, pyramidObj2);
	Object_add_component(stage0, pyramidObj3);
	Object_add_component(stage0, pyramidObj4);
	Object_add_component(stage0, groundObj0);

	Image2D_load(groundImg, "resource/image/stage/stage0/ground.png");
	Image2D_load(pyramidImg, "resource/image/stage/stage0/pyramid.png");

	Collider2D_set_collider_object(groundCol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(pyramidCol, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(pyramidCol1, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(pyramidCol2, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(pyramidCol3, COLLIDER2D_COLLIDER_RECT);
	Collider2D_set_collider_object(pyramidCol4, COLLIDER2D_COLLIDER_RECT);


	RectCollider* grc = (RectCollider*)(groundCol->colObj);
	RectCollider* prc = (RectCollider*)(pyramidCol->colObj);
	RectCollider* prc1 = (RectCollider*)(pyramidCol1->colObj);
	RectCollider* prc2 = (RectCollider*)(pyramidCol2->colObj);
	RectCollider* prc3 = (RectCollider*)(pyramidCol3->colObj);
	RectCollider* prc4 = (RectCollider*)(pyramidCol4->colObj);

	Vector2D_set_zero(&(grc->position));
	Vector2D_set_zero(&(prc->position));
	Vector2D_set_zero(&(prc1->position));
	Vector2D_set_zero(&(prc2->position));
	Vector2D_set_zero(&(prc3->position));
	Vector2D_set_zero(&(prc4->position));

	grc->size.x = 60;
	grc->size.y = 60;

	prc->size.x = 60;
	prc->size.y = 60;

	prc1->size.x = 60;
	prc1->size.y = 60;

	prc2->size.x = 60;
	prc2->size.y = 60;

	prc3->size.x = 60;
	prc3->size.y = 60;

	prc4->size.x = 60;
	prc4->size.y = 60;

	int w;
	stage0ground0Offset = 5000;
	stage0ground0length = stage0_moving_speed * stage0ground0time;
	Image2D_get_width(groundImg, &w);

	groundObj0->transform->position.x = stage0ground0Offset;
	groundObj0->transform->position.y = WINDOW_HEIGHT-50;
	groundObj0->transform->scale.x = stage0ground0length/ (double)w;
	groundObj0->transform->scale.y = 10;

	pyramidObj0->transform->scale.x = 4;
	pyramidObj0->transform->scale.y = 4;
	pyramidObj0->transform->position.x =6000;
	pyramidObj0->transform->position.y = WINDOW_HEIGHT-50-30*sqrt(pow(pyramidObj0->transform->scale.x,2) + pow(pyramidObj0->transform->scale.y,2));
	pyramidObj0->transform->rotation.z = 1;
	pyramidObj0->transform->rotation.w = 45;

	pyramidObj1->transform->scale.x = 3;
	pyramidObj1->transform->scale.y = 3;
	pyramidObj1->transform->position.x =6200;
	pyramidObj1->transform->position.y = WINDOW_HEIGHT-50-30*sqrt(pow(pyramidObj1->transform->scale.x,2) + pow(pyramidObj1->transform->scale.y,2));
	pyramidObj1->transform->rotation.z = 1;
	pyramidObj1->transform->rotation.w = 45;

	pyramidObj2->transform->scale.x = 13;
	pyramidObj2->transform->scale.y = 13;
	pyramidObj2->transform->position.x =7200;
	pyramidObj2->transform->position.y = WINDOW_HEIGHT-50-30*sqrt(pow(pyramidObj2->transform->scale.x,2) + pow(pyramidObj2->transform->scale.y,2));
	pyramidObj2->transform->rotation.z = 1;
	pyramidObj2->transform->rotation.w = 45;

	pyramidObj3->transform->scale.x = 6;
	pyramidObj3->transform->scale.y = 6;
	pyramidObj3->transform->position.x =7600;
	pyramidObj3->transform->position.y = WINDOW_HEIGHT-50-30*sqrt(pow(pyramidObj3->transform->scale.x,2) + pow(pyramidObj3->transform->scale.y,2));
	pyramidObj3->transform->rotation.z = 1;
	pyramidObj3->transform->rotation.w = 45;

	pyramidObj4->transform->scale.x = 9;
	pyramidObj4->transform->scale.y = 9;
	pyramidObj4->transform->position.x =9000;
	pyramidObj4->transform->position.y = WINDOW_HEIGHT-50-30*sqrt(pow(pyramidObj4->transform->scale.x,2) + pow(pyramidObj4->transform->scale.y,2));
	pyramidObj4->transform->rotation.z = 1;
	pyramidObj4->transform->rotation.w = 45;

	pyramidObj0->transform->rotation.z = 1;
	pyramidObj0->transform->rotation.w = 45;


	


	Collider2D_register_collider(pyramidCol, 20);
	Collider2D_register_collider(pyramidCol1, 20);
	Collider2D_register_collider(pyramidCol2, 20);
	Collider2D_register_collider(pyramidCol3, 20);
	Collider2D_register_collider(pyramidCol4, 20);
	Collider2D_register_collider(groundCol, 20);

	memcpy(pyramidCol->tag, "trrn", COLLIDER2D_TAG_LENGTH);
	memcpy(pyramidCol1->tag, "trrn", COLLIDER2D_TAG_LENGTH);
	memcpy(pyramidCol2->tag, "trrn", COLLIDER2D_TAG_LENGTH);
	memcpy(pyramidCol3->tag, "trrn", COLLIDER2D_TAG_LENGTH);
	memcpy(pyramidCol4->tag, "trrn", COLLIDER2D_TAG_LENGTH);
	memcpy(groundCol->tag, "trrn", COLLIDER2D_TAG_LENGTH);

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
// }}}

// stage 0 update
// {{{
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
	const double enemyFollow0length = 5.0;
	static bool enemyFollow0 = false;

	const double enemyFollow1start = 13.0;
	const double enemyFollow1length = 5.0;
	static bool enemyFollow1 = false;

	const double enemyWave2start = 20.0;
	const double enemyWave2length = 12.8;
	static bool enemyWave2 = false;

	const double enemyWave3start = 25.0;
	const double enemyWave3length = 12.8;
	static bool enemyWave3 = false;

	const double enemyFollow2start = 30.0;
	const double enemyFollow2length = 5.0;
	static bool enemyFollow2 = false;

	const double enemyWave4start = 35.0;
	const double enemyWave4length = 12.0;
	static bool enemyWave4 = false;

	const double enemyFixed0start= 43.6;
	const double enemyFixed0length = 25.0;
	static bool enemyFixed0 = false;

	const double enemyFollow3start = 55.5;
	const double enemyFollow3length = 10.0;
	static bool enemyFollow3 = false;

	Vector2D v;
	v.x = WINDOW_WIDTH + 200;
	v.y = WINDOW_HEIGHT / 2;

	if(Keyboard_is_pressed('h'))
		addPowerupCore(&v);


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
	if(sphinxNow == false)
	{
		stage0->transform->position.x -= stage0_moving_speed*spf;
	}

	if(st > stage0appear && stage0v == false){Object_set_valid(stage0); stage0v = true;}

	// follow 2
	v.y = 0;
	if(enemyFollow2 == false && st > enemyFollow2start && st < enemyFollow2start + enemyFollow2length) {addEnemyCharacter1(&v, 50, 4, 6); enemyFollow2 = true;}		
	if(st > enemyFollow2start + enemyFollow2length){enemyFollow2 = false;}

	v.y = WINDOW_HEIGHT / 2;
	// wave 4 
	if(enemyWave4 == false && st > enemyWave4start && st < enemyWave4start + enemyWave4length) {addEnemyCharacter0(&v, 16, 24); enemyWave4 = true;DPIF(true,"w4\n");}		
	if(st > enemyWave4start + enemyWave4length){enemyWave4 = false;Animation2D_reset(wave4animation);Animation2D_reset(wave4childAnimation);}


	// fixed 0
	if(enemyFixed0 == false && st > enemyFixed0start && st < enemyFixed0start + enemyFixed0length) {for(int x = 0; x < 5; x++){for(int i = 0; i < currentLap; i++){v.y = (double)i/currentLap*(WINDOW_HEIGHT-100) +100;addEnemyCharacter2(&v);} v.x = WINDOW_WIDTH + 100 + x*100;} enemyFixed0 = true;}		
	if(st > enemyFixed0start + enemyFixed0length){enemyFixed0 = false;}

	// follow 3
	v.y = WINDOW_HEIGHT - 100;
	if(enemyFollow3 == false && st > enemyFollow3start && st < enemyFollow3start + enemyFollow3length) {addEnemyCharacter1(&v, -50, 0, 5+currentLap*2); enemyFollow3 = true;}		
	if(st > enemyFollow3start + enemyFollow3length){enemyFollow3 = false;}




	// next lap
	if(sphinx.hp2 <= 0 && stage0->transform->position.x < -stage0ground0length-stage0ground0Offset)
	{
	stage0v = false;
	enemyWave0 = false;
	enemyWave1 = false;
	enemyFollow0 = false;
	enemyFollow1 = false;
	enemyWave2 = false;
	enemyWave3 = false;
	enemyFollow2 = false;
	enemyWave4 = false;
	Object_set_invalid(stage0);
	Timer_reset_count(stage0Timer);
	stage0->transform->position.x = 0;
	sphinx.childObject->transform->position.x = WINDOW_WIDTH + stage0_moving_speed * timeSphinxApper; // 60s
	Animation2D_play(bossReset);
	Animation2D_reset(bossStandUpAnim);
	Object_set_valid(sphinx.masterObject);

	enc0shotChance *= 0.75;
	enc1shotChance *= 0.83;
	enc2shotChance *= 0.92;

	enemyCharacter0.interval *= 0.76;
	enemyCharacter1.interval *= 0.9;
	enemyCharacter2.interval *= 0.82;

	enemyCharacter1.speed *= 1.3;

	sphinx.hp2 = SPHINX_DEFAULT_HP2;
	sphinx.hp1 = SPHINX_DEFAULT_HP1;

	sphinxShotChance *= 0.84;
	sphinxLazerInterval *= 0.91;

	enemyBullet0.speed *= 1.15;


	for(int i = 0; i < ENEMY_BULLET0_DEFAULT_NUM; i++)
	{
		enemyBullet0.object[i]->transform->scale.x *= 1.19;
		enemyBullet0.object[i]->transform->scale.y *= 1.19;
	}

	currentLap++;

	DP("clear\n");

	}

}
// }}}

// move funcs
// {{{
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

// }}}

// set stage anim
// {{{
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


// }}}

// vim:set foldmethod=marker:
