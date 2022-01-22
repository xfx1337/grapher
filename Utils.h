#pragma once

#include <iostream>
#include <algorithm>

#include "BMP.h"

using namespace std;

extern float pointsize; // size of seperated points for graph
extern int offset; // value of offset when moving graph from 0 0 to something

class Utils
{
public:
	static string ReplaceAll(string str, const string& from, const string& to);
	
	static vector<Line> findHLines(BMP bmp); // finds all vertical sections
	static vector<Line> findVLines(BMP bmp); // finds all horizontal sections
	static vector<Line> findDLines(BMP bmp); // finds all diagonal sections (e.g. y=x or y=x+1)
	

	// funcs like findXLines, but automaticaly sort them(but without time lose)
	static vector<vector<Line>> findSimilarHLines(BMP bmp); 
	static vector<vector<Line>> findSimilarVLines(BMP bmp); 
	static vector<vector<Line>> findSimilarDLines(BMP bmp); // because method of searching for diagonals is fucked - it will will found diagonals, next convert them to funcs and next find similar

	static vector<GFunction> checkForSameLimits(vector<GFunction>); // finds functions that have the same equation, but different limits

	static vector<Point> getSepPoints(BMP bmp); // finds individual pixels
	static void moveByOffset(vector <Line>& lines, vector<Point>& points, int offset = 0);

	static vector<GFunction> genGFunctions(vector<Line> hLines = {}, vector<Line> vLines = {}, vector<Line> dLines = {}); // gen all graph functions(like y=x^2) from already finded lines

	static vector<GFunction> genMultiLimitsGFunctions(vector<vector<Line>> hLines = {}, vector<vector<Line>> vLines = {}, vector<vector<Line>> = {}); // like checkForSameLimits but for all functions in one call

	static vector<GFunction> findSimilar(vector<GFunction> funcs); // finds similar functions and merges them

	static string exportDesmos(vector<GFunction> funcs = {}, vector<Point> points= {}); // get string that will easy to paste in desmos to draw graph
private:
	static vector<Line> findDLinesFromPoint(BMP bmp, vector<vector<bool>>& pixels, int i, int j, bool type); // type - check lower comments
	Utils() {}
};



/* types of diagonal lines
false:
   1
  1
 1
1
true:
1
 1
  1
   1
*/