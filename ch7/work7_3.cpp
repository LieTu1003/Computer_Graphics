#include<windows.h>
#include<stdio.h>
#include<GL/glut.h>

#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define YES 1
#define NO 0

int Drawing;
void Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glEnable(GL_POINT_SMOOTH);//点反走样
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);//直线反走样
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	Drawing = NO;
}

void OnDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (Drawing == YES) //启用混合状态
	{
		glEnable(GL_BLEND);
	}
	else //关闭混合状态
	{
		glDisable(GL_BLEND);
	}
	glColor4f(0.0f, 0.6f, 0.0f, 1.0f); //设置颜色
	glLineWidth(10);
	glBegin(GL_LINE_STRIP);
	{
		glVertex2f(-3.0f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(3.0f, -0.5f);
	}
	glEnd();
	glPointSize(10);
	glBegin(GL_POINTS);
	{
		glVertex2f(0.0f, 1.0f);
	}
	glEnd();
	glutSwapBuffers();
}

void OnReShape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h != 0)
	{
		GLfloat aspect = (float)w / (float)h;

		if (w<h)
			gluOrtho2D(-3, 3, -3 * aspect, 3 * aspect);
		else
			gluOrtho2D(-3 / aspect, 3 / aspect, -3, 3);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CreateeMenu(void)
{
	glutAddMenuEntry("normal", NO);
	glutAddMenuEntry("antialiasing", YES);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void OnMenu(int value)
{
	Drawing = value;
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//初始化显示模式
	glutInitWindowSize(300, 300);//设置初始窗口的大小
	glutCreateWindow("antialiasing");
	glutReshapeFunc(OnReShape);
	glutDisplayFunc(OnDisplay);
	glutCreateMenu(OnMenu);
	Init();//初始化
	CreateeMenu();
	glutMainLoop();//进入程序循环
	return 0;
}
