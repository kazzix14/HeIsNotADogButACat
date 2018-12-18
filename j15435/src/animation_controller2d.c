/* 
 * animation_controller2d.c
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
#include "animation_contoller2d.h"

static void release_images(AnimationController2D* const, const int, const int);
static unsigned int get_index_from_name(const AnimationController2D*, const char* name);

struct private_variables
{
	char** anim_names;
	Animation2D** p_anims;
	unsigned int anim_num;
};

AnimationController2D* AnimationController2D_new()
{
	// allocate memories
	AnimationController2D* p_anmcnt;
	p_anmcnt = (AnimationController2D*)malloc(sizeof(AnimationController2D));
	p_anmcnt->pv = (struct private_variables*)malloc(sizeof(struct private_variables));

	// return NULL when it fails to allocate memories
	if(p_anmcnt == NULL || p_anmcnt->pv == NULL)
		return NULL;

	return p_anmcnt;
}

// sharing a animation object among some animation controllers may cause a problem
// which animations on each controller will sync
void AnimationController2D_add_animation(AnimationController2D* const p_this, const Animation2D* p_anim, const char* name)
{
	// allocate mameory
	if(p_this->pv->length == 0)
	{
		p_this->pv->anim_names = (char**)malloc(sizeof(char*));
		p_this->pv->p_anims = (Animation2D**)malloc(sizeof(Animation2D*)));
	}
	else
	{
		p_this->pv->anim_names = (char**)realloc(p_this->pv->anim_names, sizeof(char*) * (p_this->pv->anim_num + 1));
		p_this->pv->p_anims = (Animation2D**)realloc(p_this->pv->p_anims, sizeof(Animation2D*) * (p_this->pv->anim_num + 1));
	}

	p_this->pv->anim_names[p_this->pv->anim_num] = name;
	p_this->pv->p_anims[p_this->pv->anim_num] = p_anim;

	p_this->pv->anim_num += 1;
}

void AnimationController2D_remove_animation(AnimationController2D* const p_this, const char* name)
{
	unsigned int index;
       	get_index_from_name(p_this, name, &index):
	p_this->pv->p_anims[index] = p_this->pv->p_anims[p_this->pv->anim_num-1];
	p_this->pv->anim_names[index] = p_this->pv->anim_names[p_this->pv->anim_num-1];
	p_this->pv->anim_num--;

	p_this->pv->anim_names = (char**)realloc(p_this->pv->anim_names, sizeof(char*) * (p_this->pv->anim_num));
	p_this->pv->p_anims = (Animation2D**)realloc(p_this->pv->p_anims, sizeof(Animation2D*) * (p_this->pv->anim_num));
}

static void release_images(AnimationController2D* const p_this, const int from, const int to)
{
	for(int i = from; i < to;i++)
		Image2D_release(p_this->pv->p_imgs[i]);
}

void AnimationController2D_release(AnimationController2D* const p_this)
{
	release_images(p_this, 0, p_this->pv->anim_length);
	free(p_this->pv->p_imgs);
	free(p_this->pv);
	free(p_this->transform);
	free(p_this);
}

void AnimationController2D_play(const AnimationController2D* p_this, const View* view)
{
	Image2D_put(p_this->pv->p_imgs[p_this->pv->current_frame], view);	

	if(++p_this->pv->current_frame == p_this->pv->anim_length)
		p_this->pv->current_frame = 0;
}

void AnimationController2D_get_size(const AnimationController2D* p_this, Vector2D* const p_rtrn)
{
}

void AnimationController2D_get_size_x(const AnimationController2D* p_this, int* const p_rtrn)
{
}

void AnimationController2D_get_size_y(const AnimationController2D* p_this, int* const p_rtrn)
{
}

static void get_index_from_name(const AnimationController2D*, const char* name, unsigned int* const p_index)
{
	for(int i = 0; i < p_this->pv->anim_num; i++)
		if(p_this->pv->anim_names[i] == name)
			*p_idnex = i;
	*p_index =  NULL;
}
