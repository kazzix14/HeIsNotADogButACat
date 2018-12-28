/* * grid3d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glpng.h>

#include "transform2d.h"
#include "grid3d.h"
#include "block2d.h"
#include "vector2d.h"
#include "view.h"
#include "image2d.h"
#include "animation_controller2d.h"

struct private_variables
{
	unsigned char*** hp;
	Block2D**** blk;
	void**** cmpt;
	unsigned int width;
	unsigned int height;
	unsigned int depth;
};


Grid3D* Grid3D_new(unsigned int w, unsigned int h, unsigned int d)
{
	// allocate memory
	Grid3D* grid;
	grid = (Grid3D*)malloc(sizeof(Grid3D));
	grid->pv = (struct private_variables*)malloc(sizeof(struct private_variables));

	grid->pv->hp = (unsigned char***)malloc(sizeof(char**) * w);
	for(int i = 0; i < w; i++)
	{
		grid->pv->hp[i] = (unsigned char**)malloc(sizeof(char*) * h);
		for(int j = 0; j < h; j++)
		{
			grid->pv->hp[i][j] = (unsigned char*)malloc(sizeof(char) * d);
			if(grid->pv->hp[i][j] == NULL)
				return NULL;
		}
	}

	grid->pv->blk = (Block2D****)malloc(sizeof(Block2D***) * w);
	for(int i = 0; i < w; i++)
	{
		grid->pv->blk[i] = (Block2D***)malloc(sizeof(Block2D**) * h);
		for(int j = 0; j < h; j++)
		{	
			grid->pv->blk[i][j] = (Block2D**)malloc(sizeof(Block2D*) * d);
			if(grid->pv->hp[i][j] == NULL)
				return NULL;
		}
	}

	grid->pv->cmpt = (void****)malloc(sizeof(void***) * w);
	for(int i = 0; i < w; i++)
	{
		grid->pv->cmpt[i] = (void***)malloc(sizeof(void**) * h);
		for(int j = 0; j < h; j++)
		{
			grid->pv->cmpt[i][j] = (void**)malloc(sizeof(void*) * d);
			if(grid->pv->cmpt[i][j] == NULL)
				return NULL;
		}
	}

	grid->pv->width  = w;
	grid->pv->height = h;
	grid->pv->depth  = d;

	if( grid     == NULL ||
    	    grid->pv == NULL )
		return NULL;

	return grid;
}

void Grid3D_release(Grid3D* const this)
{
	//free(p_this->pv->);
	//free(p_this->pv);
	//free(p_this);
}

void Grid3D_put(const Grid3D* this)
{
	//Image2D_put(this->pv->img);
}

void Grid3D_set_Block2D(Grid3D* const this, const unsigned int w, const unsigned int h, const unsigned int d, Block2D* const blk)
{
	if(0 <= w && w < this->pv->width  &&
	   0 <= h && h < this->pv->height &&
	   0 <= d && d < this->pv->depth  )
	{
		this->pv->blk[w][h][d] = blk;
		Block2D_get_hp(blk, &(this->pv->hp[w][h][d]));
	}
}
