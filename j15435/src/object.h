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

// obj -> 111 098 106 -> 1119816
#define OBJECT_TYPE_ID 1119816

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
	struct private_variables* pv;	
} Object;

Object* Object_new();
void Object_add_component(Object* p_this, const void* p_cmpt const int type_id)
void Object_remove_component(Object* p_this, const int type_id)
void Object_release(Object* const);

#endif
