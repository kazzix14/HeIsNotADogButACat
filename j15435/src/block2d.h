/* 
 * block2d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glut.h>
#include <GL/glpng.h>

// blk2 -> 098 108 107 050 -> 9818175
#define BLOCK2D_TYPE_ID 9818175

#define BLOCK2D_IMAGE_SIZE 32

#include "transform2d.h"
#include "view.h"
#include "vector2d.h"
#include "image2d.h"
#include "animation_controller2d.h"
 
struct private_variables;
typedef struct block2d
{
	struct private_variables* pv;	
} Block2D;

Block2D* Block2D_new();
void Block2D_load_Image2D(Block2D* const, const char*);
void Block2D_release(Block2D* const);
void Block2D_put(const Block2D*);
void Block2D_get_hp(const Block2D*, unsigned char* const);
void Block2D_set_hp(Block2D* const, const unsigned char);

#endif
