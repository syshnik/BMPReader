#include "stdafx.h"
#include "BMPReaderClass.h"
#include <iostream>
#include <fstream>
using namespace std;

BMPReaderClass::BMPReaderClass(void)
{
}


BMPReaderClass::~BMPReaderClass(void)
{
}


bool BMPReaderClass::Read(char* bmpfile)
{
	// читаем файл в буфер
	bool ok=false;
	_bufer.resize(0);
	std::ifstream f1;       // поток для чтения
	f1.open (bmpfile, ios::in |ifstream::binary);
    if (f1.is_open())
    {
		f1.seekg(0, ios::beg);
		char ch;
		while (f1.get(ch)) {
			_bufer.resize(_bufer.size()+1);
			_bufer[_bufer.size()-1]=ch;
		}
	    f1.close();     // закрываем файл	
		ok=RazborFile();
    } else {
		printf("Error open file.");
	}
	return ok;
}


bool BMPReaderClass::RazborFile(void)
{
	// разбираем буфер картинки
	char* p=&_bufer[0];
	long size=_bufer.size();
	// В начале стоит заголовок файла (BITMAPFILEHEADER). 
	if(size < sizeof(_s1)) return false;
	_s1=*((BITMAPFILEHEADER *)p);
	size -= sizeof(_s1);
	p += sizeof(_s1);
	// А дальше идет структура BITMAPINFOHEADER
	if(size < sizeof(_s2)) return false;
	_s2=*((BITMAPINFOHEADER *)p);
	size -= sizeof(_s2);
	//длинн строки
	_StringSize = (_s2.biWidth * (_s2.biBitCount/ 8) + 3) & ~3;
	return TestFormat();
}


void BMPReaderClass::PrintPic(void)
{
	// печатаем картинку в консоли
	if(!TestFormat()) return;
	printf("\n");
	for(int y=0; y < min(60000, _s2.biHeight); y++) {
		for(int x=0; x < min(12000, _s2.biWidth); x++) {
			if(GetBit(x, y)) printf("8");
			else printf("-");
		}
		printf("\n");
	}
}


bool BMPReaderClass::GetBit(int x, int y)
{
	//выдает цвет  катинки в точке x, y
	int adress=GetMemAdress(x, y);
	unsigned char * p=(unsigned char *)&_bufer[adress];
	int srednee=0;
	if(_s2.biBitCount==24) {
		srednee=(p[0]+p[1]+p[2])/3;
	} else if(_s2.biBitCount==32) {
		srednee=(p[0]+p[1]+p[2]+p[3])/4;
	}
	return (srednee > 255/2);
}


int BMPReaderClass::GetMemAdress(int x, int y)
{
	// выдает адрес точки в памяти
	if ( x >= _s2.biWidth || y >= _s2.biHeight) return _s1.bfOffBits;
	int adress=_s1.bfOffBits+_StringSize*(_s2.biHeight-y-1)+x*_s2.biBitCount/8;
	return adress;
}


void BMPReaderClass::InvertBit(int x, int y)
{
	// инвертирует цвет в точке x,y
	bool bit=GetBit(x, y);
	SetBit(!bit, x, y);
}


void BMPReaderClass::SetBit(bool bit, int x, int y)
{
	//устанавливает цвет  катинки в точке x, y
	int adress=GetMemAdress(x, y);
	unsigned char * p=(unsigned char *)&_bufer[adress];
	unsigned char newnote=(bit ? 255 : 0);
	if(_s2.biBitCount==24) {
		p[0]=newnote;
		p[1]=newnote;
		p[2]=newnote;
	} else if(_s2.biBitCount==32) {
		p[0]=newnote;
		p[1]=newnote;
		p[2]=newnote;
		p[3]=newnote;
	}

}


void BMPReaderClass::Krest(int x1, int y1, int x2, int y2)
{
	// рисуем крестик
	if(!TestFormat()) return;
	// подровняем
	x1=max(0, x1);
	x1=min(x1, _s2.biWidth);
	x2=max(0, x2);
	x2=min(x2, _s2.biWidth);
	y1=max(0, y1);
	y1=min(y1, _s2.biWidth);
	y2=max(0, y2);
	y2=min(y2, _s2.biWidth);
	if(x1>x2) swap( x1, x2 );
	if(y1>y2) swap( y1, y2 );
	// толщина линии
	float dopusk=((float)max((x2-x1), (y2-y1)))/20;
	dopusk=max(dopusk, 1.2);
	// ходим по массиву x1-x2:y1-y2 и если точка достаточно приближена к линиям креста меняем цвет
	for(int x=x1; x<x2; x++) {
		for(int  y=y1; y < y2; y++) {
			float d1=Dopusk((float) x, (float) y, (float) x1, (float) y1, (float) x2, (float) y2);
			if (d1 < dopusk) {
				InvertBit(x, y);
				continue;
			}
			d1=Dopusk((float) x, (float) y, (float) x2, (float) y1, (float) x1, (float) y2);
			if (d1 < dopusk) {
				InvertBit(x, y);
				continue;
			}

		}
	}

}


void BMPReaderClass::RandKrest(void)
{
	// для теста... случайные координаты крестика
	if(!TestFormat()) return;
	int x1=0, x2=0, y1=0, y2=0;
	x1=rand()%(_s2.biWidth);
	x2=rand()%(_s2.biWidth);
	y1=rand()%(_s2.biHeight);
	y2=rand()%(_s2.biHeight);
	Krest(x1, y1, x2, y2);
}


float BMPReaderClass::Dopusk(float x, float y, float x1, float y1, float x2, float y2)
{
	// определение расстояния от точки x, y до отрезка x1, y1, x2, y2
	float len = sqrt((y2 - y1) * (y2 - y1) + (y2 - y1) * (y2 - y1));
	float distance = abs((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - x1 * y2) / len;
	return distance;
}


bool BMPReaderClass::Write(char * filename)
{
	// сбросим буфер в файл
	if(!TestFormat()) return false;
	bool ok=false;
	std::ofstream f1;       // поток для чтения
	f1.open (filename, ifstream::binary);
    if (f1.is_open())
    {
		f1.write((char*)(&_bufer[0]), _bufer.size());
	    f1.close();     // закрываем файл	
		ok=true;
    } else {
		printf("Error write file.");
	}
	return ok;
}


bool BMPReaderClass::TestFormat(void)
{
	// проверка формата картинки в буфере
	// проверим размеры
	long size5=_s1.bfOffBits+_StringSize*_s2.biHeight;
	if(size5 > _bufer.size()) return false;
	//битность по тз
	if(_s2.biBitCount!=24 && _s2.biBitCount!=32) return false;
	// ok
	return true;
}
