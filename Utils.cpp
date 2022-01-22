#include "Utils.h"

string Utils::ReplaceAll(string str, const string& from, const string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

vector<Line> Utils::findHLines(BMP bmp) {
	vector<Line> lines;
	
	for (int i = 0; i < bmp.height; i++) {
		int sp[2] = { -1, -1 };
		int lp[2] = { -1, -1 };
		for (;;) {
			if (lp[1] + 1 == bmp.width) {
				lp[1] = 0;
				break;
			}
			if (bmp.m_pixels[i][lp[1] + 1] == true) {
				sp[0] = i; sp[1] = lp[1] + 1;
				lp[0] = i;
				for (int j = 1; j < bmp.width - (lp[1] + 1); j++) {
					if (bmp.m_pixels[i][sp[1] + j] == true) {
						lp[1] = sp[1] + j;
						if (lp[1] + 1 == bmp.width) {
							lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
							lp[1] -= 1;
							break;
						}
					}
					else {
						if (lp[1] != -1) {
							if (sp[1] < lp[1]) {
								lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
							}
						}
						lp[1] += 1;
						break;
					}
				}
			}
			lp[1] += 1;
		}
	}

	return lines;
}

vector<Line> Utils::findVLines(BMP bmp) {
	vector<Line> lines;

	for (int i = 0; i < bmp.width; i++) {
		int sp[2] = { -1, -1 };
		int lp[2] = { -1, -1 };
		for (;;) {
			if (lp[0] + 1 == bmp.height) {
				lp[0] = 0;
				break;
			}
			if (bmp.m_pixels[lp[0] + 1][i] == true) {
				sp[1] = i; sp[0] = lp[0] + 1;
				lp[1] = i;
				for (int j = 1; j < bmp.height - (lp[0] + 1); j++) {
					if (bmp.m_pixels[sp[0] + j][i] == true) {
						lp[0] = sp[0] + j;
						if (lp[0] + 1 == bmp.height) {
							lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
							lp[0] -= 1;
							break;
						}
					}
					else {
						if (lp[0] != -1) {
							if (sp[0] < lp[0]) {
								lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
							}
						}
						lp[0] += 1;
						break;
					}
				}
			}
			lp[0] += 1;
		}
	}

	return lines;
}


vector<Line> Utils::findDLines(BMP bmp) {
	vector<vector<bool>> imgCopy;
	imgCopy.resize(bmp.height);
	for (int i = 0; i < bmp.height; i++) {
		imgCopy.at(i).resize(bmp.width);
	}
	for (int i = 0; i < bmp.height; i++) {
		for (int j = 0; j < bmp.width; j++) {
			if (bmp.m_pixels[i][j] == true) {
				imgCopy[i][j] = true;
			}
			else {
				imgCopy[i][j] = false;
			}
		}
	}
	vector<Line> lines;

	int sp[2] = { -1, -1 };
	int lp[2] = { -1, -1 };

	// find lines like:
	//    1
	//   1
	//  1
	// 1

	for (int i = 0; i < bmp.height; i++) {
		for (int j = 0; j < bmp.width; j++) {
			if (imgCopy[i][j] == true) {
				vector<Line> vec;
				imgCopy[i][j] = false;
				sp[0] = i;
				sp[1] = j;
				lp[0] = -1;
				lp[1] = -1;
				for (int o = 1;;o++) {
					if (i + o < bmp.height && j + o < bmp.width) {
						if (imgCopy[i + o][j + o] == true) {
							imgCopy[i + o][j + o] = false;
							lp[0] = i + o;
							lp[1] = j + o;
						}
						else {
							if (lp[0] == -1) {
								break;
							}
							if (lp[0] != -1 && sp[0] != -1) {
								if (sp[0] != lp[0] && lp[0] != -1) {
									lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
									break;
								}
							}
						}
					}
					else {
						if (lp[0] == -1) {
							break;
						}
						if (lp[0] != -1 && sp[0] != -1) {
							if (sp[0] != lp[0] && lp[0] != -1) {
								lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
								break;
							}
						}
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < bmp.height; i++) {
		for (int j = 0; j < bmp.width; j++) {
			if (bmp.m_pixels[i][j] == true) {
				imgCopy[i][j] = true;
			}
			else {
				imgCopy[i][j] = false;
			}
		}
	}

	//find lines like
	// 1
	//  1
	//   1
	//    1
	for (int i = 0; i < bmp.height; i++) {
		for (int j = 0; j < bmp.width; j++) {
			if (imgCopy[i][j] == true) {
				imgCopy[i][j] = false;
				sp[0] = i;
				sp[1] = j;
				lp[0] = -1;
				lp[1] = -1;
				for (int o = 1;; o++) {
					if (i + o < bmp.height && j - o > -1) {
						if (imgCopy[i + o][j - o] == true) {
							imgCopy[i + o][j - o] = false;
							lp[0] = i + o;
							lp[1] = j - o;
						}
						else {
							if (lp[0] == -1) {
								break;
							}
							if (lp[0] != -1 && sp[0] != -1) {
								if (sp[0] != lp[0] && lp[0] != -1) {
									lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
									break;
								}
							}
						}
					}
					else {
						if (lp[0] == -1) {
							break;
						}
						if (lp[0] != -1 && sp[0] != -1) {
							if (sp[0] != lp[0] && lp[0] != -1) {
								lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
								break;
							}
						}
						break;
					}
				}
			}
		}
	}
	return lines;
}

vector<vector<Line>> Utils::findSimilarHLines(BMP bmp) {
	vector<vector<Line>> lines;
	for (int i = 0; i < bmp.height; i++) {
		vector<Line> vec;
		int sp[2] = { -1, -1 };
		int lp[2] = { -1, -1 };
		for (;;) {
			if (lp[1] + 1 == bmp.width) {
				lp[1] = 0;
				break;
			}
			if (bmp.m_pixels[i][lp[1] + 1] == true) {
				sp[0] = i; sp[1] = lp[1] + 1;
				lp[0] = i;
				for (int j = 1; j < bmp.width - (lp[1] + 1); j++) {
					if (bmp.m_pixels[i][sp[1] + j] == true) {
						lp[1] = sp[1] + j;
						if (lp[1] + 1 == bmp.width) {
							vec.push_back({ sp[0], sp[1], lp[0], lp[1] });
							lp[1] -= 1;
							break;
						}
					}
					else {
						if (lp[1] != -1) {
							if (sp[1] < lp[1]) {
								vec.push_back({ sp[0], sp[1], lp[0], lp[1] });
							}
						}
						lp[1] += 1;
						break;
					}
				}
			}
			lp[1] += 1;
		}
		if (vec.size() > 0) {
			lines.push_back(vec);
		}
	}
	return lines;
}

vector<vector<Line>> Utils::findSimilarVLines(BMP bmp) {
	vector<vector<Line>> lines;

	for (int i = 0; i < bmp.width; i++) {
		vector<Line> vec;
		int sp[2] = { -1, -1 };
		int lp[2] = { -1, -1 };
		for (;;) {
			if (lp[0] + 1 == bmp.height) {
				lp[0] = 0;
				break;
			}
			if (bmp.m_pixels[lp[0] + 1][i] == true) {
				sp[1] = i; sp[0] = lp[0] + 1;
				lp[1] = i;
				for (int j = 1; j < bmp.height - (lp[0] + 1); j++) {
					if (bmp.m_pixels[sp[0] + j][i] == true) {
						lp[0] = sp[0] + j;
						if (lp[0] + 1 == bmp.height) {
							vec.push_back({ sp[0], sp[1], lp[0], lp[1] });
							lp[0] -= 1;
							break;
						}
					}
					else {
						if (lp[0] != -1) {
							if (sp[0] < lp[0]) {
								vec.push_back({ sp[0], sp[1], lp[0], lp[1] });
							}
						}
						lp[0] += 1;
						break;
					}
				}
			}
			lp[0] += 1;
		}
		if (vec.size() > 0) {
			lines.push_back(vec);
		}
	}

	return lines;
}

vector<vector<Line>> Utils::findSimilarDLines(BMP bmp) {
	vector<vector<Line>> outlines;
	vector<vector<bool>> pixels;
	vector<vector<bool>> pixels2;
	pixels.resize(bmp.height);
	pixels2.resize(bmp.height);
	for (int i = 0; i < bmp.height; i++) {
		pixels.at(i).resize(bmp.width);
		pixels2.at(i).resize(bmp.width);
	}
	for (int i = 0; i < bmp.height; i++) {
		for (int j = 0; j < bmp.width; j++) {
			if (bmp.m_pixels[i][j] == true) {
				pixels[i][j] = true;
				pixels2[i][j] = true;
			}
			else {
				pixels[i][j] = false;
				pixels2[i][j] = false;
			}
		}
	}

	for (int i = 0; i < bmp.width; i++) {
		vector<Line> vec;
		vec = findDLinesFromPoint(bmp, pixels, 0, i, false);
		if (vec.size() > 0) {
			outlines.push_back(vec);
		}
	}
	for (int i = 0; i < bmp.height; i++) {
		vector<Line> vec;
		vec = findDLinesFromPoint(bmp, pixels, i, 0, false);
		if (vec.size() > 0) {
			outlines.push_back(vec);
		}
	}

	for (int i = 0; i < bmp.width; i++) {
		vector<Line> vec;
		vec = findDLinesFromPoint(bmp, pixels2, 0, i, true);
		if (vec.size() > 0) {
			outlines.push_back(vec);
		}
	}
	for (int i = 0; i < bmp.height; i++) {
		vector<Line> vec;
		vec = findDLinesFromPoint(bmp, pixels2, i, bmp.width-1, false);
		if (vec.size() > 0) {
			outlines.push_back(vec);
		}
	}


	return outlines;
}

vector<GFunction> Utils::checkForSameLimits(vector<GFunction> funcs) {
	vector<GFunction> s;
	return s;
}

vector<GFunction> Utils::genGFunctions(vector<Line> hLines, vector<Line> vLines, vector<Line> dLines) {
	vector<GFunction> funcs;
	string func;
	int limm = -1;
	int limx = -1;
	for (int i = 0; i < hLines.size(); i++) {
		func = "y=" + to_string(hLines[i].y1);
		limm = hLines[i].x1;
		limx = hLines[i].x2;
		if (limx != -1 && limm != -1) {
			funcs.push_back({ func, func + " {" + to_string(limm) + " <= x <= " + to_string(limx) + "}", {{limm, limx}}});
		}
		else {
			funcs.push_back({ func, func, {{-1, -1}}});
		}
	}

	for (int i = 0; i < vLines.size(); i++) {
		func = "x=" + to_string(vLines[i].x1);
		limm = vLines[i].y1;
		limx = vLines[i].y2;
		if (limx != -1 && limm != -1) {
			funcs.push_back({ func, func + " {" + to_string(limm) + " <= y <= " + to_string(limx) + "}", {{limm, limx}}});
		}
		else {
			funcs.push_back({ func, func, {{-1, -1}}});
		}

	}

	for (int i = 0; i < dLines.size(); i++) {
		if (dLines[i].x2 > dLines[i].x1) {
			func = "y=x+" + to_string(dLines[i].y1 - dLines[i].x1);
		}
		else {
			func = "y=-x+" + to_string(dLines[i].y1 + dLines[i].x1);
		}

		limm = dLines[i].x1;
		limx = dLines[i].x2;
		if (limx != -1 && limm != -1) {
			if (limx > limm) {
				funcs.push_back({ func, func + " {" + to_string(limm) + " <= x <= " + to_string(limx) + "}", {{limm, limx}}});
			}
			else {
				funcs.push_back({ func, func + " {" + to_string(limx) + " <= x <= " + to_string(limm) + "}", {{limm, limx}}});
			}
		}
		else {
			funcs.push_back({ func, func, {{-1, -1}}});
		}
	}

	return funcs;
}

vector<GFunction> Utils::genMultiLimitsGFunctions(vector<vector<Line>> hLines, vector<vector<Line>> vLines, vector<vector<Line>> dLines) {
	vector<GFunction> funcs;
	string func;
	string fulleq;
	vector<Limitation> limits;
	for (int i = 0; i < hLines.size(); i++) {
		func = "y=" + to_string(hLines[i][0].y1);
		fulleq = func + " {";
		limits.clear();
		for (int j = 0; j < hLines[i].size(); j++) {
			limits.push_back({ hLines[i][j].x1, hLines[i][j].x2 });
			if (j == 0) {
				fulleq = fulleq + to_string(hLines[i][j].x1) + " <= x <= " + to_string(hLines[i][j].x2);
			}
			else {
				fulleq = fulleq + ", " + to_string(hLines[i][j].x1) + " <= x <= " + to_string(hLines[i][j].x2);
			}
		}
		funcs.push_back({ func, fulleq + "}", limits });
	}

	for (int i = 0; i < vLines.size(); i++) {
		func = "x=" + to_string(vLines[i][0].x1);
		fulleq = func + " {";
		limits.clear();
		for (int j = 0; j < vLines[i].size(); j++) {
			limits.push_back({ vLines[i][j].y1, vLines[i][j].y2 });
			if (j == 0) {
				fulleq = fulleq + to_string(vLines[i][j].y1) + " <= y <= " + to_string(vLines[i][j].y2);
			}
			else {
				fulleq = fulleq + ", " + to_string(vLines[i][j].y1) + " <= y <= " + to_string(vLines[i][j].y2);
			}
		}
		funcs.push_back({ func, fulleq + "}", limits });
	}

	for (int i = 0; i < dLines.size(); i++) {
		if (dLines[i][0].x2 > dLines[i][0].x1) {
			func = "y=x+" + to_string(dLines[i][0].y1 - dLines[i][0].x1);
		}
		else {
			func = "y=-x+" + to_string(dLines[i][0].y1 + dLines[i][0].x1);
		}
		fulleq = func + " {";
		limits.clear();
		for (int j = 0; j < dLines[i].size(); j++) {
			if (dLines[i][j].x2 > dLines[i][j].x1) {
				limits.push_back({ dLines[i][j].x1, dLines[i][j].x2 });
			}
			else {
				limits.push_back({ dLines[i][j].x2, dLines[i][j].x1 });
			}
			if (j == 0) {
				fulleq = fulleq + to_string(limits[limits.size() - 1].low) + " <= x <= " + to_string(limits[limits.size() - 1].high);
			}
			else {
				fulleq = fulleq + ", " + to_string(limits[limits.size() - 1].low) + " <= x <= " + to_string(limits[limits.size() - 1].high);
			}
		}
		funcs.push_back({ func, fulleq + "}", limits });
	}
	return funcs;
}

vector<Point> Utils::getSepPoints(BMP bmp) {
	vector<Point> points;
	for (int i = 0; i < bmp.height; i++) {
		for (int j = 0; j < bmp.width; j++) {
			bool st = true;
			if (bmp.m_pixels[i][j] == true) {
				if (i > 0) {
					if (bmp.m_pixels[i - 1][j] == true) {
						st = false;
					}
					if (j > 0) {
						if (bmp.m_pixels[i - 1][j - 1] == true) {
							st = false;
						}
					}
					if (j + 1 < bmp.width) {
						if (bmp.m_pixels[i - 1][j + 1] == true) {
							st = false;
						}
					}
				}
				if (j > 0) {
					if (bmp.m_pixels[i][j - 1] == true) {
						st = false;
					}
				}
				if (j + 1 < bmp.width) {
					if (bmp.m_pixels[i][j + 1] == true) {
						st = false;
					}
				}
				if (i < bmp.height - 1) {
					if (bmp.m_pixels[i + 1][j] == true) {
						st = false;
					}
					if (j > 0) {
						if (bmp.m_pixels[i + 1][j - 1] == true) {
							st = false;
						}
					}
					if (j + 1 < bmp.width) {
						if (bmp.m_pixels[i + 1][j + 1] == true) {
							st = false;
						}
					}
				}
				if (st == true) {
					points.push_back({ i, j });
				}
			}
		}
	}

	return points;
}

void Utils::moveByOffset(vector <Line> &lines, vector<Point> &points, int offset) {
	for (int i = 0; i < lines.size(); i++) {
		lines.at(i).x1 += offset;
		lines.at(i).x2 += offset;
		lines.at(i).y1 += offset;
		lines.at(i).y2 += offset;
	}
	for (int i = 0; i < points.size(); i++) {
		points.at(i).x += offset;
		points.at(i).y += offset;
	}
}

vector<GFunction> Utils::findSimilar(vector<GFunction> funcs) {
	vector<GFunction> outfuncs;
	string fulleq;
	vector <Limitation> limits;
	vector<int> toerase;
	for (;;) {
		if (funcs.size() > 1) {
			limits.clear();
			toerase.clear();
			for (int j = 1; j < funcs.size(); j++) {
				if (funcs[0].equation == funcs[j].equation) {
					for (int o = 0; o < funcs[j].limits.size(); o++) {
						limits.push_back(funcs[j].limits[o]);
					}
					toerase.push_back(j);
				}
			}
			if (limits.size() > 0) {
				for (int j = 0; j < funcs[0].limits.size(); j++) {
					limits.push_back(funcs[0].limits[j]);
				}
				string sym = "x";
				if (funcs[0].equation[0] != 'y') {
					sym = 'y';
				}
				fulleq = funcs[0].equation + " {";
				for (int j = 0; j < limits.size(); j++) {
					if (j != 0) {
						if (limits[j].high > limits[j].low) {
							fulleq = fulleq + ", " + to_string(limits[j].low) + " <= " + sym + " <= " + to_string(limits[j].high);
						}
						else {
							fulleq = fulleq + ", " + to_string(limits[j].high) + " <= " + sym + " <= " + to_string(limits[j].low);
							int tmp = limits[j].high;
							limits[j].high = limits[j].low;
							limits[j].low = tmp;
						}
					}
					else {
						if (limits[j].high > limits[j].low) {
							fulleq = fulleq + to_string(limits[j].low) + " <= " + sym + " <= " + to_string(limits[j].high);
						}
						else {
							fulleq = fulleq + to_string(limits[j].high) + " <= " + sym + " <= " + to_string(limits[j].low);
							int tmp = limits[j].high;
							limits[j].high = limits[j].low;
							limits[j].low = tmp;
						}

					}
				}
				fulleq += " }";
				outfuncs.push_back({ funcs[0].equation, fulleq, limits });
			}
			else {
				outfuncs.push_back(funcs[0]);
				funcs.erase(funcs.begin());
			}
			if (toerase.size() > 0) {
				sort(toerase.begin(), toerase.end(), greater<int>());
				for (int j = 0; j < toerase.size(); j++) {
					funcs.erase(funcs.begin() + toerase[j]);
				}
				if (toerase.size() > 0) {
					funcs.erase(funcs.begin());
				}
			}
		}
		else if (funcs.size() == 1) {
			outfuncs.push_back(funcs[0]);
			break;
		}
		else {
			break;
		}
	}
	return outfuncs;
}

vector<Line> Utils::findDLinesFromPoint(BMP bmp, vector<vector<bool>>& pixels, int i, int j, bool type) {
	vector<Line> lines;
	int sp[2] = { i, j};
	int lp[2] = { -1, -1 };
	if (type == false) {
		for (int o = 1; o > -1; o++) {
			if (i + o < bmp.height && j + o < bmp.width) {
				if (pixels[i + o][j + o] == true) {
					pixels[i + o][j + o] = false;
					lp[0] = i + o;
					lp[1] = j + o;
					if (sp[0] == -1) {
						sp[0] = i + o;
						sp[1] = j + o;
					}
				}
				else {
					if (lp[0] > sp[0] && sp[0] != -1) {
						lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
					}
					lp[0] = -1;
					lp[1] = -1;
					sp[0] = -1;
					sp[1] = -1;
				}
			}
			else {
				if (lp[0] != -1 && lp[0] > sp[0] && sp[0] != -1) {
					lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
				}
				break;
			}
		}
	}
	else {
		for (int o = 1; o > -1; o++) {
			if (i + o < bmp.height && j - o > -1) {
				if (pixels[i + o][j - o] == true) {
					pixels[i + o][j - o] = false;
					lp[0] = i + o;
					lp[1] = j - o;
					if (sp[0] == -1) {
						sp[0] = i + o;
						sp[1] = j - o;
					}
				}
				else {
					if (lp[0] > sp[0] && sp[0] != -1) {
						lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
					}
					lp[0] = -1;
					lp[1] = -1;
					sp[0] = -1;
					sp[1] = -1;
				}
			}
			else {
				if (lp[0] != -1 && lp[0] > sp[0] && sp[0] != -1) {
					lines.push_back({ sp[0], sp[1], lp[0], lp[1] });
				}
				break;
			}
		}
	}
	return lines;
}

string Utils::exportDesmos(vector<GFunction> funcs, vector<Point> points) {
	string out;
	for (int i = 0; i < funcs.size(); i++) {
		string tmp = funcs[i].fulleqation;
		tmp = ReplaceAll(tmp, "{", "\\left\\{");
		tmp = ReplaceAll(tmp, "}", "\\right\\}");
		out = out + tmp + "\n";
	}

	for (int i = 0; i < points.size(); i++) {
		out += to_string(points[i].y - pointsize/2) + "<= y <= " + to_string(points[i].y + pointsize/2) + " \\left\\{" + to_string(points[i].x - pointsize / 2) + "<= x <= " + to_string(points[i].x + pointsize / 2) + "\\right\\}\n";
	}

	return out;
}
