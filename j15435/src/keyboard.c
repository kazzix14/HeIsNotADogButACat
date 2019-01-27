/* * keyboard.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <stdbool.h>

#include <GL/glut.h>

#include "keyboard.h"
#include "debug.h"

static void down(unsigned char, int, int);
static void up(unsigned char, int, int);

static bool keydownflag[128] = {0};
static char keypressed[32] ;
static int keypressedhead = 0;

void Keyboard_init()
{
	glutKeyboardFunc(down);
	glutKeyboardUpFunc(up);
}

bool Keyboard_is_pressed(unsigned char key)
{
	for(int i = 0; i < 32; i++)
	{
		if(keypressed[i] == key) return true;
	}
	return false;
}

bool Keyboard_is_down(unsigned char key)
{
	return keydownflag[key];
}

void Keyboard_update()
{
	for(int i = 0; i < 32; i++)
	{
		keypressed[i] = -1;
	}
	keypressedhead = 0;
}

static void down(unsigned char key, int x, int y)
{
	if('A' <= key && key <= 'Z')
	{
		// 'a' - 'A' == 32
		if(keydownflag[key] == 0)
		{
			DPIF(true, "Key %d is pressed\n", key);
			keypressed[keypressedhead++] = key+32;
			keydownflag[key+32] = 1;
		}
	}
	else
	{
		if(keydownflag[key] == 0)
		{
			DPIF(true, "Key %d is pressed\n", key);
			keypressed[keypressedhead++] = key;
			keydownflag[key] = 1;
		}
	}

}

static void up(unsigned char key, int x, int y)
{
	if('A' <= key && key <= 'Z')
	{
		// 'a' - 'A' == 32
		if(keydownflag[key] == 1)
		{
			DPIF(true, "Key %d is released\n", key);
			keydownflag[key+32] = 0;
		}
	}
	else
	{
		if(keydownflag[key] == 1)
		{
			DPIF(true, "Key %d is released\n", key);
			keydownflag[key] = 0;
		}
	}

}
