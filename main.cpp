#include <iostream>
#include <cstdlib>
#include <fstream>
#include <chrono>

#include "Utils.h"

using namespace std;

float pointsize;
int offset;

int main(int argc, char** argv) {
	if (argc < 4) {
		cout << "Invalid Syntax! Usage: Prog FILEIN FILEOUT POINTSIZE OFFSET" << endl;
		return -1;
	}
	string filename = argv[1];
	string out = argv[2];
	pointsize = atof(argv[3]); // check Utils.h
	offset = atoi(argv[4]);
	BMP bmp;
	bmp.filename = filename;
	//if (bmp.open() != 0) {
	//	cout << "cant open " << filename << endl;
	//	return 0;
	//}
	int x = bmp.open();
	cout << endl;
	cout << "[grapher Beta]" << endl;
	cout << "--------------------------" << endl;
	cout << "Loaded Image Info: " << endl;
	cout << "Filename: " << bmp.filename << endl;
	cout << "Resolution: " << bmp.width << "x" << bmp.height << endl;
	cout << "Bit depth:" << bmp.bytesPerPixel * 8 << endl;
	cout << "--------------------------" << endl;
	cout << "Generating grayscale...";
	bmp.genGrayScaleImage();
	cout << " done." << endl;
	int threshold;
	string tmp;
	cout << "Enter digit threshold: " << endl;
	cout << "When you accept the result - press y" << endl;
	while (true) {
		cin >> tmp;
		if (tmp == "y") {
			break;
		}
		threshold = atoi(tmp.c_str());
		bmp.genDigitImage(threshold);
		bmp.genPixels();
		bmp.write("digit.bmp", bmp.pixels);
	}
	cout << endl;
	cout << "--------------------------" << endl;
	cout << "Generating..." << endl;
	auto start = chrono::high_resolution_clock::now();
	//bmp.genBoolImage();

	///*vector<Line> hLines = Utils::findHLines(bmp);
	//vector<Line> vLines = Utils::findVLines(bmp);
	//vector<Line> dLines = Utils::findDLines(bmp);
	//vector<Point> points = Utils::getSepPoints(bmp);

	//vector<GFunction> funcs = Utils::genGFunctions(hLines, vLines, dLines);
	//string out = Utils::exportDesmos(funcs, points);
	//cout << out;*/
	cout << "Searching for horizontal lines...";
	vector<vector<Line>> hLines = Utils::findSimilarHLines(bmp);
	cout << " done." << endl;
	auto hlines = std::chrono::high_resolution_clock::now();
	cout << "Searching for vertical lines...";
	vector<vector<Line>> vLines = Utils::findSimilarVLines(bmp);
	cout << " done." << endl;
	auto vlines = std::chrono::high_resolution_clock::now();
	cout << "Searching for diagonal lines...";
	vector<vector<Line>> dLines = Utils::findSimilarDLines(bmp);
	cout << " done." << endl;
	auto dlines = std::chrono::high_resolution_clock::now();
	cout << "Generating multilimits gfunctions...";
	vector<GFunction> funcs = Utils::genMultiLimitsGFunctions(hLines, vLines, dLines);
	cout << " done." << endl;
	auto funcsd = std::chrono::high_resolution_clock::now();
	cout << "Searching for points...";
	vector<Point> points = Utils::getSepPoints(bmp);
	cout << " done." << endl;
	auto pointsd = std::chrono::high_resolution_clock::now();
	string outs = Utils::exportDesmos(funcs, points);
	
	std::ofstream outf(out);
	outf << outs;
	outf.close();
	cout << "Desmos graph generated. Check " << out << endl;
	auto end = std::chrono::high_resolution_clock::now();
	cout << "--------------------------" << endl;
	cout << "Summary: " << endl;
	cout << "Horizontal lines searching time: " << (float)(std::chrono::duration_cast<std::chrono::microseconds>(hlines - start).count()) / 1000000 << "s" << endl;
	cout << "Vertical lines searching time: " << (float)(std::chrono::duration_cast<std::chrono::microseconds>(vlines - hlines).count()) / 1000000 << "s" << endl;
	cout << "Diagonal lines searching time: " << (float)(std::chrono::duration_cast<std::chrono::microseconds>(dlines - vlines).count()) / 1000000 << "s" <<endl;
	cout << "Multilimits funcstions generating time: " << (float)(std::chrono::duration_cast<std::chrono::microseconds>(funcsd - dlines).count()) / 1000000 << "s" << endl;
	cout << "Seperated points searching time: " << (float)(std::chrono::duration_cast<std::chrono::microseconds>(pointsd - funcsd).count()) / 1000000 << "s" << endl;

	int count = 0;
	for (int i = 0; i < outs.size(); i++) {
		if (outs[i] == '\n') {
			count++;
		}
	}
	cout << "Execution time: " << (float)(chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000000 << "s" << endl;
	cout << "Number of functions: " << count << endl;
	cout << "---------------------" << endl;
	return 0;
}
