/* 
 * vector2d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include "vector2d.h"

void Vector2D_set(Vector2D* const p_this, const Vector2D* p_vec)
{
	p_this->x = p_vec->x;
	p_this->y = p_vec->y;
}

void Vector2D_set_zero(Vector2D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
}

void Vector2D_set_identity(Vector2D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 1.0;
}
void Vector2D_set_unit_x(Vector2D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 0.0;
}

void Vector2D_set_unit_y(Vector2D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 1.0;
}

void Vector2D_add(Vector2D* const p_this, const Vector2D* p_vec)
{
	p_this->x += p_vec->x;
	p_this->y += p_vec->y;
}

void Vector2D_sub(Vector2D* const p_this, const Vector2D* p_vec)
{
	p_this->x -= p_vec->x;
	p_this->y -= p_vec->y;
}

char Vector2D_compare(const Vector2D* v1, const Vector2D* v2)
{
	/* 
	 * 1x < 2x -> 0000
	 * 1x = 2x -> 0001
	 * 1x > 2x -> 0010
	 * OR
	 * 1y < 2y -> 0000
	 * 1y = 2y -> 0100
	 * 1y > 2y -> 1000
	 */
	return ((v2->x < v1->x) - (v1->x < v2->x) + 1) &
	       ((v2->y < v1->y) - (v1->x < v2->y) + 1) << 2;
}
