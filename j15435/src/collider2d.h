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
#define COLLIDER2D_COLLIDER_TYPE_CIRCLE 0;
#define COLLIDER2D_COLLIDER_TYPE_OBB 0;
#define COLLIDER2D_COLLIDER_TYPE_LINE 0;

#include "vector2d.h"

typedef struct collider2d
{
	void* collObj;
	char layer[4]; // 8 bit * 4 : 32 layer
} Collider2D;

typedef struct obb2d
{
	char collType;
	Vector2D position; //
	Vector2D vec1;
	Vector2D vec2;
} Obb2D;

Collider2D* Collider2D_new();

#endif
