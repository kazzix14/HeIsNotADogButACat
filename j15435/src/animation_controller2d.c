/* 
 * animation_controller2d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK


#include "transform2d.h"
#include "image2d.h"
#include "animation2d.h"
#include "animation_controller2d.h"

static void get_index_from_name(const AnimationController2D*, unsigned int* const, const char*);

struct private_variables
{
	Animation2D** p_anims;
	char** anim_names;
	Animation2D* current_anim;
	unsigned int anim_num;
};

AnimationController2D* AnimationController2D_new()
{
	// allocate memory
	AnimationController2D* p_anmcnt;
	p_anmcnt = (AnimationController2D*)malloc(sizeof(AnimationController2D));
	p_anmcnt->pv = (struct private_variables*)malloc(sizeof(struct private_variables));

	// return NULL when it fails to allocate memories
	if(p_anmcnt == NULL || p_anmcnt->pv == NULL)
		return NULL;

	p_anmcnt->pv->anim_num = 0;

	return p_anmcnt;
}

// sharing a animation object among animation controllers may cause a problem
void AnimationController2D_add_animation(AnimationController2D* p_this, Animation2D* const p_anim, const char* name)
{
	// allocate memory
	if(p_this->pv->anim_num == 0)
	{
		p_this->pv->anim_names = (char**)malloc(sizeof(char*));
		p_this->pv->p_anims = (Animation2D**)malloc(sizeof(Animation2D*));
	}
	else
	{
		p_this->pv->anim_names = (char**)realloc(p_this->pv->anim_names, sizeof(char*) * (p_this->pv->anim_num + 1));
		p_this->pv->p_anims = (Animation2D**)realloc(p_this->pv->p_anims, sizeof(Animation2D*) * (p_this->pv->anim_num + 1));
	}

	p_this->pv->anim_names[p_this->pv->anim_num] = (char*)malloc(sizeof(char) * ANIMATION_CONTROLLER2D_ANIMATION_NAME_LIMIT);

	strncpy(p_this->pv->anim_names[p_this->pv->anim_num], name, ANIMATION_CONTROLLER2D_ANIMATION_NAME_LIMIT);
	p_this->pv->p_anims[p_this->pv->anim_num] = p_anim;

	p_this->pv->anim_num += 1;
}

void AnimationController2D_remove_animation(AnimationController2D* p_this, const char* name)
{
	unsigned int index;
       	get_index_from_name(p_this, &index, name);

	p_this->pv->anim_num--;

	p_this->pv->p_anims[index] = p_this->pv->p_anims[p_this->pv->anim_num-1];
	p_this->pv->anim_names[index] = p_this->pv->anim_names[p_this->pv->anim_num-1];

	p_this->pv->anim_names = (char**)realloc(p_this->pv->anim_names, sizeof(char*) * (p_this->pv->anim_num));
	p_this->pv->p_anims = (Animation2D**)realloc(p_this->pv->p_anims, sizeof(Animation2D*) * (p_this->pv->anim_num));
}

void AnimationController2D_release(AnimationController2D* const p_this)
{
	//release_images(p_this, 0, p_this->pv->anim_length);
	//free(p_this->pv->p_imgs);
	//free(p_this->pv);
	//free(p_this->transform);
	//free(p_this);
}

void AnimationController2D_play(const AnimationController2D* p_this)
{
	Animation2D_play(p_this->pv->current_anim);
}

void AnimationController2D_switch(AnimationController2D* const p_this, const char* name)
{
	unsigned int index;
	get_index_from_name(p_this, &index, name);
	p_this->pv->current_anim = p_this->pv->p_anims[index];
}

static void get_index_from_name(const AnimationController2D* p_this, unsigned int* const p_index, const char* name)
{
	for(int i = 0; i < p_this->pv->anim_num; i++)
		if(strncmp(p_this->pv->anim_names[i], name, ANIMATION_CONTROLLER2D_ANIMATION_NAME_LIMIT) == 0)
			*p_index = i;
}
