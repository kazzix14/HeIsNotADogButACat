/* 
 * vector3d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef VECTOR3D_H
#define VECTOR3D_H

// vec3 ascii
#define VECTOR3D_TYPE_ID 118119951

#include "vector2d.h"

typedef struct vector3d
{
	double x,
	       y,
	       z;
} Vector3D;

void Vector3D_set(Vector3D* const, const Vector3D*);
void Vector3D_set_zero(Vector3D* const);
void Vector3D_set_identity(Vector3D* const);
void Vector3D_set_unit_x(Vector3D* const);
void Vector3D_set_unit_y(Vector3D* const);
void Vector3D_set_unit_z(Vector3D* const);
void Vector3D_add(Vector3D* const, const Vector3D*);
void Vector3D_sub(Vector3D* const, const Vector3D*);
void Vector3D_to_Vector2D(const Vector3D*, Vector2D* const);

#endif
