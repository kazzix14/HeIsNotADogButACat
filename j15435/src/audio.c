/* 
 * audio.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <stdlib.h>
#include <pthread.h>

#include <AL/al.h>
#include <AL/alut.h>

#include "audio.h"

static void* init();
static void* load(void*);
static void* play();

struct private_variables
{
	ALuint *buf,
	       *src;
	unsigned char buf_size;
};

static char is_initialized = 0;
pthread_cond_t cond_audio_play;
pthread_mutex_t mutex_var_audio_load;
pthread_mutex_t mutex_var_audio_play;
pthread_mutex_t mutex_cond_audio_play;
Audio* var_audio_load;
Audio* var_audio_play;

Audio* Audio_new(const unsigned char buf_size)
{
	pthread_t pthread_id;
	if(!is_initialized)
	{
		pthread_create(&pthread_id, NULL, init, NULL);
		pthread_join(pthread_id, NULL);

		pthread_cond_init(&cond_audio_play, NULL);
		pthread_mutex_init(&mutex_var_audio_load, NULL);
		pthread_mutex_init(&mutex_var_audio_play, NULL);
		pthread_mutex_init(&mutex_cond_audio_play, NULL);

		pthread_create(&pthread_id, NULL, play, NULL);

		is_initialized = 1;
	}

	Audio* audio;
	audio = (Audio*)malloc(sizeof(Audio));
	audio->pv = (struct private_variables*)malloc(sizeof(struct private_variables));

	audio->pv->buf_size = buf_size;
	audio->pv->buf = (ALuint*)malloc(sizeof(ALuint) * buf_size);
	audio->pv->src = (ALuint*)malloc(sizeof(ALuint) * buf_size);

	if(audio == NULL || audio->pv == NULL || audio->pv->buf == NULL || audio->pv->src == NULL)
		return NULL;

	return audio;
}

void Audio_release(Audio* const this)
{
	//free(((Audio*)this)->pv);
	//free(this);
}

void Audio_load(Audio* const this, char* const path)
{
	pthread_mutex_lock(&mutex_var_audio_load);
	var_audio_load = this;
	pthread_mutex_unlock(&mutex_var_audio_load);

	pthread_t pthread_id;
	pthread_create(&pthread_id, NULL, load, path);
	pthread_join(pthread_id, NULL);
}

void Audio_play(Audio* const this)
{
		pthread_mutex_lock(&mutex_var_audio_play);
		var_audio_play = this;	
		pthread_mutex_unlock(&mutex_var_audio_play);

		pthread_mutex_lock(&mutex_cond_audio_play);
		pthread_cond_signal(&cond_audio_play);
		pthread_mutex_unlock(&mutex_cond_audio_play);
}

static void* init()
{
	alutInit(0, NULL);
	return NULL;
}

static void* load(void* path)
{
	pthread_mutex_lock(&mutex_var_audio_load);

	for(int i = 0; i < var_audio_load->pv->buf_size; i++)
	{
		var_audio_load->pv->buf[i] = alutCreateBufferFromFile(path);
		alGenSources (1, &(var_audio_load->pv->src[i]));
		alSourcei(var_audio_load->pv->src[i], AL_BUFFER, var_audio_load->pv->buf[i]);
	}

	pthread_mutex_unlock(&mutex_var_audio_load);
	return NULL;
}

static void* play()
{
	while(1)
	{
		pthread_mutex_lock(&mutex_cond_audio_play);
		pthread_cond_wait(&cond_audio_play, &mutex_cond_audio_play);
		pthread_mutex_unlock(&mutex_cond_audio_play);

		pthread_mutex_lock(&mutex_var_audio_play);
		ALint state;
		for(int i = 0; i < var_audio_play->pv->buf_size; i++)
		{
			alGetSourcei(var_audio_play->pv->src[i], AL_SOURCE_STATE, &state);
			if(state != AL_PLAYING)
			{
				alSourcePlay(var_audio_play->pv->src[i]);
				break;
			}
		}
		pthread_mutex_unlock(&mutex_var_audio_play);
	}
	return NULL;
}