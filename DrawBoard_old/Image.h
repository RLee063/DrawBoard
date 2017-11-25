#include <GL/glut.h>
#include "Graphic.h"
#include "Rectangle.h"
#define WIDTH 1000
#define HEIGHT 1000
class Image:public Rectangle{
public:
    Image(float,float,GLfloat*,bool,float);
    virtual void draw();
    virtual void update(float, float){};
    GLint imagewidth;
    GLint imageheight;
    GLint pixellength;
    GLubyte* pixeldata;
    char path[100];
};

void Image::draw(){
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
    glRasterPos2f(parrallel_mP[0],parrallel_mP[1]);
    glDrawPixels(imagewidth,imageheight,GL_BGR_EXT,GL_UNSIGNED_BYTE,pixeldata);
    if(is_select){
        glPopMatrix();
    }
}

Image::Image(float x, float y, GLfloat* pen_color, bool fill, float lineWidth)
:Rectangle(x,y,pen_color,fill,lineWidth){
    cout << "Please input the path of the image:";
    cin >> path;
    FILE *pfile = fopen(path, "rb");
    if (pfile == 0)
    {
        cout<<"No such a image here!"<<endl;
        exit(0);
    }

    //读取图像大小
    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
    fread(&imageheight, sizeof(imageheight), 1, pfile);

    //计算像素数据长度
    pixellength = imagewidth * 3;
    while (pixellength % 4 != 0)
        pixellength++;
    pixellength *= imageheight;

    //读取像素数据
    pixeldata = (GLubyte *)malloc(pixellength);
    if (pixeldata == 0)
        exit(0);

    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);

    //关闭文件
    fclose(pfile);
    cout << imageheight << endl;
    cout << imagewidth << endl;
    originP[0] = (x-WIDTH/2)/(WIDTH/2);
    originP[1] = -(y-HEIGHT/2)/(HEIGHT/2)+(float)imageheight/(HEIGHT/2);
    mouseP[0] = (x-WIDTH/2)/(WIDTH/2)+(float)imagewidth/(WIDTH/2);
    mouseP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
    parrallel_oP[0] =  (x-WIDTH/2)/(WIDTH/2)+(float)imagewidth/(WIDTH/2);
    parrallel_oP[1] = -(y-HEIGHT/2)/(HEIGHT/2)+(float)imageheight/(HEIGHT/2);
    parrallel_mP[0] =  (x-WIDTH/2)/(WIDTH/2);
    parrallel_mP[1] = -(y-HEIGHT/2)/(HEIGHT/2);
}