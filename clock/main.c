#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "component2d.h"
#include "image2d.h"
#include "vector2d.h"
#include "view.h"

void display(void);
void reshape(int w, int h);
void timer(int value);
void getWindowSize(int *x, int *y);
void init(int *argc, char **argv, GLuint width, GLuint height, char *title);

Image2D img;
pngInfo info1;

double vertices[][3]={
	{0.0, 10.0, 0.0},
	{0.0, 20.0, 50.0},
	{50.0, 50.0, 0.0},
	{50.0, 0.0, 0.0}
};

int main(int argc, char **argv)
{
	init(&argc, argv, 900, 600, "hello");

	Image2D_construct(&img, "image.png");
	Vector2D impos;
	Vector2D_construct(&impos);
	Vector2D_set(&impos, 20, 20);
	Component2D_set_position(&(img.parent), &impos);

	glutTimerFunc(500, timer, 0);
	glutMainLoop();

	return(0);

}

void display(void)
{
	time_t tt;
	time(&tt);
	struct tm *tms;
	tms = localtime(&tt);
	View view;
	View_construct(&view);
  	//printf("%d/%d/%d ", tms->tm_mday, tms->tm_mon, tms->tm_year);
  	//printf("%d:%d:%d\n", tms->tm_hour, tms->tm_min, tms->tm_sec);

	int ww, wh;
	getWindowSize(&ww, &wh);


	glClear(GL_COLOR_BUFFER_BIT);
	View_begin_2d(&view, ww, wh);
	Image2D_put(&img);
	View_end();	
	View_begin_3d(&view, ww, wh);

	glPushMatrix();	
	//glTranslated(100, 0, 0);
	//glRotated(tms->tm_sec*M_PI, 0.0, 0.0, 1.0);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for(int i=0; i < 4; i++)
		glVertex3dv(vertices[i]);
	glEnd();
	glPopMatrix();

	glBegin(GL_LINES);
	glVertex2f(ww/2, wh/2);
	glVertex2f(ww/2 + 70*sin((float)tms->tm_sec/30.0*M_PI), wh/2 - 70*cos((float)tms->tm_sec/30.0*M_PI));
	glEnd();

	View_end();
	
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
}

void getWindowSize(int *x, int *y)
{
	*x = glutGet(GLUT_WINDOW_WIDTH);
	*y = glutGet(GLUT_WINDOW_HEIGHT);
}

void init(int *argc, char **argv, GLuint width, GLuint height, char *title)
{
	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glClearColor(0.0, 0.0, 1.0, 1.0);

	//glEnable(GL_BLEND | GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
}
