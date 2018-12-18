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

AnimationControler2D* AnimationController2D_new();
void AnimationController2D_release(Animation2D* const);
void AnimationController2D_load(Animation2D* const, const char*, const int num);
void AnimationController2D_play(const Animation2D*, const View*);
void AnimationController2D_put_with_offset(const Animation2D*, const View*, const Vector2D*);
void AnimationController2D_get_size(const Animation2D*, Vector2D* const);
void AnimationController2D_get_size_x(const Animation2D*, int* const);
void AnimationController2D_get_size_y(const Animation2D*, int* const);

#endif
