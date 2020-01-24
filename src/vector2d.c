/* 
 * vector2d.c
 *
 * (C) 2019 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <math.h>

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

void Vector2D_set_one(Vector2D* const p_this)
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

void Vector2D_normalize(Vector2D* const v)
{
	double va = sqrt(v->x*v->x + v->y*v->y);
	v->x = v->x / va;
	v->y = v->y / va;
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

void Vector2D_dot(const Vector2D* v1, const Vector2D* v2, double* const ret)
{
	*ret = v1->x * v2->x + v1->y * v2->y;
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

char Vector2D_compare_zero(const Vector2D* v1)
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
	return ((0 < v1->x) - (v1->x < 0) + 1) &
	       ((0 < v1->y) - (v1->x < 0) + 1) << 2;
}
