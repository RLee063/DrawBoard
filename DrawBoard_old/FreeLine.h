#include <GL/glut.h>
#include <list>
#include "Graphic.h"
#define WIDTH 1000
#define HEIGHT 1000


class FreeLine: public Graphic{
public:
    FreeLine(float x, float y, GLfloat*,float linewidth);
    virtual void draw();
    virtual void update(float, float);
    virtual void addFinish();
    virtual void moveFinish();
    virtual void mpUpdate();
    list<Node*> node_list;
};

void FreeLine::draw(){
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
    glBegin(GL_LINE_STRIP);
    {
        for(auto i = node_list.begin(); i!=node_list.end(); i++){
            glVertex2f((*i)->x, (*i)->y);
        }
    }
    glEnd();
    if(is_select){
        glPopMatrix();   
    }
}

void FreeLine::mpUpdate(){
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

void FreeLine::moveFinish(){
    for(auto i = node_list.begin(); i!=node_list.end(); i++){
        (*i)->x += moveIncrement[0];
        (*i)->y += moveIncrement[1];
    }
    mpUpdate();
    unSelect();
}

void FreeLine::addFinish(){
    mpUpdate();
}

void FreeLine::update(float gx, float gy){
    float x = (gx-WIDTH/2)/(WIDTH/2);
    float y = -(gy-HEIGHT/2)/(HEIGHT/2);
    Node* gp = new Node(x, y);
    node_list.push_back(gp);
}

FreeLine::FreeLine(float g_x, float g_y, GLfloat* pen_color,float linewidth):Graphic(pen_color[0],pen_color[1],pen_color[2],linewidth){
    float x = (g_x-WIDTH/2)/(WIDTH/2);
    float y = -(g_y-HEIGHT/2)/(HEIGHT/2);
    Node* gp = new Node(x, y);
    node_list.push_back(gp);
}