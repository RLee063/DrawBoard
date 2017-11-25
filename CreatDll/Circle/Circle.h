#pragma once

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

#define BUILD_DLL
#ifdef BUILD_DLL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI 
#endif // BUILD_DLL

// DLLµ¼³öº¯Êý

#ifdef __cplusplus
extern "C" {
#endif
	class Circle : public Graphic {
	public:
		DLLAPI Circle(float x, float y, GLfloat*, bool fill, float linewidth);
		//Circle(GLfloat* position, GLfloat* pen_color, float lineWidth, bool fill);
		DLLAPI Circle(GLfloat* infomation);
		virtual DLLAPI void draw();
		virtual DLLAPI void addUpdate(float, float);
		virtual DLLAPI void addFinish();
		virtual DLLAPI void moveFinish();
		virtual DLLAPI void mpUpdate();
		virtual DLLAPI void saveInfo(char*);
		virtual DLLAPI void mouseLeftUp();		//* moveFinish();
		virtual DLLAPI void mouseMotion(float, float);
		virtual DLLAPI void mouseLeftDown(float, float);
		virtual DLLAPI void mouseRightDown(float, float) {};   //just for polygon now

		float radius;
		GLfloat originP[2];
	};
	DLLAPI Graphic *getObj(float x, float y, GLfloat*, bool fill, float linewidth);
	DLLAPI Graphic *getObjByInfo(GLfloat * imfomation);
	DLLAPI const char * getName();

#ifdef __cplusplus
}
#endif

