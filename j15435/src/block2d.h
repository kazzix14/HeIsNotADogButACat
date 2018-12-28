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

// blk -> 098 108 107 -> 981817
#define BLOCK_TYPE_ID 981817

#include <GL/glut.h>
#include <GL/glpng.h>

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
void Block2D_set_Image2D(Block2D* const, Image2D* const);
void Block2D_release(Block2D* const);
void Block2D_put(const Block2D*);
void Block2D_get_hp(const Block2D*, unsigned char* const);
void Block2D_set_hp(Block2D* const, const unsigned char);

#endif
