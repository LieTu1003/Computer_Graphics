#include<windows.h>
#include<stdlib.h>
#include<stdio.h>
#include<GL/glut.h>


void putpixel(GLint x,GLint y){
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
    glFlush();
}

void circlePoint(int x,int y,int xc,int yc){
    putpixel(x+xc,y+yc);
    putpixel(y+xc,x+yc);
    putpixel(-y+xc,x+yc);
    putpixel(-x+xc,y+yc);
    putpixel(-x+xc,-y+yc);
    putpixel(-y+xc,-x+yc);
    putpixel(y+xc,-x+yc);
    putpixel(x+xc,-y+yc);
}


void draw_circle(int xc,int yc,int R){
    float d=1.25-R;
    int x=0;
    int y=R;
    while(x<y){
        circlePoint(x,y,xc,yc);
        if(d<=0){
            d=d+2*x+3;
        }else{
            d=d+2*(x-y)+5;
            y=y-1;
        }
        x=x+1;
    }
}

void init(void ){
    glClearColor(1.0,1.0,1.0,0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,200.0,0.0,150.0);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    draw_circle(100,80,50);
    glEnd();
    glFlush();
}


int main(int argc,char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50,100);
    glutInitWindowSize(400,300);
    glutCreateWindow("中点算法画圆");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
