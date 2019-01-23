/* 
 * image2d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef IMAGE2D_H
#define IMAGE2D_H

// im2 -> 105 109 050
#define IMAGE2D_TYPE_ID 15195
#define IMAGE2D_TOP_LEFT 1
#define IMAGE2D_CENTER 2

#include <GL/glut.h>
#include <GL/glpng.h>

#include "everytypeneedtohaveapointertothisstructattheheadofthestruct.h"
#include "transform2d.h"
#include "vector4d.h"
 
struct private_variables;
typedef struct image2d
{
	struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct* pEtnthapttsathots;
	Transform2D* p_transform;	
	int option;
	Vector4D color;
	struct private_variables* p_vars;	
} Image2D;

Image2D* Image2D_new();
void Image2D_release(Image2D* const);
void Image2D_load(Image2D* const, const char*);
void Image2D_put(const Image2D*);
void Image2D_put_at_with_option(const Image2D*, const Vector2D*, const int option);
void Image2D_put_at(const Image2D*, const Vector2D*);
void Image2D_get_info(const Image2D*, pngInfo* const);
void Image2D_get_id(const Image2D*, GLuint* const);
void Image2D_get_size(const Image2D*, Vector2D* const);
void Image2D_get_width(const Image2D*, int* const);
void Image2D_get_height(const Image2D*, int* const);

#endif
