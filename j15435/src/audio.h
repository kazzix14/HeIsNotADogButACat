/* 
 * audio.h
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
#ifndef AUDIO_H
#define AUDIO_H

// audi -> 97 117 100 105
#define AUDIO_TYPE_ID 97117115
 
struct private_variables;
typedef struct audio
{
	struct private_variables* pv;	
} Audio;

Audio* Audio_new(const unsigned char);
void Audio_release(Audio* const);
void Audio_load(Audio* const, char* cosnt);
void Audio_play(Audio* const);

#endif
