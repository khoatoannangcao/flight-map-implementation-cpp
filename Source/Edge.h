#pragma once
#include <iostream>
#include <string>

using namespace std;

class Vertex;
class Edge
{
public:
	Edge();
	Edge(const Edge& p);
	~Edge();

	bool isVisited;
	int flightNumber;
	float cost;
	float duration;
	string des;
	Vertex* next;
};

