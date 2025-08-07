#pragma once
#include <vector>

#include <windows.h>


class BMPReaderClass
{
private:
	std::vector<char> _bufer;
	BITMAPFILEHEADER _s1;
	BITMAPINFOHEADER _s2;
	long _StringSize;
	bool RazborFile(void);
	bool GetBit(int x, int y);
	int GetMemAdress(int x, int y);
	void InvertBit(int x, int y);
	void SetBit(bool bit, int x, int y);
	float Dopusk(float x, float y, float x1, float y1, float x2, float y2);
public:
	BMPReaderClass(void);
	~BMPReaderClass(void);
	bool Read(char* bmpfile);
	void PrintPic(void);
	void Krest(int x1, int y1, int x2, int y2);
	void RandKrest(void);
	bool Write(char * filename);
	bool TestFormat(void);
};

