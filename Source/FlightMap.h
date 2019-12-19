#pragma once
#include <fstream>
#include <vector>
#include <queue>
#include <functional>
#include "Path.h"
class FlightMap{
public:
	vector<Vertex*> graph;
	int size;
	void getCities(string fileName);
	void getFlights(string fileName);
	void getRequest(string fileName);
	Path* Dijkstra(string source, string des, bool mode);
	vector<Path*> YenAlgorithm(string source, string des, int k, bool mode);

	FlightMap();
	FlightMap(const FlightMap& p);
	~FlightMap();
private:
	Path* Dijkstra_cost(Vertex* source, Vertex* des);
	Path* Dijkstra_duration(Vertex* source, Vertex* des);
	Vertex* node(string place);
	void removeEdge(string source, string des);
	void removeVertex(string place);
	void printPath(Path* path);
	bool isCityIn(string city);
};

class comparator {
public:
	comparator();
	bool operator() (const Vertex* lhs, const  Vertex* rhs);
};




