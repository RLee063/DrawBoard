#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "Graphic.h"
using namespace std;
#define pi 3.141592
#define N 3600
#define WIDTH 1000
#define HEIGHT 1000
class Circle: public Graphic{
public:
    Circle(float x, float y, GLfloat*, bool fill, float linewidth);
    //Circle(GLfloat* position, GLfloat* pen_color, float lineWidth, bool fill);
    Circle(GLfloat* infomation);
    virtual void draw();
    virtual void update(float, float);
    virtual void addFinish();
    virtual void moveFinish();
    virtual void mpUpdate();
    virtual void saveInfo(char*);

    float radius;
    bool is_fill;
    GLfloat originP[2];
};

void Circle::saveInfo(char* path){
    ofstream out(path, ios::app);
    if (out.is_open())
    {
        out << typeName << " " << 8 << " ";
        out << originP[0] << " " << originP[1] << " " << radius << " ";
        out << color[0] << " " << color[1] << " " << color[2] << " ";
        out << lineWidth << " " << is_fill << endl;
    }
    out.close();
}

Circle::Circle(GLfloat* infomation)
{
    typeName=4;
    originP[0] = infomation[0];
    originP[1] = infomation[1];
    radius = infomation[2];
    color[0] = infomation[3];
    color[1] = infomation[4];
    color[2] = infomation[5];
    lineWidth = infomation[6];
    is_fill = infomation[7];
    mpUpdate();
}

void Circle::mpUpdate(){
    minPoint[0] = originP[0] - radius;
    minPoint[1] = originP[1] - radius;
    minEdge[0] = 2*radius;
    minEdge[1] = 2*radius;
}

void Circle::addFinish(){
    mpUpdate();
}

void Circle::moveFinish(){
    originP[0] += moveIncrement[0];
    originP[1] += moveIncrement[1];
    mpUpdate();
    unSelect();
}

void Circle::draw(){
    if(is_select){
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();     
        glTranslatef(moveIncrement[0], moveIncrement[1], 0);
        glColor3f(color[0]*0.5, color[1]*0.4, color[2]*0.3);
        glBegin(GL_LINE_LOOP);
        {
            glVertex2fv(minPoint);
            glVertex2f(minPoint[0]+minEdge[0], minPoint[1]);
            glVertex2f(minPoint[0]+minEdge[0], minPoint[1]+minEdge[1]);
            glVertex2f(minPoint[0], minPoint[1]+minEdge[1]);
        }
        glEnd();
    }
    glColor3fv(color);
    if(is_fill){
        glBegin(GL_POLYGON);
    }
    else{
        glLineWidth(lineWidth);
        glBegin(GL_LINE_LOOP);
    }
    for(int i=0;i<N;i++)  
    {  
        glVertex2f(radius*cos(2*pi*i/N)+originP[0],radius*sin(2*pi*i/N)+originP[1]);   //¶¨Òå¶¥µã  
    }  
    glEnd();  
    if(is_select){
        glPopMatrix();   
    }
}

void Circle::update(float gx, float gy){
    float x = (gx-WIDTH/2)/(WIDTH/2);
    float y = -(gy-HEIGHT/2)/(HEIGHT/2);
    radius = sqrt((x-originP[0])*(x-originP[0])+(y-originP[1])*(y-originP[1]));
}

Circle::Circle(float x, float y, GLfloat* pen_color, bool fill, float linewidth):
Graphic(pen_color[0], pen_color[1], pen_color[2], linewidth),
is_fill(fill),
radius(0)
{
    typeName=4;
    originP[0] = (x-WIDTH/2)/(WIDTH/2);
    originP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
}