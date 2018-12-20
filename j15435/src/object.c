/* 
 * object.c
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
#include "object.h"
#include "vector2d.h"
#include "view.h"

struct private_variables
{
};

Object* Object_new()
{
	// allocate memories
	Object* p_obj;

	p_obj = (Object*)malloc(sizeof(Object));
	p_obj->pv = (struct private_variables*)malloc(sizeof(struct private_variables));
	p_obj->components = ((void**)malloc(sizeof(void*));

	if(p_obj == NULL || p_obj->pv == NULL || p_obj->components == NULL)
		return NULL;

	return p_obj;
}

void Object_add_component(Object* p_this, const void* p_comp, unsigned int* const index)
{
	// allocate memory
	if(p_this->components_num == 0)
	{
		p_this->components = (void**)malloc(sizeof(void*));
		p_this->pv->p_anims = (**)malloc(sizeof(Animation2D*));
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

void Object_release(Object* const p_this)
{
	free(p_this->p_vars);
	free(p_this->p_transform);
	free(p_this);
}

void Object_load(Object* const p_this, const char* path)
{
	p_this->p_vars->img = pngBind(path, 
					PNG_NOMIPMAP,
					PNG_ALPHA,
					&(p_this->p_vars->info),
					GL_CLAMP,
					GL_NEAREST,
					GL_NEAREST);
}

void Object_put(const Object* p_this, const View* view)
{
	Object_put_at(p_this, view, &(p_this->p_transform->position));	
}

void Object_put_with_offset(const Object* p_this, const View* view, const Vector2D* p_offset)
{
	Object_put_at(p_this, view, &(p_this->p_transform->position) );	
}

void Object_put_at(const Object* p_this, const View* view, const Vector2D* position)
{
	int w = p_this->p_vars->info.Width,
	    h = p_this->p_vars->info.Height;

	int x,
	    y;

	switch(p_this->option)
	{

		default:
		case IMAGE2D_TOP_LEFT:
			x = 0;
			y = 0;
			break;

		case IMAGE2D_CENTER:
			x = -w/2;
			y = -h/2;
	}

	View_begin_2d(view);

	Transform2D* t = p_this->p_transform;
	glTranslated(position->x, position->y, 0.0f);
	glRotated(t->rotation.w, t->rotation.x, t->rotation.y, t->rotation.z);
	glScaled(t->scale.x, t->scale.y, 1.0f);
	glTranslated(x, y, 0.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p_this->p_vars->img);

	if(p_this->color.w < 0)
		glColor4ub(255, 255, 255, 255);
	else
		glColor4ub(p_this->color.x, p_this->color.y, p_this->color.z, p_this->color.w);
	
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); 
	glVertex2i(0, 0);
	
	glTexCoord2i(0, 1);
	glVertex2i(0, h);
	
	glTexCoord2i(1, 1);
	glVertex2i(w, h);
	
	glTexCoord2i(1, 0);
	glVertex2i(w, 0);
	
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

	View_end();
}

void Object_get_size(const Object* p_this, Vector2D* const p_rtrn)
{
	p_rtrn->x  = p_this->p_vars->info.Width;
	p_rtrn->y =  p_this->p_vars->info.Height;
}

void Object_get_size_x(const Object* p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->info.Width;
}

void Object_get_size_y(const Object* p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->info.Height;
}
