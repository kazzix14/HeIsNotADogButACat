#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "compoment.h"
#include "image.h"

void display(void);
void reshape(int w, int h);
void timer(int value);
void getWindowSize(int *x, int *y);
void init(int *argc, char **argv, GLuint width, GLuint height, char *title);

Image img;
pngInfo info1;

int main(int argc, char **argv)
{
	init(&argc, argv, 480, 480, "hello");

	Image_construct(&img, "atest.png", 0, 0);
	Image_get_info(&img, &info1);
   	printf("img1: W=%d, H=%d, D=%d, A=%d\n",info1.Width, info1.Height, info1.Depth, info1.Alpha);

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

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	glVertex2f(ww/2, wh/2);
	glVertex2f(ww/2 + 70*sin((float)tms->tm_sec/30.0*M_PI), wh/2 - 70*cos((float)tms->tm_sec/30.0*M_PI));
	glEnd();
	Image_put(&img);
	Image_get_info(&img, &info1);
   	//printf("img1: W=%d, H=%d, D=%d, A=%d\n",info1.Width, info1.Height, info1.Depth, info1.Alpha);
	
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glScaled(1, -1, 1);
	glTranslated(0, -h, 0);
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA);
	glClearColor(0.0, 0.0, 1.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
}
