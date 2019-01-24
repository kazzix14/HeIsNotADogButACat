/* 
 * collider2d.c
 *
 * (C) 2019 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdlib.h>

#include "collider2d.h"

Collider2D* Collider2D_new()
{
	Collider2D* coll;
	coll = (Collider2D*)malloc(sizeof(Collider2D));
	return coll;
}

void Collider2D_set_collider_object(Collider2D* const this, char ct)
{
	switch(ct)
	{
		case COLLIDER2D_COLLIDER_TYPE_POINT:
			this->collObj = (void*)malloc(sizeof(Point));
			break;
		case COLLIDER2D_COLLIDER_TYPE_CIRCLE:
			this->collObj = (void*)malloc(sizeof(Circle));
			break;
		case COLLIDER2D_COLLIDER_TYPE_LINE:
			this->collObj = (void*)malloc(sizeof(Line));
			break;
		case COLLIDER2D_COLLIDER_TYPE_RECT:
			this->collObj = (void*)malloc(sizeof(Rect));
			break;
	}
}

void Collider2D_register_collider_object(Collider2D* const this)
{

}
