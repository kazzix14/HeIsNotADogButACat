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

static void release_images(Animation2D* const, const int, const int);

struct private_variables
{
	unsigned int anim_length;
	unsigned int current_frame;
	Animation2D** P_imgs;
};

Animation2D* Animation2D_new()
{
	// allocate memories
	Animation2D* p_anim;
	p_anim = (Animation2D*)malloc(sizeof(Animation2D));
	p_anim->p_vars = (struct private_variables*)malloc(sizeof(struct private_variables));
	p_anim->p_transform = Transform2D_new();

	if(p_anim == NULL || p_anim->p_vars == NULL || p_anim->p_transform == NULL)
		return NULL;

	return p_anim;
}

Animation2D* Animation2D_load(const Animation2D* p_this, const char* path_dir, const int num)
{
	if(num < p_this->anim_length)
		release_images(p_this, num, p_this->p_vars->anim_length);

	p_this->p_vars->anim_length = num;
	char path[ANIMATION2D_LOAD_PATH_LIMIT];
	for (int i = 0; i < num; i++)
	{
		if(p_this->p_vars->p_imgs[i] = NULL)
			p_this->p_vars->p_img[i] = Image2D_new();
		snprintf(path, ANIMATION2D_LOAD_PATH_LIMIT, "%s/%d", path_dir, i);
		Image2D_load((p_this->p_vars->p_imgs[i], path);
	}
}

static void release_images(Animation2D* const p_this, const int form, const int to)
{
	for(int i = from; i < to;i++)
		Image2D_release(p_this->p_vars->p_imgs[i]);
}

void Animation2D_release(Animation2D* const p_this)
{
	release_images(p_this, 0, p_this->p_vars->anim_length);
	free(p_this->p_vars);
	free(p_this->p_transform);
	free(p_this);
}

void Animation2D_put(const Animation2D* p_this, const View* view)
{
	Image2D_put(p_this, view, &(p_this->p_transform->position));	
}

void Animation2D_put_at(const Animation2D* p_this, const View* view, const Vector2D* position)
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

void Animation3D_get_size(const Animation2D* p_this, Vector2D* const p_rtrn)
{
	p_rtrn->x  = p_this->p_vars->info.Width;
	p_rtrn->y =  p_this->p_vars->info.Height;
}

void Animation2D_get_size_x(const Animation2D* p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->info.Width;
}

void Animation2D_get_size_y(const Animation2D* p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->info.Height;
}
