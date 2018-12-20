/* 
 * object.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glpng.h>

#include "transform2d.h"
#include "object.h"
#include "vector2d.h"
#include "view.h"

static void get_index_from_type_id(const Object*, const int, unsigned int* const);

struct private_variables
{
	void** p_cmpts;
	int* cmpt_types;
	unsigned int cmpt_num;
};

Object* Object_new()
{
	// allocate memory
	Object* p_obj;

	p_obj = (Object*)malloc(sizeof(Object));
	p_obj->pv = (struct private_variables*)malloc(sizeof(struct private_variables));
	//p_obj->pv->cmpnnts = ((void**)malloc(sizeof(void*));
	//p_obj->pv->cmpnnt_types = ((int*)malloc(sizeof(int));

	if(p_obj == NULL || p_obj->pv == NULL)
		return NULL;

	return p_obj;
}

void Object_add_component(Object* p_this, const void* p_cmpt const int type_id)
{
	// allocate memory
	if(p_this->cmpt_num == 0)
	{
		p_this->pv->p_cmpts = (void**)malloc(sizeof(void*));
		p_this->pv->cmpt_types = (int*)malloc(sizeof(int));
	}
	else
	{
		p_this->pv->p_cmpts = (void**)realloc(p_this->pv->p_cmpts, sizeof(void*) * (p_this->pv->cmpt_num + 1));
		p_this->pv->cmpt_types = (int*)realloc(p_this->pv->cmpt_num, sizeof(int) * (p_this->pv->cmpt_num + 1));
	}

	p_this->pv->p_cmpts[p_this->pv->cmpt_num] = p_cmpt;
	p_this->pv->cmpt_num += 1;
}

void Object_remove_component(Object* p_this, const int type_id)
{
	unsigned int index;
       	get_index_from_type_id(p_this, &index, type_id);

	p_this->pv->cmpt_num--;

	p_this->pv->p_cmpts[index] = p_this->pv->p_cmpts[p_this->pv->cmpt_num];
	p_this->pv->anim_cmpt_types[index] = p_this->pv->cmpt_types[p_this->pv->cmpt_num];

	p_this->pv->p_cmpts = (void**)realloc(p_this->pv->p_cmpts, sizeof(void*) * (p_this->pv->cmpt_num));
	p_this->pv->cmpt_types = (int*)realloc(p_this->pv->cmpt_num, sizeof(int) * (p_this->pv->cmpt_num));
}

void Object_release(Object* const p_this)
{
	//free(p_this->pv->);
	//free(p_this->pv);
	//free(p_this);
}

static void get_index_from_type_id(const Object* p_this, unsigned int* const p_index, const int type_id)
{
	for(int i = 0; i < p_this->pv->cmpt_num; i++)
		if(p_this->cmpt_num == type_id)
			*p_index = i;
}
