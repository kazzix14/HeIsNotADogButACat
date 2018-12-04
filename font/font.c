#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

void Display(void);
void Reshape(int w, int h);
void getWindowSize(int *x, int *y);
void Timer(int value);
void PrintText(int, int, char*);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(320, 240);
	glutCreateWindow("test");

	glutDisplayFunc(Display);
	
	glutReshapeFunc(Reshape);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glClearColor(0.0, 0.0, 0.0, 1.0);


	
	glutTimerFunc(500, Timer, 0);
	glutMainLoop();

	return(0);

}

void getWindowSize(int *x, int *y)
{
	*x = glutGet(GLUT_WINDOW_WIDTH);
	*y = glutGet(GLUT_WINDOW_HEIGHT);
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

	//int ww, wh;
	//float pw, ph;

	//getWindowSize(&ww, &wh);
	//pw = ww/256.0;
	//ph = wh/256.0;

	//for(float x = 0; x < ww; x += pw)
	//{
	//	printf("%d\n", 	(int)(x/pw));
	//	for (float y = 0; y < wh; y += ph)
	//	{
	//		glColor3ub((int)(x/pw), 0, (int)(y/ph));
	//		glBegin(GL_POINTS);
	//		glVertex2i((int)(x*pw), (int)(y*ph));
	//		glEnd();
	//	}
	//}

	PrintText(123, 45, "aadfhasldkfh");

	glFlush();
	glutSwapBuffers();
}

void PrintText(int x, int y, char *s){
	glRasterPos2i(x, y);
	for(int i = 0; i < strlen(s); i++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);
	
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
