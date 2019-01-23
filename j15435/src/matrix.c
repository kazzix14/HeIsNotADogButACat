/* 
 * matrix.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include "vector2d.h"
#include "matrix.h"

void Matrix_set(Matrix* const p_this, const Matrix* p_vec)
{
	p_this->x = p_vec->x;
	p_this->y = p_vec->y;
	p_this->z = p_vec->z;
}

void Matrix_set_zero(Matrix* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
}

void Matrix_set_identity(Matrix* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 1.0;
	p_this->z = 1.0;
}

void Matrix_set_unit_x(Matrix* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
}

void Matrix_set_unit_y(Matrix* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 1.0;
	p_this->z = 0.0;
}

void Matrix_set_unit_z(Matrix* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 1.0;
}

void Matrix_add(Matrix* const p_this, const Matrix* p_vec)
{
	p_this->x += p_vec->x;
	p_this->y += p_vec->y;
	p_this->z += p_vec->z;
}

void Matrix_sub(Matrix* const p_this, const Matrix* p_vec)
{
	p_this->x -= p_vec->x;
	p_this->y -= p_vec->y;
	p_this->z -= p_vec->z;
}

void Matrix_to_Vector2D(const Matrix* this, Vector2D* const vec)
{
	vec->x = this->x;
	vec->y = this->y;
}
