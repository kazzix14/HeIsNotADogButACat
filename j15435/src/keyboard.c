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

static bool keydownflag[sizeof(char)];

void Keyboard_init()
{
	glutKeyboardFunc(down);
	glutKeyboardUpFunc(up);
	for(int i = 0; i < sizeof(char); i++)
		keydownflag[i] = false;
}

bool Keyboard_is_pressed(unsigned char key)
{
	return keydownflag[key];
}

static void down(unsigned char key, int x, int y)
{
	if('A' <= key && key <= 'Z')
		// 'a' - 'A' == 32
		keydownflag[key + 32] = 1;
	else
		keydownflag[key] = 1;
	DP("Key %d is pressed\n", key);
	DP("ESC is %s\n", keydownflag[27] ? "pressed" : "not pressed");
}

static void up(unsigned char key, int x, int y)
{
	if('A' <= key && key <= 'Z')
		// 'a' - 'A' == 32
		keydownflag[key + 32] = 0;
	else
		keydownflag[key] = 0;
	DP("Key %d is released\n", key);
	DP("ESC is %s\n", keydownflag[27] ? "pressed" : "not pressed");
}
