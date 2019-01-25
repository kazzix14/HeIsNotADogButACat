/* 
 * collider2d.c
 *
 * (C) 2019 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdlib.h>
#include <stdbool.h>

#include "everytypeneedtohaveapointertothisstructattheheadofthestruct.h"
#include "debug.h"
#include "collider2d.h"

static struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct etnthapttsathots =
{
	.isVisible = false,
	.putFunc = NULL
};

static Collider2D*** cols;
static unsigned int *colNum;
static char layer_matrix[(COLLIDER2D_LAYER_NUM+1) * COLLIDER2D_LAYER_NUM / 2] = {0}; 

Collider2D* Collider2D_new()
{
	Collider2D* col;
	col = (Collider2D*)malloc(sizeof(Collider2D));
	col->pEtnthapttsathots = &etnthapttsathots;
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
		default:
			DE("invalid collider type\n");
			break;
	}
}

void Collider2D_register_collider(Collider2D* const this, int layer)
{
	if(colNum == 0)
	{
		cols = (Collider2D***)malloc(sizeof(Collider2D**)*COLLIDER2D_LAYER_NUM);
		colNum = (unsigned int*)malloc(sizeof(unsigned int)*COLLIDER2D_LAYER_NUM);
		for(int i = 0; i < COLLIDER2D_LAYER_NUM; i++)
		{
			cols[i] = (Collider2D**)malloc(sizeof(Collider2D*));
		}
	}
	else
	{
		for(int i = 0; i < COLLIDER2D_LAYER_NUM; i++)
		{
			cols[i] = (Collider2D**)realloc(cols[i], sizeof(Collider2D*)*(colNum[i]+1));
		}
	}
	cols[layer][colNum[layer]] = this;
	colNum[layer]++;
}

/*
 *   layernum = 5
 *
 *	l0 l1 l2 l3 l4
 *   l4	0  1  2  3  4
 *   l3	5  6  7  8
 *   l2	9  10 11
 *   l1	12 13
 *   l0	14
 */
void Colldier2D_judge_all()
{
	unsigned char t;
	for(int layerIndexY = 0; layerIndexY < COLLIDER2D_LAYER_NUM; layerIndexY++)
	{
		for(int layerIndexX = 0; layerIndexX < COLLIDER2D_LAYER_NUM - layerIndexY; layerIndexX++)
		{
			if(layer_matrix[t] != 0)
			{
				for(int i1 = 0; i1 < colNum[layerIndexY]; i1++)
				{
					for(int i2 = 0; i2 < colNum[COLLIDER2D_LAYER_NUM-1-layerIndexX]; i2++)
					{
						Collider2D_judge(cols[layerIndexY][i1], cols[COLLIDER2D_LAYER_NUM-1-layerIndexX][i2]);
					}
				}
			}
			t++;
		}
	}
}
void Collider2D_judge(Collider2D* const c1, Collider2D* const c2)
{
	short flag;
	flag = *((char*)(c1->colObj)) << sizeof(char) | *((char*)(c2->colObj));
	switch(flag)
	{
		case COLLIDER2D_COLLIDER_RECT << sizeof(char) | COLLIDER2D_COLLIDER_RECT:
			DP("judge rect x rect\n");
			break;
		default:
			DP("not implemented\n");
			break;
	}
}

void Colldier2D_set_layer_matrix(const char lm[COLLIDER2D_LAYER_NUM*COLLIDER2D_LAYER_NUM])
{
	for(int i = 0; i < COLLIDER2D_LAYER_NUM*COLLIDER2D_LAYER_NUM; i++)
	{
		layer_matrix[i] = lm[i];
	}
}

void Colldier2D_get_layer_matrix(char lm[COLLIDER2D_LAYER_NUM*COLLIDER2D_LAYER_NUM])
{
	for(int i = 0; i < COLLIDER2D_LAYER_NUM*COLLIDER2D_LAYER_NUM; i++)
	{
		lm[i] = layer_matrix[i];
	}
}
