#include<windows.h>
#include<gl/glut.h>
#include<stdio.h>
#include<math.h>
#include<GL/glaux.h>

GLuint drawcube,drawsphere,drawteapot;

static float a=0.0;
static GLdouble eye=1.0;
static GLfloat locate=1.0;
static int lflag=GL_TRUE,rflag=GL_TRUE;

GLfloat planes[] = { 1.0,0.0,0.0,0.0 };
GLfloat planet[] = { 0.0,0.5,0.0,0.5 };

GLuint texture[8];    //创建一个纹理空间
AUX_RGBImageRec* LoadBMP(CHAR* Filename) //载入位图图像
{
    FILE* File = NULL;                             //文件句柄
    if (!Filename)                                //确保文件名已提供
    {
        return NULL;
    }
    File = fopen(Filename, "r");                   //尝试打开文件
    if (File)
    {
        fclose(File);                            //关闭文件
        return auxDIBImageLoadA(Filename);       //载入位图并返回指针
    }
    return NULL;                              //如果载入失败，返回NULL
}

int LoadGLTextures()  //载入位图并转换成纹理
{
    int Status = FALSE;                         //状态指示器
    AUX_RGBImageRec* TextureImage[8];         //创建纹理的存储空间
    memset(TextureImage, 0, sizeof(void*) * 8);//初始化
//载入位图，检查有无错误，如果位图没找到则退出
    char* image[] = {
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture_1.bmp",
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture1_1.bmp",
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture2_1.bmp",
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture3_1.bmp",
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture4_1.bmp",
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture5_1.bmp",
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture6_1.bmp",
        (char*)"D:\c_codeblock\qimozuoye\wenli.bmp\texture7_1.bmp",
    };
    for (int i = 0; i < 8; i++)
    {
        if (TextureImage[i] = LoadBMP(image[i]))
        {
            Status = TRUE;
            glGenTextures(1, &texture[i]);
            //使用来自位图数据生成的纹理
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            //指定二维纹理
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i]->sizeX, TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i]->data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        if (TextureImage[i])               //纹理是否存在
        {
            if (TextureImage[i]->data)     //纹理图像是否存在
            {
                free(TextureImage[i]->data);      //释放纹理图像占用的内存
            }
            free(TextureImage[i]);        //释放图像结构
        }
    }

    return Status;                   //返回Status
}

void reshape(int w, int h)
{

    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,w/h,0.5,1000);  //透视效果
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(80,100,300,0,0,0,0,10,0);//设置观察点

}
void init1() //初始化
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //画出正方体显示列表
    drawcube = glGenLists(1);
    glNewList(drawcube, GL_COMPILE);
    glutSolidCube(1);
    glEndList();

    //画出茶壶显示列表
    drawteapot = glGenLists(1);
    glNewList(drawteapot, GL_COMPILE);
    glutSolidTeapot(1);
    glEndList();


    //设置普通灯光照0位置及参数；
    GLfloat position0[] = { 30,5,30,1 };
    GLfloat light0s[] = { 0.10,0.10,0.10,0.0 };
    GLfloat light0d[] = { 0.6,0.7,0.7 };
    GLfloat light0a[] = { 0.9,0.9,0.9 };

    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0s);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0d);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0a);

    //设置探照灯光照1位置及参数
    GLfloat position1[] = { -60,40,0,1 };
    GLfloat light1s[] = { 1.0,1.0,1.0,1.0 };
    GLfloat light1d[] = { 0.06,0.1,0.1 };
    GLfloat light1a[] = { 0.91,0.99,0.96 };
    GLfloat  direction[] = { 0,-60,0,1 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1s);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1d);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1a);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    LoadGLTextures();

}

void draw()  //绘制
{
    if(lflag)
    glEnable(GL_LIGHT0);
    if(rflag)
    glEnable(GL_LIGHT1);
    glClearColor(0.51,0.40,0.5,0.0); //背景色
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(a,0.0,1.0,0.0);
    glScaled(eye,eye,eye);
    glTranslatef(0,locate,0);
    glPushMatrix();

    //房子地面贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    //房子地面
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.0);
    glTranslatef(0,-20,0);
    glScalef(147.5,1,147.5);
    glRotatef(90, 1, 0, 0);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);

    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//

    //房顶贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    //房顶
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(1.0, 1.0, 1.0, 0.0);
    glTranslatef(0, 75, 0);
    glScalef(147.5, 1, 147.5);
    glRotatef(90, 1, 0, 0);
    glCallList(drawcube);
    glColor4f(0.0, 1.0, 0.0, 0.0);
    glutSolidCube(1);

    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//


    //草坪贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    //草坪
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.5);
    glTranslatef(0,-22,0);
    glScalef(700,2,700);
    glRotatef(90, 1, 0, 0);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.5);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//

    //土地
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(210.0/255,105.0/255,30.0/255,0.5);
    glTranslatef(0,-32,0);
    glScalef(700,20,700);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.5);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //墙贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    //墙1
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.0);
    glTranslatef(75,0,0);
    glScalef(2,150,150);
    glRotatef(90, 0, 1, 0);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //墙2
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.0);
    glTranslatef(-75,0,0);
    glScalef(2,150,150);
    glRotatef(90,0,1,0);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //墙3
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.0);
    glTranslatef(0,0,-75);
    glScalef(150,150,2);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //墙4
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.0);
    glTranslatef(-45,0,75);
    glScalef(60,150,2);
    glCallList(drawcube);
    glColor4f(1.0,1.0,1.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //墙5
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.0);
    glTranslatef(45,0,75);
    glScalef(60,150,2);
    glCallList(drawcube);
    glColor4f(1.0,1.0,1.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //墙6
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,0.0);
    glTranslatef(0,60,75);
    glScalef(30,30,2);
    glCallList(drawcube);
    glColor4f(1.0,1.0,1.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//

    //门贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    //门
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1,1,1,0.0);
    glTranslatef(15,0,85);
    glScalef(2,90,20);
    glRotatef(90,0,1,0);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//

    //墙柱贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    //墙柱
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,1.0);
    glTranslatef(75,0,-75);
    glScalef(10,150,10);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,1.0);
    glTranslatef(-75,0,-75);
    glScalef(10,150,10);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,1.0);
    glTranslatef(75,0,75);
    glScalef(10,150,10);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,1.0,1.0);
    glTranslatef(-75,0,75);
    glScalef(10,150,10);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//

    //树干贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    //树干
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1,1,1,1.0);
    glTranslatef(-120,0,120);
    glScalef(15,120,15);
    glRotatef(90, 0, 1, 1);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//

    //白 枕头
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,1.0,0.9,0.0);
    glTranslatef(-62.5,-7,-67);
    glScalef(18,5,9);
    glCallList(drawcube);
    glColor4f(0.0,1.0,0.0,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //床
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.51,0.40,0.01,0.0);
    glTranslatef(-70,-15,-70);
    glScalef(3,10,3);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.51,0.40,0.01,0.0);
    glTranslatef(-55,-15,-70);
    glScalef(3,10,3);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.51,0.40,0.01,0.0);
    glTranslatef(-70,-15,-40);
    glScalef(3,10,3);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.51,0.40,0.01,0.0);
    glTranslatef(-55,-15,-40);
    glScalef(3,10,3);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//床板
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.51,0.40,0.01,0.0);
    glTranslatef(-62.5,-10,-55);
    glScalef(17,5,32);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//红 被子
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1.0,0.0,0.0,0.0);
    glTranslatef(-62.5,-7,-50);
    glScalef(18,5,25);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

  //绘制四个桌腿：
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0, 0, 0, 0.0);
    glTranslatef(20,-10,-20);
    glScalef(1,20,1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0, 0, 0, 0.0);
    glTranslatef(-20,-10,-20);
    glScalef(1,20,1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0, 0, 0, 0.0);
    glTranslatef(-20,-10,20);
    glScalef(1,20,1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0, 0, 0, 0.0);
    glTranslatef(20,-10,20);
    glScalef(1,20,1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();


    //桌面贴图
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

  //画出桌面：
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(1,1,1,0);
    glScalef(50,1,50);
    glRotatef(90, 1, 0, 0);
    glCallList(drawcube);
    glColor4f(0.39,0.30,0.1,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//


 //绘制两边台阶；
    glPushMatrix();//左边第一级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.51,0.40,0.2,1.0);
    glTranslatef(-32,-12,0);
    glScalef(10,4,50);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.1,0.50,0.51,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();// 右边第一级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.51,0.40,0.2,1.0);
    glTranslatef(32,-12,0);
    glScalef(10,4,50);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.1,0.50,0.51,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();


    glPushMatrix();//左边第二级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.5,0.40,0.3,0.0);
    glTranslatef(-34.5,-8,0);
    glScalef(5,4,50);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.1,0.50,0.51,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//右边第二级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.5,0.40,0.3,0.0);
    glTranslatef(34.5,-8,0);
    glScalef(5,4,50);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.1,0.50,0.51,0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //桌面上摆放物品，茶壶
    glPushMatrix();//茶壶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.1,0.1,0.1,0.0);
    glTranslatef(0,8,0);
    glScalef(5,10,5);
    glCallList(drawteapot);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    GLUquadricObj *pObj;
    glPushMatrix();//第一个杯子  左边
    pObj=gluNewQuadric();
    gluQuadricNormals(pObj,GLU_SMOOTH);
    glTranslatef(-8,6,-8);
    glRotatef(90,1,0,0);
    gluCylinder(pObj, 1.50f, 1.0f,5.0f, 26, 13);
    glPopMatrix();

    glPushMatrix();  //第2个杯子 右边
    glTranslatef(8,6,8);
    glRotatef(90,1,0,0);
    gluCylinder(pObj, 1.50f, 1.0f,5.0f, 26, 13);
    glPopMatrix();
    glPushMatrix();  //第3个杯子
    glTranslatef(-8,6,8);
    glRotatef(90,1,0,0);
    gluCylinder(pObj, 1.50f, 1.0f,5.0f, 26, 13);
    glPopMatrix();
    glPushMatrix();  //第4个杯子
    glTranslatef(8,6,-8);
    glRotatef(90,1,0,0);
    gluCylinder(pObj, 1.50f, 1.0f,5.0f, 26, 13);
    glPopMatrix();
    glPushMatrix();  //桌面上垫圈
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor4f(0.21,0.21,0.21,0.0);

    pObj= gluNewQuadric();
    //gluQuadricDrawStyle(pObj3,GLU_LINE);
    glTranslatef(-8,1,-8);
    glRotatef(90,1,0,0);
    gluDisk(pObj, 0.50f,  3.0f, 10, 10);
    glPopMatrix();

    glPushMatrix();  //桌面上垫圈

    pObj = gluNewQuadric();
    glTranslatef(8,1,8);
    glRotatef(90,1,0,0);
    gluDisk(pObj, 0.50f,  3.0f, 30, 30);
    glPopMatrix();
    glPushMatrix();  //桌面上垫圈

    pObj = gluNewQuadric();
    glTranslatef(-8,1,8);
    glRotatef(90,1,0,0);
    gluDisk(pObj, 0.50f,  3.0f, 30, 30);
    glPopMatrix();
    glPushMatrix();  //桌面上垫圈

    pObj = gluNewQuadric();
    glTranslatef(8,1,-8);
    glRotatef(90,1,0,0);
    gluDisk(pObj, 0.50f,  3.0f, 30, 30);
    glPopMatrix();

    glPushMatrix(); //月亮
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor3f(1.0,1.0,0.0);//moon
    glTranslatef(200,180,200);
    glutSolidSphere(35.0,20,20);
    glCallList(drawteapot);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //外地面贴图-树叶
    glPushMatrix();//
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_LINEAR, planes);
    glTexGenfv(GL_T, GL_OBJECT_LINEAR, planet);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);//

    glPushMatrix();  //树叶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT);
    glColor3f(0.0,100.0/255,0.0);//leaf
    glTranslatef(-120,90,120);
    glutSolidSphere(50.0,5,5);
    glCallList(drawteapot);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);//
    glPopMatrix();//

    glPushMatrix();//哑铃球
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor3f(0.0,0.0, 0.0);
    glTranslatef(-62.5, -15, 10);
    glutSolidSphere(4.0, 12, 10);
    glCallList(drawteapot);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glPushMatrix();//哑铃球
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-62.5, -15, 30);
    glutSolidSphere(4.0, 12, 10);
    glCallList(drawteapot);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //哑铃棍
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0, 0, 0, 1.0);
    glTranslatef(-62.5, -15, 20);
    glScalef(1.5, 1.5, 20);
    glCallList(drawcube);
    glColor4f(0.0, 1.0, 0.0, 0.0);
    glutSolidCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}
void NormalKeysProc(unsigned char key,int x,int y) //退出按键
{
    if(key==27) exit(0);
}
void SpecialKeys(int key, int x, int y)  //按键功能
{
    if (key == GLUT_KEY_LEFT)
    {
        a += 1.0;
        glutPostRedisplay();
    }
    if (key == GLUT_KEY_RIGHT)
    {
        a -= 1.0;
        glutPostRedisplay();
    }
    if (key == GLUT_KEY_DOWN)
    {
        eye -= 0.05;
        glutPostRedisplay();
    }
    if (key == GLUT_KEY_UP)
    {
        eye += 0.05;
        glutPostRedisplay();
    }

}
void MouseFunc(int button, int state, int x, int y) {
    switch (state) {
    case GLUT_UP:
        switch (button) {
        case GLUT_LEFT_BUTTON:
            locate += 5.0;
            glutPostRedisplay();
            break;
        case GLUT_RIGHT_BUTTON:
            locate -= 5.0;
            glutPostRedisplay();
            break;
        }
    }
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("finally");
    init1();
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutIdleFunc(draw);
    glutKeyboardFunc(NormalKeysProc);
    glutSpecialFunc(SpecialKeys);
    glutMouseFunc(MouseFunc);
    glutMainLoop();
    return 0;
}
