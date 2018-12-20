/* 
 * vector2d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef VECTOR2D_H
#define VECTOR2D_H

// vec2 ascii
#define VECTOR2D_TYPE_ID 11811995
typedef struct vector2d
{
	double x,
	       y;
} Vector2D;

void Vector2D_set(Vector2D* const, const Vector2D* );
void Vector2D_set_zero(Vector2D* const);
void Vector2D_set_identity(Vector2D* const);
void Vector2D_set_unit_x(Vector2D* const);
void Vector2D_set_unit_y(Vector2D* const);
void Vector2D_add(Vector2D* const, const Vector2D*);
void Vector2D_sub(Vector2D* const, const Vector2D*);

#endif
