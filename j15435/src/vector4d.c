/* 
 * vector4d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <stdbool.h>

#include "vector4d.h"

void Vector4D_set(Vector4D* const p_this, const Vector4D* p_vec)
{
	p_this->x = p_vec->x;
	p_this->y = p_vec->y;
	p_this->z = p_vec->z;
	p_this->w = p_vec->w;
}

void Vector4D_set_zero(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
	p_this->w = 0.0;
}

void Vector4D_set_identity(Vector4D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 1.0;
	p_this->z = 1.0;
	p_this->w = 1.0;
}

void Vector4D_set_unit_x(Vector4D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
	p_this->w = 0.0;
}

void Vector4D_set_unit_y(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 1.0;
	p_this->z = 0.0;
	p_this->w = 0.0;
}

void Vector4D_set_unit_z(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 1.0;
	p_this->w = 0.0;
}

void Vector4D_set_unit_w(Vector4D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
	p_this->w = 1.0;
}

void Vector4D_set_all(Vector4D* const p_this, const int a)
{
	p_this->x = a;
	p_this->y = a;
	p_this->z = a;
	p_this->w = a;
}
void Vector4D_add(Vector4D* const p_this, const Vector4D* p_vec)
{
	p_this->x += p_vec->x;
	p_this->y += p_vec->y;
	p_this->z += p_vec->z;
	p_this->w += p_vec->w;
}

void Vector4D_sub(Vector4D* const p_this, const Vector4D* p_vec)
{
	p_this->x -= p_vec->x;
	p_this->y -= p_vec->y;
	p_this->z -= p_vec->z;
	p_this->w -= p_vec->w;
}

bool Vector4D_is_equal(const Vector4D* p_this, const Vector4D* p_vec)
{
	if(p_this->x == p_vec->x &&
	   p_this->y == p_vec->y &&
	   p_this->z == p_vec->z &&
	   p_this->w == p_vec->w )
		return true;
	return false;
}

bool Vector4D_is_zero(const Vector4D* p_this)
{
	Vector4D v;
	Vector4D_set_zero(&v);

	return Vector4D_is_equal(p_this, &v);
}
