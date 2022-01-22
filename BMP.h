#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40
#define NO_COMPRESION 0
#define MAX_NUMBER_OF_COLORS 0
#define ALL_COLORS_REQUIRED 0


#include <iostream>
#include <vector>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Geometry.h"

using namespace std;

class RGB {
public:
	int r, g, b;
};

class BMP
{
public:
	string filename;
	int width;
	int height;
	FILE* file;
	int bytesPerPixel;

	bool monochrome;
	vector<vector<RGB>> pixels;
	vector<vector<RGB>> g_pixels; // for gray scale
	vector<vector<bool>> m_pixels; // digital monochrome (b/w) pixels

	void close();
	void write(string filename, vector<vector<RGB>> pixels);
	int open();

	void genGrayScaleImage(); // generate grayscale from colored image
	void genDigitImage(int threshold=200, bool invert=false); // generate b&w only image(0/1) from grayscale
	void genBoolImage(); // generate bool arrays from b&w image
	void genPixels(bool invert=false); // generate pixels from m_pixels bool array
};
