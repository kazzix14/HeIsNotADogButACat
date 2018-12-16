/* 
 * vector4d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef VECTOR4D_H
#define VECTOR4D_H

#include <stdbool.h>

typedef struct vector4d
{
	double x,
	       y,
	       z,
	       w;
} Vector4D;

void Vector4D_set(Vector4D* const, const Vector4D*);
void Vector4D_set_zero(Vector4D* const);
void Vector4D_set_identity(Vector4D* const);
void Vector4D_set_unit_x(Vector4D* const);
void Vector4D_set_unit_y(Vector4D* const);
void Vector4D_set_unit_z(Vector4D* const);
void Vector4D_set_unit_w(Vector4D* const);
void Vector4D_set_all(Vector4D* const, const int);
void Vector4D_add(Vector4D* const, const Vector4D*);
void Vector4D_sub(Vector4D* const, const Vector4D*);
bool Vector4D_is_equal(const Vector4D*, const Vector4D*);
bool Vector4D_is_zero(const Vector4D*);

#endif
