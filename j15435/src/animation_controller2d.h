/* 
 * animation_controller2d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef ANIMATION_CONTROLLER2D_H
#define ANIMATION_CONTROLLER2D_H

// an2c -> 097 110 050 099 -> 97 11 5 99
#define ANIMATION_CONTROLLER2D_TYPE_ID 9711599
#define ANIMATION_CONTROLLER2D_ANIMATION_NAME_LIMIT 32

#include <GL/glut.h>
#include <GL/glpng.h>

#include "transform2d.h"
#include "view.h"
#include "vector2d.h"
 
struct private_variables;
typedef struct animationcontroller2d
{
	struct private_variables* pv;	
} AnimationController2D;

AnimationController2D* AnimationController2D_new();
void AnimationController2D_add_animation(AnimationController2D*, Animation2D* const, const char*);
void AnimationController2D_remove_animation(AnimationController2D*, const char*);
void AnimationController2D_release(AnimationController2D* const);
void AnimationController2D_load(AnimationController2D* const, const char*, const int num);
void AnimationController2D_get_size(const AnimationController2D*, Vector2D* const);
void AnimationController2D_get_size_x(const AnimationController2D*, int* const);
void AnimationController2D_get_size_y(const AnimationController2D*, int* const);

#endif
