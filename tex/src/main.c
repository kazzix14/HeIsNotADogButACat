/* 
 * main.c
 *
 * (C) 2018 Kazuma Murata
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "transform2d.h"
#include "image2d.h"
#include "vector2d.h"
#include "view.h"

void display(void);
void reshape(int, int);
void timer(int);
void change_color();
void change_backcolor();
void getWindowSize(int *x, int *y);
void put_num();
void set_num_pos(int, int, double, double);
void init_image();
void init_gl(int*, char**, GLuint, GLuint, char*);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);
Image2D* p_num[10];
Image2D* p_hand[3];
Vector2D num_pos[12];

View* view;

bool mouse_down;
Vector4D col,
	 bcol;

int main(int argc, char **argv)
{
	mouse_down = false;
	Vector4D_set_zero(&col);
	init_gl(&argc, argv, 400, 400, "Handwritten Clock");
	init_image();
	view = View_new();

	glutTimerFunc(100, timer, 0);
	glutMainLoop();

	return(0);

}

void display(void)
{
	time_t tt;
	time(&tt);
	struct tm *tms;
	tms = localtime(&tt);
	
	int ww, wh;
	getWindowSize(&ww, &wh);

	glClear(GL_COLOR_BUFFER_BIT);
	put_num();

	p_hand[0]->p_transform->rotation.w = (double)tms->tm_hour/12.0*360.0 +
						(double)tms->tm_min / 60.0 * 6.0 +
						(double)tms->tm_sec / 60.0 * 0.1;
	p_hand[1]->p_transform->rotation.w = (double)tms->tm_min/60.0*360.0 +
						(double)tms->tm_sec / 60.0 * 6.0;
	p_hand[2]->p_transform->rotation.w = (double)tms->tm_sec/60.0*360.0;

	for(int i = 0; i < 3; i++)
		Image2D_put(p_hand[i], view);

	glFlush();
	glutSwapBuffers();
}

void timer(int value)
{
	glutTimerFunc(100, timer, 0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	int min = w < h ? w : h;
	glViewport(0, 0, w, h);
	view->screen_width = w;
	view->screen_height = h;

	for(int i = 0; i < 3; i++)
	{
		p_hand[i]->p_transform->scale.x = 0.3 * (double)min/400.0;
		p_hand[i]->p_transform->scale.y = (-0.2-0.08*i) * (double)min/400.0;
	}
	for(int i = 0; i < 10; i++)
	{
		p_num[i]->p_transform->scale.x = 0.1 * (double)min/400.0;
		p_num[i]->p_transform->scale.y = 0.1 * (double)min/400.0;
	}
	set_num_pos(w/2, h/2, min/2-min/10, 0);

	p_hand[0]->p_transform->position.x = w/2;
	p_hand[0]->p_transform->position.y = h/2;
	p_hand[1]->p_transform->position.x = w/2;
	p_hand[1]->p_transform->position.y = h/2;
	p_hand[2]->p_transform->position.x = w/2;
	p_hand[2]->p_transform->position.y = h/2;
}

void change_color()
{
	for(int i = 0; i < 10; i++)
	{
		Vector4D_set(&(p_num[i]->color), &col);
	}
	for(int i = 0; i < 3; i++)
	{
		Vector4D_set(&(p_hand[i]->color), &col);
	}
}

void change_backcolor()
{
	glClearColor((double)bcol.x/255.0, (double)bcol.y/255.0, (double)bcol.z/255.0, 1);
}

void getWindowSize(int *x, int *y)
{
	*x = glutGet(GLUT_WINDOW_WIDTH);
	*y = glutGet(GLUT_WINDOW_HEIGHT);
}

void put_num()
{
	int w1, w0;
	Vector2D vec1;
	Vector2D vec0;
	for(int i = 0; i < 12; i++)
	{
		switch(i)
		{
			case 9:
				Image2D_get_size_x(p_num[1], &w1);
				Image2D_get_size_x(p_num[0], &w0);
				Vector2D_set(&vec1, &num_pos[9]);
				Vector2D_set(&vec0, &num_pos[9]);
				vec1.x -= w1/2*p_num[1]->p_transform->scale.x;
				vec0.x += w0/2*p_num[0]->p_transform->scale.x;
				Image2D_put_at(p_num[1], view, &vec1);
				Image2D_put_at(p_num[0], view, &vec0);
				break;
			case 10:
				Image2D_get_size_x(p_num[1], &w1);
				Image2D_get_size_x(p_num[1], &w0);
				Vector2D_set(&vec1, &num_pos[10]);
				Vector2D_set(&vec0, &num_pos[10]);
				vec1.x -= w1/2*p_num[1]->p_transform->scale.x;
				vec0.x += w0/2*p_num[1]->p_transform->scale.x;
				Image2D_put_at(p_num[1], view, &vec1);
				Image2D_put_at(p_num[1], view, &vec0);
				break;
			case 11:
				Image2D_get_size_x(p_num[1], &w1);
				Image2D_get_size_x(p_num[2], &w0);
				Vector2D_set(&vec1, &num_pos[11]);
				Vector2D_set(&vec0, &num_pos[11]);
				vec1.x -= w1/2*p_num[1]->p_transform->scale.x;
				vec0.x += w0/2*p_num[2]->p_transform->scale.x;
				Image2D_put_at(p_num[1], view, &vec1);
				Image2D_put_at(p_num[2], view, &vec0);
				break;
			default:
				Image2D_put_at(p_num[i+1], view, &num_pos[i]);
				break;	
		}
	}
}

// make circle with numbers
void set_num_pos(int x, int y, double radius, double rotation)
{
	// pi * 2 / 12 = pi / 6
	double a = M_PI / 6;
	for(int i = 0; i < 12; i++)
	{
		num_pos[i].x = x + sin(a + rotation + a * (double)i) * radius;
		num_pos[i].y = y - cos(a + rotation + a * (double)i) * radius;
	}
}

void init_gl(int *argc, char **argv, GLuint width, GLuint height, char *title)
{
	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
	glutPositionWindow(100, 100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GL_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
}

void init_image()
{
	// init hands
	for(int i = 0; i < 3; i++)
	{
		p_hand[i] = Image2D_new();

		char path[30];
		sprintf(path, "resource/image/hand/%d.png", i);
		Image2D_load(p_hand[i], path);

		p_hand[i]->option = IMAGE2D_CENTER;
		Transform2D_set_default(p_hand[i]->p_transform);
		p_hand[i]->p_transform->rotation.z = 1;
		p_hand[i]->p_transform->scale.x = 0.3;
		p_hand[i]->p_transform->scale.y = -0.2-0.08*i;

		p_hand[i]->color.w = 255;

	}

	// init numbers
	for(int i = 0; i < 10; i++)
	{
		p_num[i] = Image2D_new();

		char path[30];
		sprintf(path, "resource/image/number/%d.png", i);
		Image2D_load(p_num[i], path);

		p_num[i]->option = IMAGE2D_CENTER;
		p_num[i]->p_transform->scale.x = 0.1;
		p_num[i]->p_transform->scale.y = 0.1;
		Vector4D_set_zero(&(p_num[i]->p_transform->rotation));

		p_num[i]->color.w = 255;
	}
}

void mouse(int b, int s, int x, int y)
{
	if(b == GLUT_LEFT_BUTTON)
	{
		if(s == GLUT_DOWN)	
		{
			col.x = (double)x / (double)view->screen_width *255.0,
			col.y = (double)y / (double)view->screen_height *255.0;
		}
		else if(s == GLUT_UP)	
		{
			col.z = (double)x / (double)view->screen_width *255.0,
			col.w = (double)y / (double)view->screen_height *255.0;
			change_color();
		}
	}
	else if(b == GLUT_RIGHT_BUTTON)
	{
		if(s == GLUT_DOWN)	
		{
			bcol.x = (double)x / (double)view->screen_width *255.0,
			bcol.y = (double)y / (double)view->screen_height *255.0;
		}
		else if(s == GLUT_UP)	
		{
			bcol.z = (double)x / (double)view->screen_width *255.0,
			bcol.w = (double)y / (double)view->screen_height *255.0;
			change_backcolor();
		}
	}
	else if(b == GLUT_MIDDLE_BUTTON)
		exit(0);
}

void keyboard(unsigned char key, int x, int y)
{
	//q esc
	if((key == 'q') || (key == 27))
		exit(0);
}
