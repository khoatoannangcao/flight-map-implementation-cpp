#pragma once
#ifndef PATH
#define PATH
#include "Edge.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Path;
class Vertex
{
public:
	string place;
	bool isVisited;
	float amount;
	vector<Edge*> next;
	Vertex();
	Vertex(string);
	Vertex(const Vertex& p);
	Vertex(Vertex* p);
	~Vertex();

	bool operator < (Vertex* p);
	bool operator > (Vertex* p);
	
};

#endif

#include "Path.h"