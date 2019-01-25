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
#define COLLIDER2D_TAG_LENGTH 4

#define COLLIDER2D_COLLIDER_POINT 0
#define COLLIDER2D_COLLIDER_CIRCLE 1
#define COLLIDER2D_COLLIDER_LINE 2
#define COLLIDER2D_COLLIDER_RECT 3

#include "vector2d.h"

typedef struct collider2d
{
	struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct* pEtnthapttsathots;
	void* colObj;
	void** objHits; // pointer to objects hit
	char tag[COLLIDER2D_TAG_LENGTH]; //
} Collider2D;

typedef struct rectcollider
{
	char colType;
	Vector2D position; //
	Vector2D vec1;
	Vector2D vec2;
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
	Vector2D position; //
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

#endif
