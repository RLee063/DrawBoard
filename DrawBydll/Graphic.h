#pragma once
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <algorithm>
#define WIDTH 1000
#define HEIGHT 1000
using namespace std;
struct Node {
	float x;
	float y;
	Node(float gx, float gy) {
		x = gx;
		y = gy;
	}
};

class Graphic {
public:
	Graphic(float r, float g, float b, float linewidth);
	Graphic() {};
	virtual bool detect(float, float);
	virtual void select(float, float);	//* moveInit
	virtual void moveInit(float, float);
	virtual void moveUpdate(float, float);
	virtual void moveFinish() {};
	virtual void mouseLeftUp() {};		//* moveFinish();
	virtual void mouseLeftDown() {};
	virtual void mouseRightDown(float, float) {};   //just for polygon now
	virtual void mouseMotion(float, float) {};
	virtual void saveAsInfo(char*) {};
	virtual void unSelect();
	virtual void draw() = 0;
protected:
	bool is_fill;
	float lineWidth;
	bool is_adding;
	bool is_select; //for moving
	int typeName;
	GLfloat color[3];
	GLfloat minPoint[2];
	GLfloat minEdge[2];
	GLfloat moveIncrement[2];
	GLfloat moveOriginalPoint[2];
	virtual void mpUpdate() {};
	virtual void addFinish() {};
	virtual void update(float, float) {};
};

Graphic::Graphic(float r, float g, float b, float linewidth) :
	lineWidth(linewidth),
	is_select(0)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	moveIncrement[0] = 0;
	moveIncrement[1] = 0;
}

bool Graphic::detect(float gx, float gy) {
	float x = (gx - WIDTH / 2) / (WIDTH / 2);
	float y = -(gy - HEIGHT / 2) / (HEIGHT / 2);
	if (x>minPoint[0] && x<(minPoint[0] + minEdge[0])) {
		if (y>minPoint[1] && y<(minPoint[1] + minEdge[1])) {
			return true;
		}
	}
	return false;
}

void Graphic::moveInit(float gx, float gy) {
	float x = (gx - WIDTH / 2) / (WIDTH / 2);
	float y = -(gy - HEIGHT / 2) / (HEIGHT / 2);
	moveOriginalPoint[0] = x;
	moveOriginalPoint[1] = y;
	is_select = 1;
}

void Graphic::moveUpdate(float gx, float gy) {
	float x = (gx - WIDTH / 2) / (WIDTH / 2);
	float y = -(gy - HEIGHT / 2) / (HEIGHT / 2);
	moveIncrement[0] = x - moveOriginalPoint[0];
	moveIncrement[1] = y - moveOriginalPoint[1];
}

void Graphic::select(float x, float y) {
	moveInit(x, y);
}

void Graphic::unSelect() {
	is_select = 0;
}