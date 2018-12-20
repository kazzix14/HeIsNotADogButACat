/* 
 * object.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef IMAGE2D_H
#define IMAGE2D_H

#define IMAGE2D_TOP_LEFT 1
#define IMAGE2D_CENTER 2

#include <GL/glut.h>
#include <GL/glpng.h>

#include "transform2d.h"
#include "view.h"
#include "vector2d.h"
 
struct private_variables;
typedef struct object
{
	void** components;
	unsigned int components_num;
	struct private_variables* pv;	
} Object;

Object* Object_new();
void Object_release(Object* const);
void Object_load(Object* const, const char*);
void Object_put(const Object*, const View*);
void Object_put_at_with_option(const Object*, const View*, const Vector2D*, const int option);
void Object_put_at(const Object*, const View*, const Vector2D*);
void Object_get_info(const Object*, pngInfo* const);
void Object_get_id(const Object*, GLuint* const);
void Object_get_size(const Object*, Vector2D* const);
void Object_get_size_x(const Object*, int* const);
void Object_get_size_y(const Object*, int* const);

#endif
