#include<windows.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <conio.h>

#include<gl/glut.h>
#include<stdio.h>
#include<math.h>
#include<GL/glaux.h>
using namespace std;

float* pixel; //定义像素点颜色指针，方便自定义三角形颜色,同时代替帧缓冲器
const int width = 600, length = 600;//窗口大小
float plane[3]; //存储空间直角坐标系平面方程系数ABC
float zBuff[length][width];//数组代替深度缓冲器，存储深度值

void init(void)
{
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 200.0);
}

void setPixel(int x, int y, float z, float r, float g, float b)
{
	if (z > zBuff[y][x])//如果(z(x,y)大于z缓存在(x,y)的值)
	{
		zBuff[y][x] = z;//把z(x,y)存入z缓存中(x,y)处
		pixel[3 * y * width + x * 3 + 0] = r;
		pixel[3 * y * width + x * 3 + 1] = g;
		pixel[3 * y * width + x * 3 + 2] = b;
	}
}

void zPlane(float x[], float y[], float z[])//计算出Ax + By + Cz + D = 0中的ABC系数
{
	float x1 = x[0];
	float x2 = x[1];
	float x3 = x[2];
	float y1 = y[0];
	float y2 = y[1];
	float y3 = y[2];
	float z1 = z[0];
	float z2 = z[1];
	float z3 = z[2];
	float temp = x1 * (y2 * z3 - y3 * z2) - y1 * (x2 * z3 - x3 * z2) + z1 * (x2 * y3 - x3 * y2);
	float A = (y3 * z2 - y2 * z3 + y1 * z3 - y3 * z1 + y2 * z1 - y1 * z2) / temp;
	float B = (x2 * z3 - x3 * z2 + x3 * z1 - x1 * z3 + x1 * z2 - x2 * z1) / temp;
	float C = (x3 * y2 - x2 * y3 + x1 * y3 - x3 * y1 + x2 * y1 - x1 * y2) / temp;
	plane[0] = A;
	plane[1] = B;
	plane[2] = C;
}

float Zs(float x, float y)
{
	float z = 0;
	z = (-plane[0] * x - plane[1] * y - 1) / plane[2];//由Ax + By + Cz + D = 0得，为了方便令D = 1
	return z;
}

//画三角形(画三角形部分是用的之前多边形代码改的，就不写注释了)
void triangleDraw(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b)
{
	float X[3];//存储三点顶点的x坐标，下同
	X[0] = x0;
	X[1] = x1;
	X[2] = x2;
	float Y[3];
	Y[0] = y0;
	Y[1] = y1;
	Y[2] = y2;
	float Z[3];
	Z[0] = z0;
	Z[1] = z1;
	Z[2] = z2;
	zPlane(X, Y, Z);//计算系数

	//顶点排序
	float xTemp[3], yTemp[3], zTemp[3];
	for (int i = 0; i < 3; i++)
	{
		xTemp[i] = X[i];
		yTemp[i] = Y[i];
		zTemp[i] = Z[i];
	}
	//找到三角形最小的y扫描线
	float temp = Y[0];
	int n = 0;
	for (int i = 1; i < 3; i++)
	{
		if (Y[i] < temp)
		{
			temp = Y[i];
			n = i;
		}
	}
	Y[0] = yTemp[n];
	X[0] = xTemp[n];
	Z[0] = zTemp[n];
	float yMin = temp;
	float xMin = xTemp[n];

	for (int i = 0, a = 0; i < 3; i++)
	{
		if (temp < yTemp[i])
		{
			a++;
			Y[a] = yTemp[i];
			X[a] = xTemp[i];
			Z[a] = zTemp[i];
			temp = yTemp[i];
		}
		else if (yTemp[i] >= yMin)
		{
			if (xTemp[i] != xMin)
			{
				float t1, t2, t3;
				t1 = Y[a];
				t2 = X[a];
				t3 = Z[a];
				Y[a] = yTemp[i];
				X[a] = xTemp[i];
				Z[a] = zTemp[i];
				Y[a + 1] = t1;
				X[a + 1] = t2;
				Z[a + 1] = t3;
				a++;
				yTemp[n] = yMin;
			}
		}
	}

	float y = Y[0];
	float xL = X[0], xR = X[0];
	float k1 = (y1 - y0) / (x1 - x0);//计算三角形边的斜率
	float k2 = (y2 - y1) / (x2 - x1);
	float k3 = (y2 - y0) / (x2 - x0);

	if (Y[0] != Y[1])
	{
		while (y < Y[2])
		{
			y++;
			if (y != Y[1])
			{
				xL = xL + 1 / k1;
				xR = xR + 1 / k3;

				for (int i = xL; i <= xR; i++)
				{
					setPixel(i, y, Zs(i, y), r, g, b);//填充像素
				}
			}
			else if (y == Y[1])
			{
				xL = xL + 1 / k1;
				xR = xR + 1 / k3;
				for (int i = xL; i <= xR; i++)
				{
					setPixel(i, y, Zs(i, y), r, g, b);
				}
				k1 = k2;
			}
		}
	}
	else if (Y[0] == Y[1])
	{
		xR = x1;
		for (int i = x0; i <= xR; i++)
		{
			setPixel(i, y, Zs(i, y), r, g, b);
		}
		while (y < Y[2])
		{
			y++;
			xL = xL + 1 / k1;
			xR = xR + 1 / k3;
			for (int i = xL; i <= xR; i++)
			{
				setPixel(i, y, Zs(i, y), r, g, b);
			}
		}
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  // 清理窗口缓存
	glLoadIdentity();
	glDrawPixels(width, length, GL_RGB, GL_FLOAT, pixel);//初始化背景色
	glFlush();
}

void buffer(int argc, char** argv) {//同一平面按作图顺序作画
	//glutInit(&argc, argv);                         // 初始化
	glutInitWindowSize(width, length);      // 设置显示窗口的大小
	glutCreateWindow("z-buffer算法");
	//           x0   y0  z0  x1   y1   z1  x2   y2    z2  r  g    b
	triangleDraw(100, 130, 3, 30, 300, 3, 180, 300, 6, 0, 0.3, 1); //蓝
	triangleDraw(120, 180, 5, 60, 340, 5, 600, 400, 4, 1, 0.8, 0); //黄
	triangleDraw(450, 220, 3, 0, 360, 3, 420, 460, 6, 1, 0, 1); //粉
	init();
	glutDisplayFunc(display);       // 发送图形到显示窗口
	glutMainLoop();                  // 循环显示图像，防闪退
}

void draw(int argc, char** argv) {//通过z缓冲器存储深度
	//glutInit(&argc, argv);                         // 初始化
	glutInitWindowSize(width, length);      // 设置显示窗口的大小
	glutCreateWindow("画家算法");
	//           x0   y0  z0  x1   y1   z1  x2   y2    z2  r  g    b
	triangleDraw(120, 180, 3, 60, 340, 3, 600, 400, 3, 1, 0.8, 0); //黄
	triangleDraw(100, 130, 3, 30, 300, 3, 180, 300, 3, 0, 0.3, 1); //蓝
	triangleDraw(450, 220, 3, 0, 360, 3, 420, 460, 3, 1, 0, 1); //粉
	init();
	glutDisplayFunc(display);       // 发送图形到显示窗口
	glutMainLoop();                     // 循环显示图像，防闪退
}

int main()
{
	float x0[9], y0[9], z0[9], x1[9], y1[9], z1[9], x2[9], y2[9], z2[9], r[9], g[9], b[9];
	int flag;
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < width; j++)
		{
			zBuff[i][j] = -0xffffffff;//定义最小深度值
		}
	}
	pixel = new float[3 * width * length];//定义像素点
	cout << "按序号输入算法类型(1或2)" << endl << "1.画家算法 2.Z-buffer算法:" << endl;
	cin >> flag;
	if (flag == 1)
		draw(NULL, NULL);
	else if(flag == 2)
		buffer(NULL,NULL);
	return 0;
}

