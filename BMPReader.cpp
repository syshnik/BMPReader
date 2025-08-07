// BMPReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BMPReaderClass.h"
#include <iostream>
#include <conio.h> 
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
	// 120*30
	BMPReaderClass br;
	for(;;) {
		// чтение файла
		std::cout << "Enter input BMP file name (exit-for exit): ";
		std::string filein, fileout;
		std::cin >> filein;
		if(filein=="exit") return 0;
		if (br.Read((char*)filein.data())) {
			// вывод картинки
			br.PrintPic();
			// рисуем крестик
			std::cout << "Enter x1, y1, x2, y2: ";
			int x1, y1, x2, y2;
			std::cin >> x1 >> y1 >> x2 >> y2;
			br.Krest(x1, y1, x2, y2);
			br.PrintPic();
			// сохранним
			std::cout << "Enter output BMP file name (exit-for exit): ";
			std::cin >> fileout;
			if(fileout=="exit") return 0;
			br.Write((char *)fileout.data());
		} else {
			std::cout << "Error open file " << filein.data();
		}
	}
	/*
	if (br.Read("C:/vrem/32.bmp")) {
		br.PrintPic();
		br.RandKrest();
		br.PrintPic();
		br.Write("C:/vrem/32_krest.bmp");
	}
	*/
	return 0;
}

