/* 
 * everytypeneedtohaveapointertothisstructattheheadofthestruct.h
 *
 * (C) 2019 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef EVERYTYPENEEDTOHAVEAPOINTERTOTHISSTRUCTATTHEHEADOFTHESTRUCT_H
#define EVERYTYPENEEDTOHAVEAPOINTERTOTHISSTRUCTATTHEHEADOFTHESTRUCT_H

#include <stdbool.h>
 
struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct
{
	bool isVisible;
	void (*putFunc)(void*);
};

#endif
