/* 
 * grid3d.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef GRID3D_H
#define GRID3D_H

// blk -> 098 108 107 -> 981817
#define BLOCK_TYPE_ID 981817

#include "view.h"
#include "block2d.h"

struct private_variables;
typedef struct grid3d
{
	struct private_variables* pv;	
} Grid3D;

Grid3D* Grid3D_new(unsigned int, unsigned int, unsigned int);
void Grid3D_release(Grid3D* const);
void Grid3D_put(const Grid3D*, const unsigned int, const View*);
void Grid3D_set_Block2D(Grid3D* const, const unsigned int, const unsigned int, const unsigned int, Block2D* const);

#endif
