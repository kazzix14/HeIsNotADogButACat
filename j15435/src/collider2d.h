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

#define COLLIDER2D_COLLIDER_TYPE_POINT 0;
#define COLLIDER2D_COLLIDER_TYPE_CIRCLE 1;
#define COLLIDER2D_COLLIDER_TYPE_LINE 2;
#define COLLIDER2D_COLLIDER_TYPE_RECT 3;

#include "vector2d.h"

typedef struct collider2d
{
	void* collObj;
	char layer[4]; // 8 bit * 4 : 32 layer
} Collider2D;

typedef struct rect
{
	char collType;
	Vector2D position; //
	Vector2D vec1;
	Vector2D vec2;
} Rect;

typedef struct circle
{
	char collType;
	Vector2D position; //
	double radius;
} Circle;

typedef struct point
{
	char collType;
	Vector2D position; //
} Point;

Collider2D* Collider2D_new();
void Collider2D_set_collider_object(Collider2D* const, char);

#endif
