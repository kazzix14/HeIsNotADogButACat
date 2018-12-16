/* 
 * vector3d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include "vector3d.h"

void Vector3D_set(Vector3D* const p_this, const Vector3D* p_vec)
{
	p_this->x = p_vec->x;
	p_this->y = p_vec->y;
	p_this->z = p_vec->z;
}

void Vector3D_set_zero(Vector3D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
}

void Vector3D_set_identity(Vector3D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 1.0;
	p_this->z = 1.0;
}

void Vector3D_set_unit_x(Vector3D* const p_this)
{
	p_this->x = 1.0;
	p_this->y = 0.0;
	p_this->z = 0.0;
}

void Vector3D_set_unit_y(Vector3D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 1.0;
	p_this->z = 0.0;
}

void Vector3D_set_unit_z(Vector3D* const p_this)
{
	p_this->x = 0.0;
	p_this->y = 0.0;
	p_this->z = 1.0;
}

void Vector3D_add(Vector3D* const p_this, const Vector3D* p_vec)
{
	p_this->x += p_vec->x;
	p_this->y += p_vec->y;
	p_this->z += p_vec->z;
}

void Vector3D_sub(Vector3D* const p_this, const Vector3D* p_vec)
{
	p_this->x -= p_vec->x;
	p_this->y -= p_vec->y;
	p_this->z -= p_vec->z;
}
