/* 
 * matrix.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix
{
	double x,
	       y,
	       z;
} Matrix;

void Matrix_set(Matrix* const, const Matrix*);
void Matrix_set_zero(Matrix* const);
void Matrix_set_identity(Matrix* const);
void Matrix_set_unit_x(Matrix* const);
void Matrix_set_unit_y(Matrix* const);
void Matrix_set_unit_z(Matrix* const);
void Matrix_add(Matrix* const, const Matrix*);
void Matrix_sub(Matrix* const, const Matrix*);
void Matrix_to_Vector2D(const Matrix*, Vector2D* const);

#endif
