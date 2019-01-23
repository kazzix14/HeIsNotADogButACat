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
#include <stddef.h>

#include "debug.h"
#include "timer.h"
#include "transform2d.h"
#include "image2d.h"
#include "animation2d.h"

static struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct etnthapttsathots =
{
	.isVisible = true,
	.putFunc = (void*)Animation2D_play
};

struct frame
{
	void** values;
	double length;
};


struct private_variables
{
	unsigned int anim_length; // frame num
	int current_frame;

	double frame_time_current;
	double last_frame_time;
	double* frame_time; // second
	struct frame* frm;

	int numVars;
	void** variables; 
	size_t* sizes;
	char* smoothingTypes; // 0:no smoothing, 1:smoothing
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

void Animation2D_add_frame(Animation2D* const this)
{
	if(this->pv->anim_length == 0)
	{
		this->pv->frm = (struct frame*)malloc(sizeof(struct frame));
	}
	else
	{
		this->pv->frm = (struct frame*)realloc(this->pv->frm, sizeof(struct frame) * (this->pv->anim_length+1));
	}

	this->pv->numVars = 0;
	this->pv->frm[this->pv->anim_length].length = 1;

	this->pv->anim_length++;

}

void Animation2D_set_frame_length(Animation2D* const this, const unsigned int index, const double length)
{
	this->pv->frm[index].length = length;
}

//
void Animation2D_add_animated_variable(Animation2D* const this, const unsigned int index, void* const varaddr, const void* valaddr, const size_t sizeofval, const char smoothingType)
{

	if(this->pv->numVars == 0)
	{
		this->pv->variables = (void**)malloc(sizeof(void*));
		this->pv->frm[index].values = (void**)malloc(sizeof(void*)); 
		this->pv->sizes = (size_t*)malloc(sizeof(size_t));
		this->pv->smoothingTypes = (char*)malloc(sizeof(char));
	}
	else
	{
	
		DDO
		(
			void* a = this->pv->variables;
			void* b = this->pv->frm[index].values;
			void* c = this->pv->sizes;
			void* d = this->pv->smoothingTypes;
		)

		this->pv->variables = (void**)realloc(this->pv->variables, sizeof(void*) * (this->pv->numVars+1));
		this->pv->frm[index].values = (void**)realloc(this->pv->frm[index].values, sizeof(void*) * (this->pv->numVars+1));
		this->pv->sizes = (size_t*)realloc(this->pv->sizes, sizeof(size_t) * (this->pv->numVars+1));
		this->pv->smoothingTypes = (char*)realloc(this->pv->smoothingTypes, sizeof(char) * (this->pv->numVars+1));

		DEIF((a != this->pv->variables), "anm->pv->variables changed %p\n", this->pv->variables);
		DEIF((b != this->pv->frm[index].values), "anm->pv->frm[i]->values changed %p\n", this->pv->frm[index].values);
		DEIF((c != this->pv->sizes), "anm->pv->sizes changed %p\n", this->pv->sizes);
		DEIF((d != this->pv->smoothingTypes), "anm->pv->smoothingTypes changed %p\n", this->pv->smoothingTypes);
		
	}
	
	this->pv->frm[index].values[this->pv->numVars] = (void*) malloc(sizeofval);
	memcpy(this->pv->frm[index].values[this->pv->numVars], valaddr, sizeofval);

	this->pv->variables[this->pv->numVars] = varaddr;
	this->pv->sizes[this->pv->numVars] = sizeofval;
	this->pv->smoothingTypes[this->pv->numVars] = smoothingType;
	this->pv->numVars++;
}

void Animation2D_release(Animation2D* const p_this)
{
	/*
	release_images(p_this, 0, p_this->pv->anim_length);
	free(p_this->pv->p_imgs);
	free(p_this->pv->frm);
	free(p_this->pv->frame_time);
	free(p_this->pv);
	free(p_this->transform);
	free(p_this);
	*/
}
void Animation2D_play(Animation2D* const p_this)
{

	double tc;
	Timer_get_spf(&tc);
	p_this->pv->frame_time_current += tc;
	p_this->pv->last_frame_time = tc;

	for(int i = 0; i < p_this->pv->numVars; i++)
	{
		if(p_this->pv->smoothingTypes[i] == ANIMATION_LINER_SMOOTHING_FOR_DOUBLE)
		{

			/*
			 * var += val * timespent/frmlength
			 */

			double var;
			double val;
			double valp;
			int previousfrmindex;

			if(p_this->pv->current_frame - 1 < 0)
			{
				previousfrmindex = p_this->pv->anim_length - 1;
			}
			else
			{	
				previousfrmindex = p_this->pv->current_frame - 1;
			}
			
			// (void**)   p_this->pv->variables
			// (void*)    p_this->pv->variables[i]
			
			memcpy(&var,
				p_this->pv->variables[i],
				p_this->pv->sizes[i]);

			memcpy(&val,
				p_this->pv->frm[p_this->pv->current_frame].values[i],
				p_this->pv->sizes[i]);

			memcpy(&valp,
				p_this->pv->frm[previousfrmindex].values[i],
				p_this->pv->sizes[i]);

			var += (val-valp) * p_this->pv->last_frame_time / p_this->pv->frm[p_this->pv->current_frame].length;

			memcpy(p_this->pv->variables[i],
				&var,
				p_this->pv->sizes[i]);
		}
	}

	if(p_this->pv->frame_time_current >= p_this->pv->frm[p_this->pv->current_frame].length)
	{
		// copy all data
		for(int i = 0; i < p_this->pv->numVars; i++)
		{
			memcpy(p_this->pv->variables[i],
				p_this->pv->frm[p_this->pv->current_frame].values[i],
				p_this->pv->sizes[i]);
		}
		
		p_this->pv->frame_time_current = 0.0;
		if(p_this->pv->current_frame == p_this->pv->anim_length-1)
			p_this->pv->current_frame = 0;
		else
			p_this->pv->current_frame++;
	}
}
