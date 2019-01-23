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

#include "everytypeneedtohaveapointertothisstructattheheadofthestruct.h"
#include "debug.h"
#include "transform2d.h"
#include "image2d.h"

static struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct etnthapttsathots =
{
	.isVisible = true,
	.putFunc = (void*)Image2D_put
};

struct private_variables
{
	GLuint img;
	pngInfo info;
};

Image2D* Image2D_new()
{
	// allocate memories
	Image2D* p_img;
	p_img = (Image2D*)malloc(sizeof(Image2D));
	p_img->p_vars = (struct private_variables*)malloc(sizeof(struct private_variables));
	p_img->p_transform = Transform2D_new();
	p_img->pEtnthapttsathots = &etnthapttsathots;

	p_img->option = IMAGE2D_TOP_LEFT;
	p_img->color.x = 255;
	p_img->color.y = 255;
	p_img->color.z = 255;
	p_img->color.w = -1;

	if(p_img == NULL || p_img->p_vars == NULL || p_img->p_transform == NULL)
		return NULL;

	return p_img;
}

void Image2D_release(Image2D* const p_this)
{
	free(p_this->p_vars);
	free(p_this->p_transform);
	free(p_this);
}

void Image2D_load(Image2D* const this, const char* path)
{
	this->p_vars->img = pngBind(path, 
					PNG_NOMIPMAP,
					PNG_ALPHA,
					&(this->p_vars->info),
					GL_CLAMP,
					GL_NEAREST,
					GL_NEAREST);
	DEIF(this->p_vars->img == 0,"failed to load image: \"%s\"\n", path);
}

void Image2D_put(const Image2D* p_this)
{
	Image2D_put_at(p_this, &(p_this->p_transform->position));	
}

void Image2D_put_with_offset(const Image2D* p_this, const Vector2D* p_offset)
{
	Image2D_put_at(p_this, &(p_this->p_transform->position) );	
}

void Image2D_put_at(const Image2D* p_this, const Vector2D* position)
{
	int w = p_this->p_vars->info.Width,
	    h = p_this->p_vars->info.Height;

	// offset
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

	//View_begin_2d(view);

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

	//View_end();
}

void Image2D_get_size(const Image2D* p_this, Vector2D* const p_rtrn)
{
	p_rtrn->x  = p_this->p_vars->info.Width;
	p_rtrn->y =  p_this->p_vars->info.Height;
}

void Image2D_get_width(const Image2D* p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->info.Width;
}

void Image2D_get_height(const Image2D* p_this, int* const p_rtrn)
{
	*p_rtrn = p_this->p_vars->info.Height;
}
