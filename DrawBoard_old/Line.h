#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Graphic.h"
using namespace std;
#define WIDTH 1000
#define HEIGHT 1000
class Line: public Graphic{
public:
    Line(float x, float y, GLfloat *,float linewidth);
    //Line(GLfloat* postion, GLfloat* pen_color, float linewidth);
    Line(GLfloat* information);
    virtual void draw();
    virtual void update(float, float);
    virtual void addFinish();
    virtual void moveFinish();
    virtual void mpUpdate();
    virtual void saveInfo(char*);

    GLfloat startP[2];
    GLfloat endP[2];
};

void Line::moveFinish(){
    startP[0]+=moveIncrement[0];
    startP[1]+=moveIncrement[1];
    endP[0]+=moveIncrement[0];
    endP[1]+=moveIncrement[1];
    mpUpdate();
    unSelect();
}

void Line::mpUpdate(){
    float minX = min(startP[0], endP[0]);
    float minY = min(startP[1], endP[1]);
    float maxX = max(startP[0], endP[0]);
    float maxY = max(startP[1], endP[1]);
    minPoint[0] = minX;
    minPoint[1] = minY;
    minEdge[0] = maxX-minX;
    minEdge[1] = maxY-minY;
}

void Line::addFinish(){
    mpUpdate();
}

void Line::draw(){
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
    glLineWidth(lineWidth);
    glBegin(GL_LINES);
    {
        glVertex2fv(startP); 
        glVertex2fv(endP);
    }
    glEnd();
    if(is_select){
        glPopMatrix();
    }
}

void Line::saveInfo(char* path){
    ofstream out(path, ios::app);
    if (out.is_open())
    {
        out << typeName << " " << 8 << " ";
        out << startP[0] << " " << startP[1] << " " << endP[0] << " " << endP[1] << " ";
        out << color[0] << " " << color[1] << " " << color[2] << " ";
        out << lineWidth << endl;
    }
    out.close();
}

Line::Line(GLfloat* information)
{
    typeName=2;
    startP[0] = information[0];
    startP[1] = information[1];
    endP[0] = information[2];
    endP[1] = information[3];
    color[0] = information[4];
    color[1] = information[5];
    color[2] = information[6];
    lineWidth = information[7];
    mpUpdate();
}

void Line::update(float x, float y){
    endP[0] = (x-WIDTH/2)/(WIDTH/2);
    endP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
}

Line::Line(float x, float y, GLfloat * pen_color,float linewidth)
:Graphic(pen_color[0], pen_color[1], pen_color[2], linewidth)
{
    typeName=2;
    startP[0] = (x-WIDTH/2)/(WIDTH/2);
    startP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
    endP[0] = (x-WIDTH/2)/(WIDTH/2);
    endP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
}