#include <iostream>
#include <GL/glut.h>
#include <malloc.h>
#include <list>
#include "Graphic.h"
#include "Line.h"
#include "FreeLine.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Polygon.h"
#include "Image.h"
#define WIDTH 1000
#define HEIGHT 1000
#define BMP_Header_Length 54 
using namespace std;

class Board{
public:
    list<Graphic *> graphic_list;
    Graphic* now_graphic;
    GLfloat pen_color[3];
    int windowWidth;
    int windowHeight;
    bool is_drawing;
    bool is_fill;
    float lineWidth;
    float auto_linewidth_incremental;
    enum g_types{freeLine=1, line, rectangle, circle, polygon, image}graphic_type, load_graphic_type;
    enum w_types{creat=1, move}work_type;

    Board();
    void draw(); 
    void mouseLeftDown(float x, float y);
    void mouseLeftUp(float x, float y);
    void mouseRightDown(float x, float y);
    void mouseMotion(float x, float y);
    void add(float x, float y);
    void add_finished();
    void show_information();
    void setBrushTypes(int type);
    void setColor();
    void setFill();
    void setLineWidth(bool);
    void setWorkTypes();
    void save();
    void saveAsInfo();
    void loadInfo();
    bool DetectSelection(float x, float y);
};

Board::Board()
    : auto_linewidth_incremental(0.5),
      work_type(creat),
      lineWidth(1),
      is_drawing(0),
      is_fill(0),
      now_graphic(NULL),
      graphic_type(line),
      windowWidth(1000),
      windowHeight(1000)
{
    pen_color[0] = 1;
    pen_color[1] = 1;
    pen_color[2] = 1;
}

void Board::loadInfo(){
    system("clear");
    cout << "######################Load info###################" << endl;
    char path[50];
    cout << "Input the load path:";
    cin >> path;
    ifstream in(path);
    if(!in.is_open()){
        cout << "No such a file." << endl;
        return ;
    }

    int typeName;
    int n;
    GLfloat infomation[20];
    Graphic* tgp;

    while(1){
        in >> typeName;
        load_graphic_type = g_types(graphic_type);
        switch(typeName){
        case line://line
            in >> n;
            for(int i=0; i<n; i++){
                in >> infomation[i];
            }
            tgp = new Line(infomation);
            graphic_list.push_back(tgp);
            break;
        case rectangle://rectangle
            in >> n;
            for(int i=0; i<n; i++){
                in >> infomation[i];
            }
            tgp = new Rectangle(infomation);
            graphic_list.push_back(tgp);
            break;
        case circle://circle
            in >> n;
            for(int i=0; i<n; i++){
                in >> infomation[i];
            }
            tgp = new Circle(infomation);
            graphic_list.push_back(tgp);
            break;
            default:
            in.close();
            show_information();
            return;
        }
        typeName = 0;
    }
    in.close();
    show_information();
    return;
}

void Board::saveAsInfo(){
    system("clear");
    cout << "######################Save info###################" << endl;
    char path[50];
    cout << "Input the save path:";
    cin >> path;
    ofstream out(path);
    if(!out.is_open()){
        cout << "No such a file." << endl;
        return ;
    }
    out.close();
    for(auto i=graphic_list.begin(); i!=graphic_list.end(); i++){
        (*i)->saveInfo(path);
    }
}

void Board::draw(){
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    for(auto i = graphic_list.begin(); i!=graphic_list.end(); i++){
        (*i)->draw();
    }
    glutSwapBuffers();
}

void Board::mouseLeftUp(float x, float y){
    switch(work_type){
    case creat:
        if(now_graphic!=NULL){
            add_finished();
        }
        break;
    case move:
        if(now_graphic!=NULL){
            now_graphic->moveFinish();
        }
        now_graphic = NULL;
        break;
    }
    draw();
}

void Board::mouseRightDown(float x, float y){
    switch(work_type){
    case creat:
        if(now_graphic!=NULL){
            now_graphic->mouseRightDown(x, y);//special act
        }
        break;
    case move:
        /////////////////////////////////////////////////
        break;
    }
}

void Board::mouseLeftDown(float x, float y){
    switch(work_type){
    case creat:
        add(x, y);
        break;
    case move:
        if(DetectSelection(x, y)){
            now_graphic->moveInit(x, y);
        }
        else{
            if(now_graphic!=NULL){
                now_graphic->unSelect();
                now_graphic = NULL;
            }
        }
        break;
    }
}

void Board::mouseMotion(float x, float y){
    switch(work_type){
    case creat:
        if(now_graphic!=NULL){
            now_graphic->update(x, y);
        }
        break;
    case move:
        if(now_graphic!=NULL){
            now_graphic->moveUpdate(x, y);
        }
        break;
    }
}

void Board::add_finished(){
    now_graphic->addFinish();
    is_drawing = 0;
    now_graphic = NULL;
    draw();
}

void Board::add(float x, float y){
    switch (graphic_type){
    case freeLine:
        now_graphic = new FreeLine(x, y, pen_color, lineWidth);
        break;
    case line:
        now_graphic = new Line(x, y, pen_color, lineWidth);
        break;
    case rectangle:
        now_graphic = new Rectangle(x, y, pen_color, is_fill, lineWidth);
        break;
    case circle:
        now_graphic = new Circle(x, y, pen_color, is_fill, lineWidth);
        break;
    case polygon:
        now_graphic = new Polygon(x, y, pen_color, is_fill, lineWidth);
        break;
    case image:
        now_graphic = new Image(x, y, pen_color, is_fill, lineWidth);
        break;
    }
    graphic_list.push_back(now_graphic);
    is_drawing = 1;
}

void Board::setLineWidth(bool is_increase){
    if(is_increase){
        lineWidth += auto_linewidth_incremental;
    }
    else{
        if(lineWidth>auto_linewidth_incremental){
            lineWidth -= auto_linewidth_incremental;
        }
    }
    show_information();
}

void Board::setBrushTypes(int type){
    graphic_type = (g_types)type;
    show_information();
}

void Board::setColor(){
    float r,g,b;
    cout<<"Input the color you want:";
    cin>>r>>g>>b;
    pen_color[0] = r;
    pen_color[1] = g;
    pen_color[2] = b;
    show_information();
}

void Board::setFill(){
    is_fill = !is_fill;
    show_information();
}

void Board::setWorkTypes(){
    //strange
    if(work_type == creat){
        work_type = move;
    }
    else{
        work_type = creat;
    }
    show_information();
}

void Board::show_information(){
    system("clear");
    if(work_type == move){
        cout<<"The current type of work is: Adjustment."<<endl;
        cout<<"Press 'p' to change the type of work to Create."<<endl;
        return;
    }
    else{
        cout<<"The current type of work is: Create."<<endl;
        cout<<"Press 'p' to change the type of work to Adjustment."<<endl;
    }
    switch (graphic_type){
    case freeLine:
        cout<<"The current type of brush is:FreeLine."<<endl;
        break;
    case line:
        cout<<"The current type of brush is:StraightLine."<<endl;
        break;
    case rectangle:
        cout<<"The current type of brush is:Rectangle."<<endl;
        break;
    case circle:
        cout<<"The current type of brush is:Circle."<<endl;
        break;
    case polygon:
        cout<<"The current type of brush is:Polygon."<<endl;
    break;
    case image:
        cout<<"The current type of brush is:Image."<<endl;
    break;
    }
    cout<<"The current color of brush is:"<<pen_color[0]<<" "<<pen_color[1]<<" "<<pen_color[2]<<", ";
    if(is_fill){
        cout<<"fill the polygon."<<endl;
    }
    else{
        cout<<"dont fill the polygon."<<endl;
    }
    cout<<"The linewidth is:"<<lineWidth<<", "<<"press '[' to decrease ']' to increase."<<endl;
    cout<<"Press 'q' to make the type change to freeline,'w' change to straight,'e' change to rectangle, 'r' change to circle, 't' change to polygon."<<endl;
    cout<<"Press 'c' to change to color of brush."<<endl;
    cout<<"Press 'l' to save as info."<<endl;
    cout<<"Press 'o' to load  info."<<endl;
}

void Board::save(){
    FILE*    pDummyFile;  //ָ����һbmp�ļ������ڸ��������ļ�ͷ����Ϣͷ����  
    FILE*    pWritingFile;  //ָ��Ҫ�����ͼ��bmp�ļ�  
    GLubyte* pPixelData;    //ָ���µĿյ��ڴ棬���ڱ����ͼbmp�ļ�����  
    GLubyte  BMP_Header[BMP_Header_Length];  
    GLint    i, j;  
    GLint    PixelDataLength;   //BMP�ļ������ܳ���  
    
    // �����������ݵ�ʵ�ʳ���  
    i = WIDTH * 3;   // �õ�ÿһ�е��������ݳ���  
    while( i%4 != 0 )      // �������ݣ�ֱ��i�ǵı���  
        ++i;                
    PixelDataLength = i * HEIGHT;  //��������λ��  
    
    // �����ڴ�ʹ��ļ�  
    pPixelData = (GLubyte*)malloc(PixelDataLength);  
    if( pPixelData == 0 )  
        exit(0);  
    
    pDummyFile = fopen("/media/lee/D/UBuNTUCODE/Draw/bitmapH.bmp", "rb");//ֻ����ʽ��,��ȡbmp�ļ���Header
    if( pDummyFile == NULL )  
        exit(0);  
    
    pWritingFile = fopen("/media/lee/D/UBuNTUCODE/Draw/grab.bmp", "wb"); //ֻд��ʽ��  
    if( pWritingFile == 0 )  
        exit(0);  
    
    //�Ѷ����bmp�ļ����ļ�ͷ����Ϣͷ���ݸ��ƣ����޸Ŀ������  
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //��ȡ�ļ�ͷ����Ϣͷ��ռ��54�ֽ�  
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);  
    fseek(pWritingFile, 0x0012, SEEK_SET); //�ƶ���0X0012����ָ��ͼ���������ڴ�  
    i = WIDTH;  
    j = HEIGHT;  
    fwrite(&i, sizeof(i), 1, pWritingFile);  
    fwrite(&j, sizeof(j), 1, pWritingFile);  
    
    // ��ȡ��ǰ������ͼ�����������  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //����4λ���뷽ʽ  
    glReadPixels(0, 0, WIDTH, HEIGHT,  
        GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);      
    
    // д����������  
    fseek(pWritingFile, 0, SEEK_END);  
    //��������BMP�ļ�����д��pWritingFile  
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);   
    
    // �ͷ��ڴ�͹ر��ļ�  
    fclose(pDummyFile);  
    fclose(pWritingFile);  
    free(pPixelData);  
}

bool Board::DetectSelection(float x, float y){
    auto i = graphic_list.end();
    for(--i; i!=graphic_list.begin(); i--){
        if((*i)->detect(x, y)){
            now_graphic = *i;
            return true;
        }
    }
    i = graphic_list.begin();   //for first
    if((*i)->detect(x, y)){
            now_graphic = *i;
            return true;
    }
    return false;
}
