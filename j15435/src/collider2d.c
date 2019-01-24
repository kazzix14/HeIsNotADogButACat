/* 
 * collider2d.c
 *
 * (C) 2019 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdlib.h>

#include "collider2d.h"

static Collider2D*** cols;
static unsigned int *colNum;
static char layer_matrix[(COLLIDER2D_LAYER_BYTES*8) * COLLIDER2D_LAYER_BYTES*8] = {0}; // 8 bit * 4 : 32 layer

Collider2D* Collider2D_new()
{
	Collider2D* col;
	col = (Collider2D*)malloc(sizeof(Collider2D));
	return col;
}

void Collider2D_set_collider_object(Collider2D* const this, char ct)
{
	switch(ct)
	{
		case COLLIDER2D_COLLIDER_POINT:
			this->colObj = (void*)malloc(sizeof(PointCollider));
			break;
		case COLLIDER2D_COLLIDER_CIRCLE:
			this->colObj = (void*)malloc(sizeof(CircleCollider));
			break;
		case COLLIDER2D_COLLIDER_LINE:
			this->colObj = (void*)malloc(sizeof(LineCollider));
			break;
		case COLLIDER2D_COLLIDER_RECT:
			this->colObj = (void*)malloc(sizeof(RectCollider));
			break;
	}
}

void Collider2D_register_collider(Collider2D* const this, int layer)
{
	if(colNum == 0)
	{
		cols = (Collider2D***)malloc(sizeof(Collider2D**)*COLLIDER2D_LAYER_BYTES*8);
		colNum = (unsigned int*)malloc(sizeof(unsigned int)*COLLIDER2D_LAYER_BYTES*8);
		for(int i = 0; i < COLLIDER2D_LAYER_BYTES*8; i++)
		{
			cols[i] = (Collider2D**)malloc(sizeof(Collider2D*));
		}
	}
	else
	{
		for(int i = 0; i < COLLIDER2D_LAYER_BYTES*8; i++)
		{
			cols[i] = (Collider2D**)realloc(cols[i], sizeof(Collider2D*)*(colNum[i]+1));
		}
	}
	cols[layer][colNum[layer]] = this;
	colNum[layer]++;
}

void Colldier2D_judge()
{
	for(int layerIndexY = 0; layerIndexY < COLLIDER2D_LAYER_BYTES*8; layerIndexY++)
	{
		for(int layerIndexX = 0; layerIndexX < COLLIDER2D_LAYER_BYTES*8; layerIndexX++)
		{
			if(layer_matrix[layerIndexY*8*COLLIDER2D_LAYER_BYTES+layerIndexX] != 0)
			{
				for(int i = 0; i < colNum[layerIndexY; i++)
				{
				cols[layerIndexY]
				}
			}
		}
	}
}

void Colldier2D_set_layer_matrix(const char lm[COLLIDER2D_LAYER_BYTES*COLLIDER2D_LAYER_BYTES*8*8])
{
	for(int i = 0; i < COLLIDER2D_LAYER_BYTES*COLLIDER2D_LAYER_BYTES*8*8; i++)
	{
		layer_matrix[i] = lm[i];
	}
}

void Colldier2D_get_layer_matrix(char lm[COLLIDER2D_LAYER_BYTES*COLLIDER2D_LAYER_BYTES*8*8])
{
	for(int i = 0; i < COLLIDER2D_LAYER_BYTES*COLLIDER2D_LAYER_BYTES*8*8; i++)
	{
		lm[i] = layer_matrix[i];
	}
}
