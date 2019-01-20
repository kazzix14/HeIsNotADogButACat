/* 
 * image2d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <stdio.h>

#include "debug.h"
#include "timer.h"
#include "transform2d.h"
#include "image2d.h"
#include "animation2d.h"

static void release_images(Animation2D* const, const int, const int);

static struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct etnthapttsathots =
{
	.isVisible = true,
	.putFunc = (void*)Animation2D_play
};

struct frame
{
	int numVars;
	void** variables;
	void** values;
	double* length;
	char* smooth;
};

struct private_variables
{
	unsigned int anim_length; // frame num
	unsigned int current_frame;

	double frame_time_current;
	double* frame_time; // second
	Image2D** p_imgs;
	struct frame* frm;
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

	p_anim->pEtnthapttsathots = &etnthapttsathots;
	p_anim->pv->anim_length = 0;
	p_anim->pv->current_frame = 0;

	return p_anim;
}

void Animation2D_load(Animation2D* const p_this, const char* path_dir, const int num)
{
	if(p_this->pv->anim_length == 0)
	{
		p_this->pv->p_imgs = (Image2D**)malloc(sizeof(Image2D*) * num);
		p_this->pv->frame_time = (double*)malloc(sizeof(double) * num);
		p_this->pv->frm = (struct frame*)malloc(sizeof(struct frame) * num);
	}
	else if(num != p_this->pv->anim_length)
	{
		p_this->pv->p_imgs = (Image2D**)realloc(p_this->pv->p_imgs, sizeof(Image2D*) * num);
		p_this->pv->frame_time = (double*)realloc(p_this->pv->frame_time, sizeof(double) * num);
		p_this->pv->frm = (struct frame*)realloc(p_this->pv->frm, sizeof(struct frame) * num);
	}

	char path[ANIMATION2D_LOAD_PATH_LIMIT];
	for (int i = 0; i < num; i++)
	{
		if(p_this->pv->anim_length <= i)
			p_this->pv->p_imgs[i] = Image2D_new();
		snprintf(path, ANIMATION2D_LOAD_PATH_LIMIT, "%s/%d.png", path_dir, i);
		Image2D_load(p_this->pv->p_imgs[i], path);

		p_this->pv->p_imgs[i]->option = IMAGE2D_CENTER;
		p_this->pv->frame_time[i] = 0.1; //0.1second
	}
	p_this->pv->anim_length = num;
}

void Animation2D_release(Animation2D* const p_this)
{
	release_images(p_this, 0, p_this->pv->anim_length);
	free(p_this->pv->p_imgs);
	free(p_this->pv->frm);
	free(p_this->pv->frame_time);
	free(p_this->pv);
	free(p_this->transform);
	free(p_this);
}

void Animation2D_condigure_frame(Animation2D* const p_this, const unsigned int index, const unsigned int frame_length)
{
	p_this->pv->frame_time[index] = frame_length;
}

void Animation2D_play(const Animation2D* p_this)
{
	Transform2D* t = p_this->transform;
	glTranslated(t->position.x, t->position.y, 0.0);
	glRotated(t->rotation.w, t->rotation.x, t->rotation.y, t->rotation.z);
	glScaled(t->scale.x, t->scale.y, 1.0);

	Image2D_put(p_this->pv->p_imgs[p_this->pv->current_frame]);	

	if(p_this->pv->frame_time_current >= p_this->pv->frame_time[p_this->pv->current_frame])
	{
		p_this->pv->frame_time_current = 0;
		if(p_this->pv->current_frame == p_this->pv->anim_length-1)
			p_this->pv->current_frame = 0;
		else
			p_this->pv->current_frame++;

	}
	else
	{
		double tc;
		Timer_get_spf(&tc);
		p_this->pv->frame_time_current += tc;
	}
}

static void release_images(Animation2D* const p_this, const int from, const int to)
{
	for(int i = from; i < to;i++)
		Image2D_release(p_this->pv->p_imgs[i]);
}
