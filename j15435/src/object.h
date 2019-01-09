/* 
 * object.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef OBJECT_H
#define OBJECT_H

// obj -> 111 098 106 -> 1119816
#define OBJECT_TYPE_ID 1119816

#include <GL/glut.h>
#include <GL/glpng.h>

#include "transform2d.h"
#include "view.h"
#include "vector2d.h"
#include "image2d.h"
#include "animation_controller2d.h"
 
struct private_variables;
typedef struct object
{
	Transform2D* transform;
	struct private_variables* pv;	
} Object;

Object* Object_new();
void Object_set_Image2D(Object* const, Image2D* const);
void Object_set_AnimationController2D(Object* const, AnimationController2D* const);
void Object_play_AnimationController2D(const Object*);
void Object_release(Object* const);

#endif
