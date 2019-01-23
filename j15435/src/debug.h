/* * debug.h
 *
 * (C) 2019 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
	#define DP(fmt, args...) fprintf(stdout, fmt, ##args)
	#define DPIF(condition, fmt, args...) if(condition)fprintf(stdout, fmt, ##args)
	#define DE(fmt, args...) fprintf(stderr, "%s:%d:%s():" fmt, __FILE__, __LINE__, __func__, ##args)
	#define DEIF(condition, fmt, args...) if(condition)fprintf(stderr, "%s:%d:%s():" fmt, __FILE__, __LINE__, __func__, ##args)
	#define DDO(code) code
#else
	#define DP(fmt, args...)
	#define DPIF(fmt, args...)
	#define DE(fmt, args...)
	#define DEIF(fmt, args...)
	#define DDO(code)
#endif

#endif
