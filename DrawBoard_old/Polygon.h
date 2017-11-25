#include <GL/glut.h>
#include <list>
#include "Graphic.h"
#define WIDTH 1000
#define HEIGHT 1000

class Polygon: public Graphic{
public:
    Polygon(float x, float y, GLfloat*, bool fill, float linewidth);
    virtual void draw();
    virtual void update(float, float);
    virtual void addFinish();
    virtual void mouseRightDown(float, float);
    virtual void mpUpdate();
    virtual void moveFinish();
    Node* tempN;
    list<Node*> node_list;
};

void Polygon::moveFinish(){
    for(auto i = node_list.begin(); i!=node_list.end(); i++){
        (*i)->x += moveIncrement[0];
        (*i)->y += moveIncrement[1];
    }
    mpUpdate();
    unSelect();
}

void Polygon::mpUpdate(){
    is_adding = false;
    auto i = node_list.begin();
    float minX = (*i)->x;
    float minY = (*i)->y;
    float maxX = (*i)->x;
    float maxY = (*i)->y;
    i++;
    for(i; i!=node_list.end(); i++){
        if((*i)->x<minX){
            minX = (*i)->x;
        }
        if((*i)->y<minY){
            minY = (*i)->y;
        }
        if((*i)->x>maxX){
            maxX = (*i)->x;
        }
        if((*i)->y>maxY){
            maxY = (*i)->y;
        }
    }
    minPoint[0] = minX;
    minPoint[1] = minY;
    minEdge[0] = maxX-minX;
    minEdge[1] = maxY-minY;
}

void Polygon::addFinish(){
    mpUpdate();
}

void Polygon::draw(){
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
    if(is_adding){
        glLineWidth(lineWidth);
        glBegin(GL_LINE_STRIP);
        for(auto i = node_list.begin(); i!=node_list.end(); i++){
            glVertex2f((*i)->x, (*i)->y);
        }
        glVertex2f(tempN->x, tempN->y);
        glEnd();
    }
    else{
        if(is_fill){
            glBegin(GL_POLYGON);
        }
        else{
            glLineWidth(lineWidth);
            glBegin(GL_LINE_LOOP);
        }
        {
            for(auto i = node_list.begin(); i!=node_list.end(); i++){
                glVertex2f((*i)->x, (*i)->y);
            }
        }
        glEnd();
    }
    if(is_select){
        glPopMatrix();
    }
}

void Polygon::update(float gx, float gy){
    float x = (gx-WIDTH/2)/(WIDTH/2);
    float y = -(gy-HEIGHT/2)/(HEIGHT/2);
    tempN->x = x;
    tempN->y = y;
}

void Polygon::mouseRightDown(float gx, float gy){
    float x = (gx-WIDTH/2)/(WIDTH/2);
    float y = -(gy-HEIGHT/2)/(HEIGHT/2);
    Node* tp = new Node(x, y);
    node_list.push_back(tp);
}

Polygon::Polygon(float gx, float gy, GLfloat* pen_color,bool fill,float linewidth):
Graphic(pen_color[0],pen_color[1],pen_color[2],linewidth)
{
    float x = (gx-WIDTH/2)/(WIDTH/2);
    float y = -(gy-HEIGHT/2)/(HEIGHT/2);
    tempN = new Node(x, y);
    Node* gp = new Node(x, y);
    node_list.push_back(gp);
    is_fill = fill;
    is_adding = 1;
}