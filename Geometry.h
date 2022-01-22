#pragma once

#include <iostream>
#include <vector>

using namespace std;

class Line {
public:
	int y1, x1, y2, x2;
};

class Point {
public:
	int y, x;
};

class Limitation {
public:
	int low;
	int high;
};

class GFunction { // Graph Function(e.g. y=x^2)
public:
	string equation;
	string fulleqation; // full eqaution with limits in it
	vector<Limitation> limits;
};
