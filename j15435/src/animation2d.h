/* 
 * animation2d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef ANIMATION2D_H
#define ANIMATION2D_H

#define ANIMATION2D_LOAD_PATH_LIMIT 256
#define ANIMATION2D_TOP_LEFT 1
#define ANIMATION2D_CENTER 2

#include <GL/glut.h>
#include <GL/glpng.h>

#include "transform2d.h"
#include "view.h"
#include "vector2d.h"
 
struct private_variables;
typedef struct animation2d
{
	Transform2D* p_transform;	
	struct private_variables* p_vars;	
} Animation2D;

Animation2D* Animation2D_new();
void Animation2D_release(Animation2D* const);
void Animation2D_load(Animation2D* const, const char*);
void Animation2D_put(const Animation2D*, const View*);
void Animation2D_put_with_offset(const Animation2D*, const View*, const Vector2D*);
void Animation2D_put_at(const Animation2D*, const View*, const Vector2D*);
void Animation2D_get_info(const Animation2D*, pngInfo* const);
void Animation2D_get_id(const Animation2D*, GLuint* const);
void Animation2D_get_size(const Animation2D*, Vector2D* const);
void Animation2D_get_size_x(const Animation2D*, int* const);
void Animation2D_get_size_y(const Animation2D*, int* const);

#endif
