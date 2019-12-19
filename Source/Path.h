#pragma once

#include "Vertex.h"
#include <utility>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
typedef pair<string, float> Node;
class Path
{
public:
	vector<Node> _path;
	Path(vector<Node>);
	Path(const Path& p);
	Path* subPath(string source, string des);
	Path* operator + (const Path& p);
	bool operator == (Path p);
	Path();

	float amount();
	int size();
	~Path();
};


