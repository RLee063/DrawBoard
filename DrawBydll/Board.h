#pragma once
#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <malloc.h>
#include <Windows.h>
#include <fstream>
#include <list>
#include "Graphic.h"
#define WIDTH 1000
#define HEIGHT 1000
#define BMP_Header_Length 54 
class Board {
public:
	vector<string> name_list;
	vector<HINSTANCE> modules;
	list<Graphic *> graphic_list;
	Graphic* now_graphic;
	GLfloat pen_color[3];
	int windowWidth;
	int windowHeight;
	int graphic_type;
	int load_graphic_type;
	bool is_drawing;
	bool is_fill;
	float lineWidth;
	float auto_linewidth_incremental;
	enum w_types { creat = 1, move }work_type;

	Board();
	void draw();
	void mouseLeftDown(float x, float y);
	void mouseLeftUp(float x, float y);
	void mouseRightDown(float x, float y);
	void mouseMotion(float x, float y);
	void add(float x, float y);
	void addFinished();
	void setBrushTypes(int type);
	void setColor();
	void setFill();
	void setLineWidth(bool);
	void setWorkTypes();
	void saveAsImage();
	void saveAsInfo();
	void loadInfo();
	void showInformation();
	bool DetectSelection(float x, float y);
	void getPlugins();
	Graphic * getObj(int, float, float, GLfloat*, bool, float);
	Graphic * getObj(int, GLfloat*);
};

using namespace std;
Board::Board()
	: auto_linewidth_incremental(0.5),
	work_type(creat),
	lineWidth(1),
	is_drawing(0),
	is_fill(0),
	now_graphic(NULL),
	graphic_type(1),
	windowWidth(1000),
	windowHeight(1000)
{
	pen_color[0] = 1;
	pen_color[1] = 1;
	pen_color[2] = 1;
}

void Board::draw() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	for (auto i = graphic_list.begin(); i != graphic_list.end(); i++) {
		(*i)->draw();
	}
	glutSwapBuffers();
}

void Board::mouseLeftDown(float x, float y) {
	switch (work_type) {
	case creat:
		add(x, y);
		break;
	case move:
		if (DetectSelection(x, y)) {
			now_graphic->select(x, y);
		}
		else {
			if (now_graphic != NULL) {
				now_graphic->unSelect();
				now_graphic = NULL;
			}
		}
		break;
	}
}

void Board::mouseLeftUp(float x, float y) {
	switch (work_type) {
	case creat:
		if (now_graphic != NULL) {
			now_graphic->mouseLeftUp();
			addFinished();
		}
		break;
	case move:
		if (now_graphic != NULL) {
			now_graphic->mouseLeftUp();
		}
		now_graphic = NULL;
		break;
	}
	draw();
}

void Board::mouseRightDown(float x, float y) {
	switch (work_type) {
	case creat:
		if (now_graphic != NULL) {
			now_graphic->mouseRightDown(x, y);//special act
		}
		break;
	case move:
		/////////////////////////////////////////////////
		break;
	}
}

void Board::mouseMotion(float x, float y) {
	switch (work_type) {
	case creat:
		if (now_graphic != NULL) {
			now_graphic->mouseMotion(x, y);
		}
		break;
	case move:
		if (now_graphic != NULL) {
			now_graphic->mouseMotion(x, y);
		}
		break;
	}
}

void Board::add(float x, float y) {
	now_graphic = getObj(graphic_type, x, y, pen_color, is_fill, lineWidth);
	graphic_list.push_back(now_graphic);
	is_drawing = 1;
}

void Board::addFinished() {
	is_drawing = 0;
	now_graphic = NULL;
	draw();
}

void Board::setLineWidth(bool is_increase) {
	if (is_increase) {
		lineWidth += auto_linewidth_incremental;
	}
	else {
		if (lineWidth>auto_linewidth_incremental) {
			lineWidth -= auto_linewidth_incremental;
		}
	}
	showInformation();
}

void Board::setBrushTypes(int type) {
	graphic_type = type;
	showInformation();
}

void Board::setColor() {
	float r, g, b;
	cout << "Input the color you want:";
	cin >> r >> g >> b;
	pen_color[0] = r;
	pen_color[1] = g;
	pen_color[2] = b;
	showInformation();
}

void Board::setFill() {
	is_fill = !is_fill;
	showInformation();
}

void Board::setWorkTypes() {
	//strange
	if (work_type == creat) {
		work_type = move;
	}
	else {
		work_type = creat;
	}
	showInformation();
}

void Board::saveAsImage() {
	FILE*    pDummyFile;  //指向另一bmp文件，用于复制它的文件头和信息头数据  
	FILE*    pWritingFile;  //指向要保存截图的bmp文件  
	GLubyte* pPixelData;    //指向新的空的内存，用于保存截图bmp文件数据  
	GLubyte  BMP_Header[BMP_Header_Length];
	GLint    i, j;
	GLint    PixelDataLength;   //BMP文件数据总长度  

								// 计算像素数据的实际长度  
	i = WIDTH * 3;   // 得到每一行的像素数据长度  
	while (i % 4 != 0)      // 补充数据，直到i是的倍数  
		++i;
	PixelDataLength = i * HEIGHT;  //补齐后的总位数  

								   // 分配内存和打开文件  
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	pDummyFile = fopen("/media/lee/D/UBuNTUCODE/Draw/bitmapH.bmp", "rb");//只读形式打开,获取bmp文件的Header
	if (pDummyFile == NULL)
		exit(0);

	pWritingFile = fopen("/media/lee/D/UBuNTUCODE/Draw/grab.bmp", "wb"); //只写形式打开  
	if (pWritingFile == 0)
		exit(0);

	//把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据  
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节  
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存  
	i = WIDTH;
	j = HEIGHT;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 读取当前画板上图像的像素数据  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式  
	glReadPixels(0, 0, WIDTH, HEIGHT,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 写入像素数据  
	fseek(pWritingFile, 0, SEEK_END);
	//把完整的BMP文件数据写入pWritingFile  
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件  
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}

void Board::saveAsInfo() {
	system("cls");
	cout << "######################Save info###################" << endl;
	char path[50];
	cout << "Input the save path:";
	cin >> path;
	ofstream out(path);
	if (!out.is_open()) {
		cout << "No such a file." << endl;
		return;
	}
	out.close();
	for (auto i = graphic_list.begin(); i != graphic_list.end(); i++) {
		(*i)->saveAsInfo(path);
	}
}

void Board::loadInfo() {
	system("cls");
	cout << "######################Load info###################" << endl;
	char path[50];
	cout << "Input the load path:";
	cin >> path;
	ifstream in(path);
	if (!in.is_open()) {
		cout << "No such a file." << endl;
		return;
	}

	int typeName;
	int n;
	GLfloat infomation[20];
	Graphic* tgp;

	while (1) {
		in >> typeName;
		load_graphic_type = graphic_type;
		in >> n;
		for (int i = 0; i<n; i++) {
			in >> infomation[i];
		}
		tgp = getObj(graphic_type, infomation);
		graphic_list.push_back(tgp);
		typeName = 0;
	}
	in.close();
	showInformation();
	return;
}

void Board::showInformation() {
	system("cls");
	if (work_type == move) {
		cout << "The current type of work is: Adjustment." << endl;
		cout << "Press 'p' to change the type of work to Create." << endl;
		return;
	}
	else {
		cout << "The current type of work is: Create." << endl;
		cout << "Press 'p' to change the type of work to Adjustment." << endl;
	}
	cout << "The current type of brush is:" << name_list[graphic_type-1] << endl;

	cout << "The current color of brush is:" << pen_color[0] << " " << pen_color[1] << " " << pen_color[2] << ", ";
	if (is_fill) {
		cout << "fill the polygon." << endl;
	}
	else {
		cout << "dont fill the polygon." << endl;
	}
	cout << "The linewidth is:" << lineWidth << ", " << "press '[' to decrease ']' to increase." << endl;
	for (int i = 0; i < (int)name_list.size(); i++) {
		cout << "press" << i << "change to" << name_list[i] << endl;
	}
	cout << "Press 'c' to change to color of brush." << endl;
	cout << "Press 'l' to save as info." << endl;
	cout << "Press 'o' to load  info." << endl;
}

bool Board::DetectSelection(float x, float y) {
	auto i = graphic_list.end();
	for (--i; i != graphic_list.begin(); i--) {
		if ((*i)->detect(x, y)) {
			now_graphic = *i;
			return true;
		}
	}
	i = graphic_list.begin();   //for first
	if ((*i)->detect(x, y)) {
		now_graphic = *i;
		return true;
	}
	return false;
}

Graphic* Board::getObj(int index, float x, float y, GLfloat* color, bool is_fill, float lineWidth) {
	int i = index - 1;
	HINSTANCE mod = modules[i];

	typedef Graphic* (__cdecl *ObjProc)(float, float, GLfloat*, bool, float);
	typedef const char* (__cdecl *NameProc)(void);

	ObjProc objFunc = (ObjProc)GetProcAddress(mod, "getObj");
	//NameProc nameFunc = (NameProc)GetProcAddress(mod, "getName");

	return objFunc(x, y, color, is_fill, lineWidth);
}

Graphic* Board::getObj(int index, GLfloat * information) {
	int i = index - 1;
	HINSTANCE mod = modules[i];

	typedef Graphic* (__cdecl *ObjProc)(float *);
	typedef const char* (__cdecl *NameProc)(void);

	ObjProc objFunc = (ObjProc)GetProcAddress(mod, "getObjByInfo");
	//NameProc nameFunc = (NameProc)GetProcAddress(mod, "getName");

	return objFunc(information);
}

void Board::getPlugins() {
	modules.clear();
	// 在plugins目录中查找dll文件并将文件信息保存在fileData中
	WIN32_FIND_DATA fileData;
	HANDLE fileHandle = FindFirstFile(L"plugins/*.dll", &fileData);

	if (fileHandle == (void*)ERROR_INVALID_HANDLE ||
		fileHandle == (void*)ERROR_FILE_NOT_FOUND) {
		// 没有找到任何dll文件，返回空vector
		return;
	}

	// 循环加载plugins目录中的所有dll文件
	do {
		typedef const char* (__cdecl *NameProc)(void);

		// 将dll加载到当前进程的地址空间中
		HINSTANCE mod = LoadLibrary((L"./plugins/" + std::wstring(fileData.cFileName)).c_str());

		//if (!mod) {
		//	// 加载dll失败，则释放所有已加载dll *不会发生
		//	for (HINSTANCE hInst : modules)
		//		FreeLibrary(hInst);
		//	throw MyException(L"Library " + std::wstring(fileData.cFileName) + L" wasn't loaded successfully!");
		//}
		// 从dll句柄中获取getObj和getName的函数地址
		//		ObjProc objFunc = (ObjProc)GetProcAddress(mod, "getObj");
		NameProc nameFunc = (NameProc)GetProcAddress(mod, "getName");

		//		if (!objFunc || !nameFunc)
		//			throw std::runtime_error("Invalid Plugin DLL: both 'getObj' and 'getName' must be defined.");

		//		ret.push_back(objFunc());  // 保存objFunc（即getObj）生成的对象指针
		modules.push_back(mod);  // 保存dll句柄
		name_list.push_back(nameFunc());
	} while (FindNextFile(fileHandle, &fileData));

	//	std::clog << std::endl;

	// 关闭文件句柄
	FindClose(fileHandle);
	return;
}