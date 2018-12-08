#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

void Display(void);
void Reshape(int w, int h);
void Timer(int value);
void getWindowSize(int *x, int *y);

int main(int argc, char **argv)
{
	
	glutTimerFunc(500, Timer, 0);
	glutMainLoop();

	return(0);

}

void Display(void)
{
	time_t tt;
	time(&tt);
	struct tm *tms;
	tms = localtime(&tt);
	
  	printf("%d/%d/%d ", tms->tm_mday, tms->tm_mon, tms->tm_year);
  	printf("%d:%d:%d\n", tms->tm_hour, tms->tm_min, tms->tm_sec);

	int ww, wh;
	getWindowSize(&ww, &wh);

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
	glVertex2f(ww/2, wh/2);
	glVertex2f(ww/2 + 70*sin((float)tms->tm_sec/30.0*M_PI), wh/2 - 70*cos((float)tms->tm_sec/30.0*M_PI));
	glEnd();

	glFlush();
	glutSwapBuffers();
}

void Timer(int value)
{
	glutTimerFunc(500, Timer, 0);
	glutPostRedisplay();
}

void Reshape(int w, int h)
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

void init(char &argv,int argv, GLuint width, GLuint height, char *title)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
}
