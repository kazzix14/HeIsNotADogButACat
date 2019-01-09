/* 
 * keyboard.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

void Keyboard_init();
bool Keyboard_is_pressed(unsigned char);
#endif
