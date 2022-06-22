#include<windows.h>
#include <GL/glut.h>
GLsizei winWidth = 400, winHeight = 300;
GLint endPtCtr = 0;
class scrpt
{
public:
    GLint x, y;
};
void init(void)
{
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 200, 0.0, 150.0);
}
void displayFcn(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
}
void winReshapeFcn(GLint newWidth, GLint newHeight)
{
    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
    winWidth = newWidth;
    winHeight = newHeight;
}
void drawLineSegment(scrpt s1, scrpt s2)
{
    glBegin(GL_LINES);
    glVertex2i(s1.x, s1.y);
    glVertex2i(s2.x, s2.y);
    glEnd();
}
void polyline(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
    static scrpt endpt1, endpt2;
    if (endPtCtr == 0)
    {
        if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
        {
            endPtCtr = 1;
            endpt1.x = xMouse;
            endpt1.y = winHeight - yMouse;
        }
        else if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
            exit(0);
    }
    else if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
    {
        endpt2.x = xMouse;
        endpt2.y = yMouse;
        drawLineSegment(endpt1, endpt2);
        endpt1 = endpt2;
    }
    else if (button == GLUT_RIGHT_BUTTON)
        exit(0);
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Drawing   Interactive polyline");
    init();
    glutDisplayFunc(displayFcn);
    glutMouseFunc(polyline);
    glutMainLoop();
    return 0;
}

