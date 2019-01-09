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

// an2 -> 097 110 050 -> 97 11 5
#define ANIMATION2D_TYPE_ID 97115
#define ANIMATION2D_LOAD_PATH_LIMIT 256

#include "transform2d.h"
#include "vector2d.h"
 
struct private_variables;
typedef struct animation2d
{
	Transform2D* transform;	
	struct private_variables* pv;	
} Animation2D;

Animation2D* Animation2D_new();
void Animation2D_release(Animation2D* const);
void Animation2D_load(Animation2D* const, const char*, const int);
void Animation2D_play(const Animation2D*);
void Animation2D_set_frame_length(Animation2D* const, const unsigned int);
void Animation2D_put_with_offset(const Animation2D*, const Vector2D*);

#endif
