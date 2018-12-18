/* 
 * image2d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>

#include "transform2d.h"
#include "image2d.h"
#include "vector2d.h"
#include "view.h"
#include "animation2d.h"

static void release_images(Animation2D* const, const int, const int);

struct private_variables
{
	unsigned int anim_length;
	unsigned int current_frame;
	Image2D** p_imgs;
};

Animation2D* Animation2D_new()
{
	// allocate memories
	Animation2D* p_anim;
	p_anim = (Animation2D*)malloc(sizeof(Animation2D));
	p_anim->pv = (struct private_variables*)malloc(sizeof(struct private_variables));
	p_anim->transform = Transform2D_new();

	if(p_anim == NULL || p_anim->pv == NULL || p_anim->transform == NULL)
		return NULL;

	return p_anim;
}

void Animation2D_load(Animation2D* const p_this, const char* path_dir, const int num)
{
	if(num < p_this->pv->anim_length)
		release_images(p_this, num, p_this->pv->anim_length);

	p_this->pv->anim_length = num;
	char path[ANIMATION2D_LOAD_PATH_LIMIT];
	for (int i = 0; i < num; i++)
	{
		if(p_this->pv->p_imgs[i] == NULL)
			p_this->pv->p_imgs[i] = Image2D_new();
		snprintf(path, ANIMATION2D_LOAD_PATH_LIMIT, "%s/%d", path_dir, i);
		Image2D_load(p_this->pv->p_imgs[i], path);
	}
}

static void release_images(Animation2D* const p_this, const int from, const int to)
{
	for(int i = from; i < to;i++)
		Image2D_release(p_this->pv->p_imgs[i]);
}

void Animation2D_release(Animation2D* const p_this)
{
	release_images(p_this, 0, p_this->pv->anim_length);
	free(p_this->pv);
	free(p_this->transform);
	free(p_this);
}

void Animation2D_play(const Animation2D* p_this, const View* view)
{
	Image2D_put(p_this->pv->p_imgs[p_this->pv->current_frame], view);	
	p_this->pv->current_frame += 1;
}

void Animation2D_get_size(const Animation2D* p_this, Vector2D* const p_rtrn)
{
}

void Animation2D_get_size_x(const Animation2D* p_this, int* const p_rtrn)
{
}

void Animation2D_get_size_y(const Animation2D* p_this, int* const p_rtrn)
{
}
