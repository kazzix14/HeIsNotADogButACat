/* 
 * image2d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <stdlib.h>
#include <sys/time.h>

#include "debug.h"
#include "timer.h"

static void getSpf(Timer* const this, double* const ret);

struct private_variables
{
	double count;
	double ts;
	double tc;
	double spf;
};

Timer* Timer_new()
{
	// allocate memories
	Timer* t;
	t = (Timer*)malloc(sizeof(Timer));
	t->pv = (struct private_variables*)malloc(sizeof(struct private_variables));
	t->pv->count = 0.0; 

	double ret;
	getSpf(t, &ret);

	return t;
}

// call this function every frame
void Timer_count(Timer* const this)
{
	double r;
	getSpf(this, &r);
	this->pv->count += r;
	this->pv->spf = r;
}

void Timer_get_count(Timer* const this, double* const ret)
{
	*ret = this->pv->count;
}

void Timer_reset_count(Timer* const this)
{
	this->pv->count = 0.0;
}

void Timer_set_count(Timer* const this, const double count)
{
	this->pv->count = count;
}

// second per frame
void Timer_get_spf(Timer* const this, double* const ret)
{
	*ret = this->pv->spf;
}

void Timer_get_fps()
{
	static int count=0;
	static double ts=0;
	static double tc=0;
	static struct timeval tv;

	count++;
	gettimeofday(&tv, NULL);
	tc = (double)tv.tv_sec + (double)tv.tv_usec * 1.e-6;
	if(tc - ts >= 1.0)
	{
		DP("fps:%2d\n", count);
		ts = tc;
		count = 0;
	}
}

static void getSpf(Timer* const this, double* const ret)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	this->pv->tc = (double)tv.tv_sec + (double)tv.tv_usec * 1.e-6;
	*ret = this->pv->tc - this->pv->ts;
	this->pv->ts = this->pv->tc;
}
