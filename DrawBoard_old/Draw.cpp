#include <GL/glut.h>
#include "Board.h"
using namespace std;


Board *myBoard = new Board();
bool mouseleftdown;
bool mouserightdown;

void myDisplay(void){
    myBoard->draw();
    myBoard->show_information();
}

void myKeyboradFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    // case 'q':
    //     myBoard->setBrushTypes(1);
    //     break;
    case 'w':
        myBoard->setBrushTypes(2);
        break;
    case 'e':
        myBoard->setBrushTypes(3);
        break;
    case 'r':
        myBoard->setBrushTypes(4);
        break;
    // case 't':
    //     myBoard->setBrushTypes(5);
    //     break;
    // case 'y':
    //     myBoard->setBrushTypes(6);
    //     break;
    case 'c':
        myBoard->setColor();
        break;
    case 'f':
        myBoard->setFill();
        break;
    case '[':
        myBoard->setLineWidth(false);
        break;
    case ']':
        myBoard->setLineWidth(true);
        break;
    case 'p':
        myBoard->setWorkTypes();
        break;
    case 'l':
        myBoard->save();
        myBoard->saveAsInfo();
        break;
    case 'o':
        myBoard->loadInfo();
        break;
    }
}

void mouseCB(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            myBoard->mouseLeftDown(x, y);
            mouseleftdown = true;
        }
        else if(state == GLUT_UP){
             myBoard->mouseLeftUp(x, y);
             mouseleftdown = false;
        }
    }
    if(button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_DOWN){
            myBoard->mouseRightDown(x ,y);
        }
    }
}

void mouseMotionCB(int x, int y){
    if(mouseleftdown){
        myBoard->mouseMotion(x, y);
    }
    glutPostRedisplay();
}

int main(int argc, char **argv){
    /*��ʼ��*/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //˫���塢RGBģʽ
    glutInitWindowSize(myBoard->windowWidth, myBoard->windowHeight);
    glutInitWindowPosition(600, 100);
    glutCreateWindow("�������ʾ��"); //���ڱ���
    /*��������ʾ*/
    //glutReshapeFunc(myReshape); //���ڴ�С�����ı�ʱ��ȡ����Ϊ
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
    glutKeyboardFunc(myKeyboradFunc);
    glutDisplayFunc(myDisplay); //��ʾ����ͼ��
    glutMainLoop(); //ѭ��
    return (0);
}