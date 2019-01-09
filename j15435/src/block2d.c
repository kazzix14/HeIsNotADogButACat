/* * block2d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glpng.h>

#include "transform2d.h"
#include "block2d.h"
#include "vector2d.h"
#include "view.h"
#include "image2d.h"
#include "animation_controller2d.h"

struct private_variables
{
	Image2D* img;
	unsigned char hp;
};


Block2D* Block2D_new()
{
	// allocate memory
	Block2D* blk;
	blk = (Block2D*)malloc(sizeof(Block2D));
	blk->pv = (struct private_variables*)malloc(sizeof(struct private_variables));
	blk->pv->img = Image2D_new();
	Transform2D_set_default(blk->pv->img->p_transform);

	if( blk	     	 == NULL ||
    	    blk->pv->img == NULL )
		return NULL;

	return blk;
}

void Block2D_release(Block2D* const this)
{
	//free(p_this->pv->);
	//free(p_this->pv);
	//free(p_this);
}

void Block2D_load_Image2D(Block2D* const this, const char* path)
{
	Image2D_load(this->pv->img, path);
}

void Block2D_put(const Block2D* this)
{
	Image2D_put(this->pv->img);
}

void Block2D_get_hp(const Block2D* this, unsigned char* const hp)
{
	*hp = this->pv->hp;
}


void Block2D_set_hp(Block2D* const this, const unsigned char hp)
{
	this->pv->hp = hp;
}
