#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Graphic.h"
using namespace std;
#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
#define WIDTH 1000
#define HEIGHT 1000
class Rectangle: public Graphic{
public:
    Rectangle(float x, float y, GLfloat*, bool fill, float linewidth);
    //Rectangle(GLfloat* position, GLfloat* pen_color, float lineWidth, bool fill);
    Rectangle(GLfloat* infomation);
    virtual void draw();
    virtual void update(float, float);
    virtual void addFinish();
    virtual void moveFinish();
    virtual void mpUpdate();
    virtual void saveInfo(char*);
    GLfloat originP[2];
    GLfloat mouseP[2];
    GLfloat parrallel_oP[2];
    GLfloat parrallel_mP[2];
};

void Rectangle::saveInfo(char* path){
    ofstream out(path, ios::app);
    if (out.is_open())
    {
        out << typeName << " " << 13 << " ";
        out << originP[0] << " " << originP[1]<< " ";
        out << mouseP[0] << " " << mouseP[1]<< " ";
        out << parrallel_oP[0] << " " << parrallel_oP[1]<< " ";
        out << parrallel_mP[0] << " " << parrallel_mP[1]<< " ";
        out << color[0] << " " << color[1] << " " << color[2] << " ";
        out << lineWidth << " " << is_fill << endl;
    }
    out.close();
}

Rectangle::Rectangle(GLfloat* infomation)
{
    typeName = 3;
    originP[0] = infomation[0];
    originP[1] = infomation[1];
    mouseP[0] = infomation[2];
    mouseP[1] = infomation[3];
    parrallel_oP[0] = infomation[4];
    parrallel_oP[1] = infomation[5];
    parrallel_mP[0] = infomation[6];
    parrallel_mP[1] = infomation[7];
    color[0] = infomation[8];
    color[1] = infomation[9];
    color[2] = infomation[10];
    lineWidth = infomation[11];
    is_fill = infomation[12];
    mpUpdate();
}

void Rectangle::mpUpdate(){
    float minX = min(originP[0], mouseP[0]);
    float minY = min(originP[1], mouseP[1]);
    float maxX = max(originP[0], mouseP[0]);
    float maxY = max(originP[1], mouseP[1]);
    minPoint[0] = minX;
    minPoint[1] = minY;
    minEdge[0] = maxX-minX;
    minEdge[1] = maxY-minY;
}

void Rectangle::addFinish(){
    mpUpdate();
}

void Rectangle::draw(){         
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
    {
        glVertex2fv(originP); 
        glVertex2fv(parrallel_oP); 
        glVertex2fv(mouseP);
        glVertex2fv(parrallel_mP);
    }
    glEnd();
    if(is_select){
        glPopMatrix();   
    }
}

void Rectangle::moveFinish(){
    originP[0]+=moveIncrement[0];
    originP[1]+=moveIncrement[1];
    parrallel_oP[0]+=moveIncrement[0];
    parrallel_oP[1]+=moveIncrement[1];
    mouseP[0]+=moveIncrement[0];
    mouseP[1]+=moveIncrement[1];
    parrallel_mP[0]+=moveIncrement[0];
    parrallel_mP[1]+=moveIncrement[1];

    moveIncrement[0] = 0;
    moveIncrement[1] = 0;
    mpUpdate();
    unSelect();
}

void Rectangle::update(float x, float y){
    mouseP[0] = (x-WIDTH/2)/(WIDTH/2);
    mouseP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
    parrallel_oP[0] =  (x-WIDTH/2)/(WIDTH/2);
    parrallel_mP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
}

Rectangle::Rectangle(float x, float y, GLfloat* pen_color, bool fill, float linewidth):
Graphic(pen_color[0], pen_color[1], pen_color[2], linewidth)
{
    typeName=3;
    originP[0] = (x-WIDTH/2)/(WIDTH/2);
    originP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
    mouseP[0] = (x-WIDTH/2)/(WIDTH/2);
    mouseP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
    parrallel_oP[0] =  (x-WIDTH/2)/(WIDTH/2);
    parrallel_oP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
    parrallel_mP[0] =  (x-WIDTH/2)/(WIDTH/2);
    parrallel_mP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
    is_adding = 1;
    is_fill = fill;
}
#endif