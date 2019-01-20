/* 
 * image2d.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <sys/time.h>

#include "debug.h"
#include "timer.h"

struct private_variables
{
	int count;
	double ts;
	double tc;
};

Timer* Timer_new()
{
	// allocate memories
	return NULL;
}

// second per frame
void Timer_get_spf(double* const ret)
{
	static double ts;
	static double tc;
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	tc = (double)tv.tv_sec + (double)tv.tv_usec * 1.e-6;
	*ret = tc - ts;
	ts = tc;
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
