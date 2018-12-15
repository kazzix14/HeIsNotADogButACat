#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <time.h> 
#include <math.h>


void display(void);
void reshape(int w, int h);
void timer(int value);
void getWindowSize(int *x, int *y);
void init(int *argc, char **argv, GLuint width, GLuint height, char *title);
void mouse(int, int, int, int);
void begin2d();

pngInfo info1;

int *screenBuf;

int main(int argc, char **argv)
{
	init(&argc, argv, 900, 600, "hello");

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

	int ww, wh;
	getWindowSize(&ww, &wh);


	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 1.0, 0.0);
	
	glBegin(GL_POINTS);
		glVertex2i(11, 23);
		glVertex2i(12, 23);
		glVertex2i(13, 23);
	glEnd();

	glBegin(GL_POINTS);
	for(int i = 0; i < ww * wh; i++)
	{
		if(screenBuf[i]){
			printf("%d: %d, %d\n", i, screenBuf[i]%ww, screenBuf[i]/ww);
			glVertex2i(i%ww, i/ww);
		}
	}
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
	begin2d();
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
	glutMouseFunc(mouse);

	screenBuf = (int*)malloc(sizeof(int)*width*height);
	for(int i = 0; i < width*height; i++)
	{
		screenBuf[i] = 0;
	}

}
void mouse(int btn, int stat, int x, int y)
{
	int w, h; getWindowSize(&w, &h);
	if(btn == GLUT_LEFT_BUTTON){
		//printf("%d, %d\n", x, y);
		screenBuf[x+y*w] = 1;
		}
}

void begin2d()
{
	int w, h;
	getWindowSize(&w, &h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
