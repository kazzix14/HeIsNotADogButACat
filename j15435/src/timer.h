/* 
 * timer.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef TIMER_H
#define TIMER_H

// tmr ->  116 109 114 -> 116 19 114
#define TIMER_TYPE_ID 11619114
 
struct private_variables;
typedef struct timer
{
	struct private_variables* pv;	
} Timer;

Timer* Timer_new();
void Timer_count(Timer* const);
void Timer_get_count(Timer* const, double* const);
void Timer_reset_count(Timer* const);
void Timer_set_count(Timer* const, const double);
void Timer_get_spf(Timer* const, double* const);
void Timer_get_fps();

#endif
