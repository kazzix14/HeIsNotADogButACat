/* * object.c
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
#include "image2d.h"
#include "animation_controller2d.h"

struct private_variables
{
	Image2D* p_img;
	AnimationController2D* p_ac;
};

Object* Object_new()
{
	// allocate memory
	Object* p_obj;

	p_obj = (Object*)malloc(sizeof(Object));
	p_obj->transform = Transform2D_new();
	p_obj->pv = (struct private_variables*)malloc(sizeof(struct private_variables));
	p_obj->pv->p_img = Image2D_new();
	p_obj->pv->p_ac = AnimationController2D_new();

	if( p_obj	     == NULL ||
	    p_obj->pv	     == NULL ||
	    p_obj->pv->p_img == NULL ||
	    p_obj->pv->p_ac  == NULL )
		return NULL;

	return p_obj;
}

void Object_set_Image2D(Object* const p_this, Image2D* const p_img)
{
	p_this->pv->p_img = p_img;
}

void Object_set_AnimationController2D(Object* const p_this, AnimationController2D* const p_ac)
{
	p_this->pv->p_ac = p_ac;
}

void Object_play_AnimationController2D(const Object* p_this)
{
	Transform2D* t = p_this->transform;
	glTranslated(t->position.x, t->position.y, 0.0f);
	glRotated(t->rotation.w, t->rotation.x, t->rotation.y, t->rotation.z);
	glScaled(t->scale.x, t->scale.y, 1.0f);
	
	AnimationController2D_play(p_this->pv->p_ac);
}

void Object_release(Object* const p_this)
{
	//free(p_this->pv->);
	//free(p_this->pv);
	//free(p_this);
}
