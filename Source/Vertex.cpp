#include "Vertex.h"



Vertex::Vertex(){
	amount = 0;
	isVisited = 0;
}

Vertex::Vertex(string p){
	place = p;
	amount = 0;
	isVisited = 0;
}

Vertex::Vertex(const Vertex & p){
	place = p.place;
	amount = 0;
	isVisited = p.isVisited;
}

Vertex::Vertex(Vertex * p){
	place = p->place;
	amount = 0;
}

Vertex::~Vertex()
{
}

bool Vertex::operator<(Vertex * p){
	return amount < p->amount;
}
bool Vertex::operator>(Vertex * p) {
	return amount > p->amount;
}
