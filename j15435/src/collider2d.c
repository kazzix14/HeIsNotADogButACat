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
#include <math.h>

#include <GL/gl.h>

#include "everytypeneedtohaveapointertothisstructattheheadofthestruct.h"
#include "debug.h"
#include "view.h"
#include "collider2d.h"

static struct everyTypeNeedToHaveAPointerToThisStructAtTheHeadOfTheStruct etnthapttsathots =
{
	.isVisible = true,
	.putFunc = (void*)Collider2D_update
};

struct obb
{
	Vector2D pos;
	Vector2D vecU;
	Vector2D vecR;
};

static Collider2D*** cols;
static unsigned int *colNum;
static char layer_matrix[(COLLIDER2D_LAYER_NUM+1) * COLLIDER2D_LAYER_NUM / 2] = {0}; 
static void addHits(Collider2D* const this, Collider2D* const hit);
static void resetHits(Collider2D* const this);
static bool initialilzed = false;
DDO(
	static void dl(const double a, const double b, const double c, const double d);
	static void dp(const double a, const double b);
	static void dr(const double a, const double b, const double c, const double d, const double e, const double f, const double g, const double h);
)

Collider2D* Collider2D_new()
{
	if(initialilzed == false)
	{
		cols = (Collider2D***)malloc(sizeof(Collider2D**)*COLLIDER2D_LAYER_NUM);
		colNum = (unsigned int*)malloc(sizeof(unsigned int)*COLLIDER2D_LAYER_NUM);
		initialilzed = true;
	}
	Collider2D* col;
	col = (Collider2D*)malloc(sizeof(Collider2D));
	col->pEtnthapttsathots = &etnthapttsathots;
	col->isValid = true;
	resetHits(col);
	return col;
}

// this function have to be placed above Image2D_load somehow
void Collider2D_set_collider_object(Collider2D* const this, char ct)
{
	switch(ct)
	{
		case COLLIDER2D_COLLIDER_POINT:
			this->colObj = (void*)malloc(sizeof(PointCollider));
			*((char*)(this->colObj)) = COLLIDER2D_COLLIDER_POINT;
			break;
		case COLLIDER2D_COLLIDER_CIRCLE:
			this->colObj = (void*)malloc(sizeof(CircleCollider));
			*((char*)(this->colObj)) = COLLIDER2D_COLLIDER_CIRCLE;
			break;
		case COLLIDER2D_COLLIDER_LINE:
			this->colObj = (void*)malloc(sizeof(LineCollider));
			*((char*)(this->colObj)) = COLLIDER2D_COLLIDER_LINE;
			break;
		case COLLIDER2D_COLLIDER_RECT:
			this->colObj = (void*)malloc(sizeof(RectCollider));
			*((char*)(this->colObj)) = COLLIDER2D_COLLIDER_RECT;
			((RectCollider*)(this->colObj))->bb = (struct obb*)malloc(sizeof(struct obb));
			break;
		default:
			DE("invalid collider type\n");
			break;
	}
}

void Collider2D_register_collider(Collider2D* const this, int layer)
{
	if(colNum[layer] == 0)
	{
		cols[layer] = (Collider2D**)malloc(sizeof(Collider2D*));
	}
	else
	{
		cols[layer] = (Collider2D**)realloc(cols[layer], sizeof(Collider2D*)*(colNum[layer]+1));
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
void Collider2D_judge_all()
{
	unsigned int t = 0;
	for(int layerIndexY = 0; layerIndexY < COLLIDER2D_LAYER_NUM; layerIndexY++)
	{
		for(int layerIndexX = 0; layerIndexX < COLLIDER2D_LAYER_NUM - layerIndexY; layerIndexX++)
		{
			for(int i1 = 0; i1 < colNum[COLLIDER2D_LAYER_NUM-1-layerIndexY]; i1++)
			{
				for(int i2 = 0; i2 < colNum[layerIndexX]; i2++)
				{
					resetHits(cols[COLLIDER2D_LAYER_NUM-1-layerIndexY][i1]);
				}
			}
			t++;
		}
	}

	t = 0;

	for(int layerIndexY = 0; layerIndexY < COLLIDER2D_LAYER_NUM; layerIndexY++)
	{
		for(int layerIndexX = 0; layerIndexX < COLLIDER2D_LAYER_NUM - layerIndexY; layerIndexX++)
		{
			if(layer_matrix[t] != 0)
			{
				for(int i1 = 0; i1 < colNum[COLLIDER2D_LAYER_NUM-1-layerIndexY]; i1++)
				{
					for(int i2 = 0; i2 < colNum[layerIndexX]; i2++)
					{

						DPIF(false, "judge %d %d\n", COLLIDER2D_LAYER_NUM-1-layerIndexY, layerIndexX);
						if(cols[COLLIDER2D_LAYER_NUM-1-layerIndexY][i1]->isValid == false) continue;
						else if(cols[layerIndexX][i2]->isValid == false) continue;
						else if((COLLIDER2D_LAYER_NUM-1-layerIndexY) == layerIndexX && i1 == i2) continue;
						Collider2D_judge(cols[COLLIDER2D_LAYER_NUM-1-layerIndexY][i1], cols[layerIndexX][i2]);
					}
				}
			}
			t++;
		}
	}
}
void Collider2D_judge(Collider2D* const c1, Collider2D* const c2)
{
	short typeFlag;
	typeFlag = *((char*)(c1->colObj)) << sizeof(char) | *((char*)(c2->colObj));
	bool hitFlag;
	switch(typeFlag)
	{
		case COLLIDER2D_COLLIDER_RECT << sizeof(char) | COLLIDER2D_COLLIDER_RECT:
			DPIF(false, "judge rect x rect\n");

			/*
			 *	
			 *
			 *
			 *
			 */
			
			RectCollider* rc1 = (RectCollider*)(c1->colObj);
			RectCollider* rc2 = (RectCollider*)(c2->colObj);

			double dist;     // distance between center of the c1 and c2
			double lx1; 	 // col1.right dot vecs
			double ly1; 	 // col1.up    dot vecs
			double lx2; 	 // col2.right dot vecs
			double ly2; 	 // col2.up    dot vecs
			
			Vector2D veci;   // vec interval
			Vector2D vecs;   // separating axis

			hitFlag = true;

			for(int i = 0; i < 4; i++)
			{
				switch(i)
				{
					case 0:
						Vector2D_set(&vecs, &(rc1->bb->vecR));
						break;
					case 1:
						Vector2D_set(&vecs, &(rc1->bb->vecU));
						break;
					case 2:
						Vector2D_set(&vecs, &(rc2->bb->vecR));
						break;
					case 3:
						Vector2D_set(&vecs, &(rc2->bb->vecU));
						break;
				}
				// lx1 + ly1 + lx2 + ly2 < dist : hit

				// veci = pos1 - pos2
				Vector2D_set(&veci, &(rc1->bb->pos));
				Vector2D_sub(&veci, &(rc2->bb->pos));

				// vecs = normalize(vecs)
				Vector2D_normalize(&vecs);

				// dist = veci dot vecs
				Vector2D_dot(&veci, &vecs, &dist);
				Vector2D_dot(&(rc1->bb->vecR), &vecs, &lx1);
				Vector2D_dot(&(rc1->bb->vecU), &vecs, &ly1);
				Vector2D_dot(&(rc2->bb->vecR), &vecs, &lx2);
				Vector2D_dot(&(rc2->bb->vecU), &vecs, &ly2);

				// var = abs(var)
				dist = dist / ((dist >= 0) - (0 > dist));
				lx1  = lx1  / ((lx1  >= 0) - (0 > lx1));
				ly1  = ly1  / ((ly1  >= 0) - (0 > ly1));
				lx2  = lx2  / ((lx2  >= 0) - (0 > lx2));
				ly2  = ly2  / ((ly2  >= 0) - (0 > ly2));
				
				DPIF
				(false,
				 	"dist : %f\n"
				        "lx1 : %f\n"
					"ly1 : %f\n"
					"lx2 : %f\n"
					"ly2 : %f\n\n",
					dist,
					lx1,
					ly1,
					lx2,
					ly2
				);

				if(lx1 + ly1 + lx2 + ly2 < dist)
				{
					hitFlag = false;
					break;
				}
			}

			break;
		default:
			hitFlag = false;
			DE("not implemented\n");
			break;
	}

	if(hitFlag == true)
	{
		DPIF(true, "detect collision : %p x %p\n", c1, c2);
		addHits(c1, c2);
		addHits(c2, c1);
	}
}

void Collider2D_update(Collider2D* const this)
{
	//resetHits(this);
	GLdouble model[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model);

	//double sx = rc->size.x/2.0*model[0] + rc->size.y/2.0*model[4];
	//double sy = rc->size.y/2.0*model[5] + rc->size.x/2.0*model[1];

	/*
	 * --- scale
	 *
	 *    Sx   ,          ,         ,
	 *         ,    Sy    ,         ,
	 *         ,          ,    Sz   ,
	 *         ,          ,         ,
	 *
	 * --- rotation
	 *
	 *  cos z  ,   sin z  ,         ,
	 * -sin z  ,   cos z  ,         ,
	 *         ,          ,         ,
	 *         ,          ,         ,
	 *
	 * --- position
	 *
	 *         ,          ,         ,
	 *         ,          ,         ,
	 *         ,          ,         ,
	 *    Px   ,    Py    ,    Pz   ,
	 */

	DPIF
	( false,
	  "collider : %p\n"
	   "%6.2f, %6.2f, %6.2f, %3.1f\n"
	   "%6.2f, %6.2f, %6.2f, %3.1f\n"
	   "%6.2f, %6.2f, %6.2f, %3.1f\n"
	   "%6.2f, %6.2f, %6.2f, %3.1f\n",
	   this,
	   model[0], model[1], model[2], model[3],
	   model[4], model[5], model[6], model[7],
	   model[8], model[9], model[10],model[11],
	   model[12],model[13],model[14],model[15]
	 );
	
	switch(*((char*)(this->colObj)))
	{
		case COLLIDER2D_COLLIDER_RECT:
			DPIF(false, "rect update\n");

			RectCollider* rc = (RectCollider*)(this->colObj);

			/*
			 * there is a issue that collider will be located at wrong coordiantes
			 * if position is not set to (0, 0)
			 */

			DEIF(Vector2D_compare_zero(&(rc->position)) == 0x5, "wrong position\n");

			// center x : px + width/2*cos - height/2*(-sin)
			// center y : py + height/2*cos - width/2*sin
			double offsetX = rc->size.x/2.0;
			double offsetY = rc->size.y/2.0;
			rc->bb->pos.x = model[12] + offsetX*model[0] + offsetY*model[4];
			rc->bb->pos.y = model[13] + offsetY*model[5] + offsetX*model[1];

			rc->bb->vecR.x = model[0] * offsetX;
			rc->bb->vecR.y = model[1] * offsetX;
			//Vector2D_normalize(&(rc->bb->vecR));

			rc->bb->vecU.x = model[4] * offsetY;
			rc->bb->vecU.y = model[5] * offsetY;
			//Vector2D_normalize(&(rc->bb->vecU));
			
			DPIF(false, "4, 5 : %f, %f\n", rc->bb->vecU.x, rc->bb->vecU.y);

			//
			DDO
			(
			 	//if(this->hits[0] != NULL) glColor3i(1, 0, 0);
				dr
				(	
					rc->position.x, 		rc->position.y,
					rc->position.x+rc->size.x, 	rc->position.y,
					rc->position.x+rc->size.x, 	rc->position.y+rc->size.y,
					rc->position.x, 		rc->position.y+rc->size.y
				);

				glPushMatrix();
					glLoadIdentity();
					dp(rc->bb->pos.x, rc->bb->pos.y);
					dl(rc->bb->pos.x, rc->bb->pos.y, rc->bb->pos.x + rc->bb->vecR.x/2, rc->bb->pos.y + rc->bb->vecR.y/2);
					dl(rc->bb->pos.x, rc->bb->pos.y, rc->bb->pos.x + rc->bb->vecU.x/2, rc->bb->pos.y + rc->bb->vecU.y/2);
				glPopMatrix();
			)
			break;
		default:
			DP("not implemented\n");
			break;
	}
}

void Collider2D_set_layer_matrix(const char lm[COLLIDER2D_LAYER_NUM*COLLIDER2D_LAYER_NUM])
{
	for(int i = 0; i < (COLLIDER2D_LAYER_NUM+1)*COLLIDER2D_LAYER_NUM/2; i++)
	{
		layer_matrix[i] = lm[i];
	}
}

void Collider2D_get_layer_matrix(char lm[COLLIDER2D_LAYER_NUM*COLLIDER2D_LAYER_NUM])
{
	for(int i = 0; i < (COLLIDER2D_LAYER_NUM+1)*COLLIDER2D_LAYER_NUM/2; i++)
	{
		lm[i] = layer_matrix[i];
	}
}

static void addHits(Collider2D* const this, Collider2D* const hit)
{
	for(int i = 0; i < COLLIDER2D_HITS_NUM; i++)
	{
		if(this->hits[i] == hit)
		{
			return;
		}
		else if(this->hits[i] == NULL)
		{
			this->hits[i] = hit;
			return;
		}
	}
	DE("hits overflow\n");
}

static void resetHits(Collider2D* const this)
{
	for(int i = 0; i < COLLIDER2D_HITS_NUM; i++)
	{
		this->hits[i] = NULL;
	}
}
DDO(
	// draw line
	static void dl(const double a, const double b, const double c, const double d)
	{
		glPushMatrix();
			glBegin(GL_LINES);
				glVertex2d(a, b);
				glVertex2d(c, d);
			glEnd();
		glPopMatrix();
	}
	
	// draw point
	static void dp(const double a, const double b)
	{
		glPushMatrix();
			glBegin(GL_POINTS);
				glVertex2d(a, b);
			glEnd();
		glPopMatrix();
	}
	
	// draw rect
	static void dr(const double a, const double b, const double c, const double d, const double e, const double f, const double g, const double h)
	{
		glPushMatrix();
			glBegin(GL_LINE_LOOP);
				glVertex2d(a, b);
				glVertex2d(c, d);
				glVertex2d(e, f);
				glVertex2d(g, h);
			glEnd();
		glPopMatrix();
	}
)
