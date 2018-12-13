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
void reshape(int w, int h);
void timer(int value);
void getWindowSize(int *x, int *y);
void put_num();
void set_num_pos(int, int, double, double);
void init_image();
void init_gl(int *argc, char **argv, GLuint width, GLuint height, char *title);

Image2D* p_num[10];
Image2D* p_hand[3];
Vector2D num_pos[12];

pngInfo info1;
View* view;

int main(int argc, char **argv)
{
	init_gl(&argc, argv, 900, 600, "hello");
	init_image();
	view = View_new();

	glutTimerFunc(20, timer, 0);
	glutMainLoop();

	return(0);

}

void display(void)
{
	time_t tt;
	time(&tt);
	struct tm *tms;
	tms = localtime(&tt);
  	
	//printf("%d/%d/%d ", tms->tm_mday, tms->tm_mon, tms->tm_year);
  	//printf("%d:%d:%d\n", tms->tm_hour, tms->tm_min, tms->tm_sec);

	int ww, wh;
	getWindowSize(&ww, &wh);

	glClear(GL_COLOR_BUFFER_BIT);

	//for(int i = 0; i < 10; i++)
	//	Image2D_put(p_num[i], p_view);
	set_num_pos(500, 500, 150, (double)tms->tm_sec/30*M_PI);
	put_num();

	//hand
	p_hand[2]->p_transform->position.x = ww/2;
	p_hand[2]->p_transform->position.y = wh/2;
	//p_hand[2]->p_transform->rotation.w = (double)tms->tm_sec/30*M_PI;
	Image2D_put(p_hand[2], view);

//
	View_begin_2d(view);

	glBegin(GL_LINES);
	glVertex2f(ww/2, wh/2);
	glVertex2f(ww/2 + 70*sin((float)tms->tm_sec/30.0*M_PI), wh/2 - 70*cos((float)tms->tm_sec/30.0*M_PI));
	glEnd();

	View_end();
//	
	glFlush();
	glutSwapBuffers();
}

void timer(int value)
{
	glutTimerFunc(500, timer, 0);
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	view->screen_width = w;
	view->screen_height = h;
}

void getWindowSize(int *x, int *y)
{
	*x = glutGet(GLUT_WINDOW_WIDTH);
	*y = glutGet(GLUT_WINDOW_HEIGHT);
}

void put_num()
{
	for(int i = 0; i < 12; i++)
	{
		switch(i)
		{
			case 9:
				Image2D_put_at(p_num[0], view, &num_pos[9]);
				break;
			case 10:
				Image2D_put_at(p_num[1], view, &num_pos[10]);
				break;
			case 11:
				Image2D_put_at(p_num[2], view, &num_pos[11]);
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//glEnable(GL_BLEND | GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
}

void init_image()
{
	p_hand[0] = Image2D_new();
	p_hand[1] = Image2D_new();
	p_hand[2] = Image2D_new();

	Image2D_load(p_hand[0], "resource/image/hand/short.png");
	Image2D_load(p_hand[1], "resource/image/hand/medium.png");
	Image2D_load(p_hand[2], "resource/image/hand/long.png");

	p_hand[0]->option = IMAGE2D_CENTER;
	p_hand[1]->option = IMAGE2D_CENTER;
	p_hand[2]->option = IMAGE2D_CENTER;
	
	Transform2D_set_default(p_hand[0]->p_transform);
	Transform2D_set_default(p_hand[1]->p_transform);
	Transform2D_set_default(p_hand[2]->p_transform);

	p_hand[0]->p_transform->rotation.z = 1;
	p_hand[1]->p_transform->rotation.z = 1;
	p_hand[2]->p_transform->rotation.z = 1;

	//load numbers
	for(int i = 0; i < 10; i++)
	{
		// construct
		p_num[i] = Image2D_new();

		// load image
		char path[30];
		sprintf(path, "resource/image/number/%d.png", i);
		Image2D_load(p_num[i], path);
		p_num[i]->option = IMAGE2D_CENTER;

		// init position
		p_num[i]->p_transform->position.x = 500.0;
		p_num[i]->p_transform->position.y = 500.0;

		// init scale
		p_num[i]->p_transform->scale.x = 0.1;
		p_num[i]->p_transform->scale.y = 0.1;

		// init rotation
		Vector4D_set_zero(&(p_num[i]->p_transform->rotation));
	}
}
