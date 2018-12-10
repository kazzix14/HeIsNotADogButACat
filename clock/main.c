#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "component.h"
#include "image.h"

void display(void);
void reshape(int w, int h);
void timer(int value);
void getWindowSize(int *x, int *y);
void init(int *argc, char **argv, GLuint width, GLuint height, char *title);

Image img;
pngInfo info1;

double vertices[][3]={
	{0.0, 10.0, 0.0},
	{0.0, 20.0, 50.0},
	{50.0, 50.0, 0.0},
	{50.0, 0.0, 0.0}
};

int main(int argc, char **argv)
{
	init(&argc, argv, 480, 480, "hello");

	Image_construct(&img, "image.png", 0, 0);

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

  	//printf("%d/%d/%d ", tms->tm_mday, tms->tm_mon, tms->tm_year);
  	//printf("%d:%d:%d\n", tms->tm_hour, tms->tm_min, tms->tm_sec);

	int ww, wh;
	getWindowSize(&ww, &wh);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1000.0,
		  0.0, 0.0, 0.0,
		  0.0, 1.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT);

	Image_put(&img);
	
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0, w, 0, h, 1.0, -1.0);
	gluPerspective(30.0, (double)w / (double)h, 1.0, 10000.0);

	glScaled(1, -1, 1);
	glTranslated(0, -h, 0.0);
	
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

	glEnable(GL_BLEND | GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
}
