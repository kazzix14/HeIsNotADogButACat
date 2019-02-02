/* * object.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <stdlib.h>
#include <stdbool.h>

#include <GL/gl.h>

#include "everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct.h"
#include "transform2d.h"
#include "object.h"
#include "view.h"

static struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct etnthapttsathots =
{
	.isVisible = true,
	.putFunc = (void*)Object_put
};


struct private_variables
{
	void** components;
	unsigned int componentNum;
	bool isValid;
};

Object* Object_new()
{
	// allocate memory
	Object* p_obj;

	p_obj = (Object*)malloc(sizeof(Object));
	p_obj->transform = Transform2D_new();
	p_obj->pv = (struct private_variables*)malloc(sizeof(struct private_variables));
	p_obj->pv->components = (void**)malloc(sizeof(void*) * 1);

	p_obj->pEtnthapttsathots = &etnthapttsathots;
	p_obj->pv->isValid = true;
	p_obj->pv->componentNum = 0;

	if( p_obj	     	  == NULL ||
	    p_obj->pv	     	  == NULL ||
	    p_obj->pv->components == NULL)
		return NULL;

	return p_obj;
}

void Object_add_component(Object* const this, void* const newComponent)
{
	this->pv->components = (void**)realloc(this->pv->components, sizeof(void*) * (this->pv->componentNum+1));
	this->pv->components[this->pv->componentNum] = newComponent;
	this->pv->componentNum++;
}

void Object_remove_component(Object* const this, void* const remComponent)
{
	bool found = false;
	for(int i = 0; i < this->pv->componentNum; i++)
	{
		if(this->pv->components[i] == remComponent)
		{
			this->pv->componentNum--;
			found = true;
		}
		if(found == true)
		{
			this->pv->components[i] = this->pv->components[i+1];
		}
	}
	this->pv->components = (void**)realloc(this->pv->components, sizeof(void*) * (this->pv->componentNum));
}

void Object_clear_component(Object* const this)
{
	this->pv->components = (void**)realloc(this->pv->components, 0);
	this->pv->componentNum = 0;
}

void Object_set_valid(Object* const this)
{
	this->pv->isValid = true;
}

void Object_set_invalid(Object* const this)
{
	this->pv->isValid = false;
}

void Object_is_valid(Object* const this, bool* const ret)
{
	*ret = this->pv->isValid;
}

void Object_put(const Object* this)
{
	if(this->pv->isValid == false) return;
	View_begin();

		Transform2D* t = this->transform;
		glTranslated(t->position.x, t->position.y, 0.0f);
		glRotated(t->rotation.w, t->rotation.x, t->rotation.y, t->rotation.z);
		glScaled(t->scale.x, t->scale.y, 1.0f);
		
		// call putFunc() of every component if it is visible
		for(int i = 0; i < this->pv->componentNum; i++)
		{
			// first member  : bool isVisible
			// second member : void (putFunc*)(void *)
			struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct* s;
			s = (struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct*)(*((void **)(this->pv->components[i])));
			if(s->isVisible == true)
			{
				s->putFunc(this->pv->components[i]);
			}
		}
	View_end();
}

void Object_release(Object* const p_this)
{
	//free(p_this->pv->);
	//free(p_this->pv);
	//free(p_this);
}
