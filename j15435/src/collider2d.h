/* 
 * collider2d.h
 *
 * (C) 2019 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef COLLIDER2D_H
#define COLLIDER2D_H

#define COLLIDER2D_LAYER_NUM 32
#define COLLIDER2D_HITS_NUM 32
#define COLLIDER2D_TAG_LENGTH 4

#define COLLIDER2D_COLLIDER_POINT 0
#define COLLIDER2D_COLLIDER_CIRCLE 1
#define COLLIDER2D_COLLIDER_LINE 2
#define COLLIDER2D_COLLIDER_RECT 3
#define COLLIDER2D_COLLIDER_OBB 4

#include "vector2d.h"

struct obb;
typedef struct collider2d Collider2D;

typedef struct collider2d
{
	struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct* pEtnthapttsathots;
	void* colObj;
	Collider2D* hits[COLLIDER2D_HITS_NUM]; // array of pointer to objects hit
	char tag[COLLIDER2D_TAG_LENGTH]; //
	bool isValid;
} Collider2D;

typedef struct rectcollider
{
	char colType;
	Vector2D position; //
	Vector2D size;
	struct obb *bb;
} RectCollider;

typedef struct circlecollider
{
	char colType;
	Vector2D position; //
	double radius;
} CircleCollider;

typedef struct pointcollider
{
	char colType;
	Vector2D position1; //
	Vector2D position2; //
} PointCollider;

typedef struct linecollider
{
	char colType;
	Vector2D position; //
} LineCollider;

Collider2D* Collider2D_new();
void Collider2D_set_collider_object(Collider2D* const, char);
void Collider2D_register_collider(Collider2D* const, int);
void Collider2D_judge_all();
void Collider2D_judge(Collider2D* const, Collider2D* const);
void Collider2D_update(Collider2D* const);
void Collider2D_set_layer_matrix(const char[(COLLIDER2D_LAYER_NUM+1)*COLLIDER2D_LAYER_NUM/2]);
void Collider2D_get_layer_matrix(char[(COLLIDER2D_LAYER_NUM+1)*COLLIDER2D_LAYER_NUM/2]);

#endif
