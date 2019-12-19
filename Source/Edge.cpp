#include "Edge.h"



Edge::Edge(){
	isVisited = 0;
	flightNumber = 0;
	cost = 0;
	duration = 0;
	next = NULL;
}

Edge::Edge(const Edge& p){
	des = p.des;
	isVisited = p.isVisited;
	flightNumber = p.flightNumber;
	cost = p.cost;
	duration = p.duration;
	next = NULL;
}


Edge::~Edge()
{
}
