/* 
 * view.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef VIEW_H
#define VIEW_H

#include "vector3d.h"

struct private_variables;
typedef struct view
{
	Vector3D position,
		 traget_position,
		 up_vetor;
	int screen_width,
	    screen_height;
	struct private_variables* p_vars;	
} View;

View* View_new();
void View_release(View* const);
void View_begin_2d(const View*);
void View_begin_3d(const View*);
void View_end();
#endif
