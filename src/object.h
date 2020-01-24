/* 
 * object.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef OBJECT_H
#define OBJECT_H

// obj -> 111 098 106 -> 1119816
#define OBJECT_TYPE_ID 1119816

#include <stdbool.h>

#include "everytypeneedtohaveapointertothisstructattheheadofthestruct.h"
#include "transform2d.h"

struct private_variables;
typedef struct object
{
	struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct* pEtnthapttsathots;
	Transform2D* transform;
	struct private_variables* pv;	
} Object;

Object* Object_new();
void Object_add_component(Object* const, void* const);
void Object_remove_component(Object* const, void* const);
void Object_clear_component(Object* const);
void Object_set_valid(Object* const);
void Object_set_invalid(Object* const);
void Object_is_valid(Object* const, bool* const);
void Object_put(const Object*);
void Object_play_AnimationController2D(const Object*);
void Object_release(Object* const);

#endif
