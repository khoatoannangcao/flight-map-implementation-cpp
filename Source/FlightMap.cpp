#include "FlightMap.h"



void FlightMap::getCities(string fileName){
	ifstream file(fileName);
	string k;
	Vertex* node;
	while (getline(file, k)) {
		size++;
		node = new Vertex(k);
		graph.push_back(node);
	}
}

void FlightMap::getFlights(string fileName){
	ifstream file(fileName);

	string line;
	Edge* edge;
	float number[3];
	int index, temp_number;
	//It holds the value of a flight (an edge in graph) in each line.
	//infomation[0]: duration
	//infomation[1]: cost
	//infomation[2]: flight number
	//index: current index of infomation array
	//temp_number: temporative number when read a line.
	string source, des;
	
	int line_size, i, j, firstPos = 0;
	bool isInNumber = 0;

	while (getline(file, line)) {
		line_size = line.size();
		i = line_size - 1;
		index = 0;
		temp_number = 0;
		while (i > 0) {
			if (index < 3) {
				if ((line[i] >= '0' && line[i] <= '9') && ((line[i + 1] == '\t') || i == line.size() - 1)) {
					firstPos = i;
				}
				if ((line[i] >= '0' && line[i] <= '9') && (line[i - 1] == '\t')) {
					number[index] = stof(line.substr(i, firstPos - i + 1));
					index++;
				}
			}
			else {
				if (line[i] != '\t')
					break;
			}
			i--;
		}
		for (j = 0; j < i; j++) {
			if (line[j] == ',' && line[j+1] == '\t') break;
		}
		source = line.substr(0, j);
		for (j; j < i; j++) {
			if (line[j] != '\t') break;
		}
		des = line.substr(j + 2, i - j - 1);
		edge = new Edge;
		edge->cost = number[1];
		edge->flightNumber = (int)number[2];
		edge->duration = number[0];
		edge->des = des;
		for (int k = 0; k < size; k++) {
			if (graph[k]->place == source) {
				graph[k]->next.push_back(edge);
			}
			else if (graph[k]->place == des) {
				edge->next = graph[k];
			}
		}
	}
	
}

void FlightMap::getRequest(string fileName){
	ifstream file(fileName);
	string line;
	string source, des;

	int K;
	vector<Path*> result;
	file >> K ;
	int mode;
	int line_size, i, j = 0, firstPos = 0;
	getline(file, line);
	while (getline(file, line)) {
		line_size = line.size();
		firstPos = 0;
		j = 0;
		for (i = 0; i < line_size; i++) {
			if (line[i] == '\t') {
				if (j == 0) {
					source = line.substr(firstPos, i - firstPos - 1);
					firstPos = i;
					j++;
				}
				else if (j == 1) {
					des = line.substr(firstPos + 1, i - firstPos - 1);
					firstPos = i;
					j++;
				}
			}
		}
		mode = stoi(line.substr(firstPos + 1, 1));
		
		result.clear();
		cout << "Request is to fly from " << source << " to " << des << "." << endl;
		if (!isCityIn(source)){
			cout << "Sorry. HPAir does not serve " << source << "." << endl << endl;
			continue;
		}
		if (!isCityIn(des)) {
			cout << "Sorry. HPAir does not serve " << des << "." << endl << endl;
			continue;
		}
		result = YenAlgorithm(source, des, K, mode - 1);
		if (result.size() == 0) {
			cout << "Sorry. HPAir does not fly from " << source << " to " << des << endl << endl;
			continue;
		}
		for (int i = 0; i < result.size(); i++) {
			cout << "Route " << i + 1 << " : ";
			printPath(result[i]);
			delete result[i];
			cout << endl;
		}
	}
}





FlightMap::FlightMap(){
	size = 0;
}

FlightMap::FlightMap(const FlightMap & p){
	size = p.size;
	Edge *edge;
	for (int i = 0; i < size; i++) {
		graph.push_back(new Vertex((p.graph[i])));		
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < (int)p.graph[i]->next.size(); j++) {
			edge = new Edge(*(p.graph[i]->next[j]));
			for (int k = 0; k < size; k++) {
				if (graph[k]->place == edge->des) {
					edge->next = p.graph[k];
					break;
				}
			}
			graph[i]->next.push_back(edge);
			edge = NULL;
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < (int)graph[i]->next.size(); j++) {
			for (int k = 0; k < size; k++) {
				if (graph[k]->place == graph[i]->next[j]->des) {
					graph[i]->next[j]->next = graph[k];
					break;
				}
			}
		}
	}
}


FlightMap::~FlightMap(){
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < (int)graph[i]->next.size(); j++) delete graph[i]->next[j];
		delete graph[i];
	}
}

Path* FlightMap::Dijkstra(string source, string des, bool mode) {
	Vertex* sour_Node = node(source);
	Vertex* des_Node = node(des);
	
	if (mode == 0) return Dijkstra_cost(sour_Node, des_Node);
	return Dijkstra_duration(sour_Node, des_Node);
}

vector<Path*> FlightMap::YenAlgorithm(string source, string des, int K, bool mode){
	Path* totalPath;
	int h;
	bool is_Duplicate;
	if (K <= 0) return vector<Path*>();
	if (source == des) return vector<Path*>();
	//Wrong parameter case.

	vector<Path*> B;
	//Potential path implement

	vector<Path*> result;
	//K-shortest paths.

	Path* first_p = Dijkstra(source, des, mode);

	if (first_p == NULL) return result;
	//Get the shortest path.

	result.push_back(first_p);
	//Add 
	for (int k = 1; k < K; k++) {
		for (int i = 0; i < result[k - 1]->size() - 1; i++) {

			FlightMap currMap(*this);
			//Dupliating to not change the original map.
			string spurNode = result[k - 1]->_path[i].first;

			Path* rootPath = result[k - 1]->subPath(source, spurNode);

			for (int j = 0; j < (int)result.size(); j++) {
				Path* tmp = result[j]->subPath(source, spurNode);
				if (*rootPath == *tmp && i + 1 < result[j]->_path.size()) {
					// Remove the links that are part of the previous shortest paths which share the same root path.
					currMap.removeEdge(spurNode, result[j]->_path[i + 1].first);
				}
				delete tmp;
			}
			
			for (int j = 0; j < rootPath->size() - 1; j++) {
				currMap.removeVertex(rootPath->_path[j].first);
			}

			Path* spurPath = currMap.Dijkstra(spurNode,des , mode);

			is_Duplicate = 0;
			for (h = 0; h < B.size(); h++) {
				if (B[h] == spurPath) is_Duplicate = 1;
			}

			if (spurPath == NULL || is_Duplicate == 1) {
				delete spurPath;
				delete rootPath;
				continue;
			}

			for (h = 0; h < graph.size(); h++) {
				if (rootPath->_path.size() > 0 && graph[h]->place == rootPath->_path[rootPath->_path.size() - 1].first) {
					for(int r = 0; r < graph[h]->next.size(); r++){
						if (graph[h]->next[r]->des == spurNode) {
							spurPath->_path[0].second = mode == 0 ?graph[h]->next[r]->cost : graph[h]->next[r]->duration;
						}
					}
				}
			}

			
			totalPath = *rootPath + *spurPath;

			B.push_back(totalPath);

			delete spurPath;
			delete rootPath;
		}

		if (B.empty()) break;
		Path* tmp = B[0]; int tmp_pos = 0;
		
		for (h = 1; h < B.size(); h++) {
			if (B[h]->amount() < tmp->amount()) {
				tmp = B[h];
				tmp_pos = h;
			}
		}
		B.erase(B.begin() + tmp_pos);
		result.push_back(tmp);
	}
	return result;
}

Vertex * FlightMap::node(string place){
	for (int i = 0; i < size; i++) {
		if (graph[i]->place == place) return graph[i];
	}
	return NULL;
}

void FlightMap::removeEdge(string source, string des){
	for (int i = 0; i < size; i++) {
		if (graph[i]->place == source) {
			for (int j = 0; j < (int)graph[i]->next.size(); j++) {
				if (graph[i]->next[j]->des == des) {
					graph[i]->next.erase(graph[i]->next.begin() + j);
				}
			}
		}
	}
}

void FlightMap::removeVertex(string place){
	for (int i = 0; i < (int)graph.size(); i++) {
		if (graph[i]->place == place) {
			graph.erase(graph.begin() + i);
		}
	}
	size--;
	return;
}

void FlightMap::printPath(Path * path){
	float totalCost = 0, totalDuration = 0;
	bool isPrint = 0;
	for (int i = 0; i < path->_path.size() - 1; i++) {
		cout << path->_path[i].first << " - ";
	}
	cout << path->_path[path->_path.size() - 1].first << endl;
	for (int i = 0; i < path->_path.size() - 1; i++) {
		cout << "Flight #";
		isPrint = 0;
		for (int j = 0; j < size && isPrint == 0; j++) {
			if (graph[j]->place == path->_path[i].first) {
				for (int k = 0; k < graph[j]->next.size(); k++) {
					if (graph[j]->next[k]->des == path->_path[i + 1].first) {
						cout << graph[j]->next[k]->flightNumber;
						cout << " from " << path->_path[i].first << " to " << path->_path[i + 1].first << " Cost: $";
						cout << graph[j]->next[k]->cost << " Duration: " << graph[j]->next[k]->duration << " hours" << endl;
						totalCost += graph[j]->next[k]->cost;
						totalDuration += graph[j]->next[k]->duration;
						isPrint = 1;
						break;
					}
				}
			}
		}
	}
	cout << "Total cost: \t$" << totalCost << endl << "Total duration: " << totalDuration << " minutes" << endl;
}

bool FlightMap::isCityIn(string city){
	for (int i = 0; i < size; i++) {
		if (graph[i]->place == city) 
			return 1;
	}
	return 0;
}

Path* FlightMap::Dijkstra_cost(Vertex * source, Vertex * des){
	Path* result_path;
	result_path = new Path[size];

	int number_start, number_end, number_destination;

	std::priority_queue<Vertex*, vector<Vertex*>, comparator> min_queue;
	FlightMap k(*this);
	for (int i = 0; i < k.size; i++) {
		if (k.graph[i]->place == source->place) {
			//If graph[i] is source.
			k.graph[i]->amount = 0;
			result_path[i]._path.push_back(Node(k.graph[i]->place, 0));
		}	
		else {
			if (k.graph[i]->place == des->place)
				number_destination = i;
			k.graph[i]->amount = FLT_MAX;
			
		}
		min_queue.push((k.graph[i]));
	}
	while (!min_queue.empty()) {
		Vertex* minVer = min_queue.top();
		if (k.graph[number_destination]->isVisited && minVer->amount == FLT_MAX) {
			break;
		}
		min_queue.pop();
		minVer->isVisited = true;

		for (int i = 0; i < (int)minVer->next.size(); i++) {
			if (minVer->amount + minVer->next[i]->cost < minVer->next[i]->next->amount) {
				minVer->next[i]->next->amount = minVer->amount + minVer->next[i]->cost;
				for (int j = 0; j < k.size; j++) {
					if (k.graph[j]->place == minVer->next[i]->next->place) number_end = j;
					else if (k.graph[j]->place == minVer->place) number_start = j;
				}
				result_path[number_end]._path = result_path[number_start]._path;
				result_path[number_end]._path.push_back(Node(minVer->next[i]->des, minVer->next[i]->cost));
			}
		}
		while(!min_queue.empty()) {
			min_queue.pop();
		}
		for (int i = 0; i < size; i++) {
			if(!k.graph[i]->isVisited)
			min_queue.push(k.graph[i]);
		}
		if (!min_queue.empty() && min_queue.top()->amount == FLT_MAX && k.graph[number_destination]->isVisited == 0) {
			return NULL;
		}
	}
	Path* result = new Path(result_path[number_destination]);
	delete[] result_path;
	return result;
}

Path* FlightMap::Dijkstra_duration(Vertex * source, Vertex * des){
	Path* result_path;
	result_path = new Path[size];
	int number_visited = 0;

	int number_start, number_end, number_destination;

	std::priority_queue<Vertex*, vector<Vertex*>, comparator> min_queue;
	FlightMap k(*this);
	for (int i = 0; i < k.size; i++) {
		if (k.graph[i]->place == source->place) {
			//If graph[i] is source.
			k.graph[i]->amount = 0;
			result_path[i]._path.push_back(Node(k.graph[i]->place, 0));
		}
		else {
			if (k.graph[i]->place == des->place)
				number_destination = i;
			k.graph[i]->amount = FLT_MAX;

		}
		min_queue.push((k.graph[i]));
	}
	while (!min_queue.empty()) {
		Vertex* minVer = min_queue.top();
		if (k.graph[number_destination]->isVisited && minVer->amount == FLT_MAX) {
			break;
		}
		min_queue.pop();
		minVer->isVisited = true;

		for (int i = 0; i < (int)minVer->next.size(); i++) {
			if (minVer->amount + minVer->next[i]->duration < minVer->next[i]->next->amount) {
				minVer->next[i]->next->amount = minVer->amount + minVer->next[i]->duration;
				for (int j = 0; j < k.size; j++) {
					if (k.graph[j]->place == minVer->next[i]->next->place) number_end = j;
					else if (k.graph[j]->place == minVer->place) number_start = j;
				}
				result_path[number_end]._path = result_path[number_start]._path;
				result_path[number_end]._path.push_back(Node(minVer->next[i]->des, minVer->next[i]->duration));
			}
		}
		while (!min_queue.empty()) {
			min_queue.pop();
		}
		for (int i = 0; i < size; i++) {
			if (!k.graph[i]->isVisited)
				min_queue.push(k.graph[i]);
		}
		if (!min_queue.empty() && min_queue.top()->amount == FLT_MAX && k.graph[number_destination]->isVisited == 0) {
			return NULL;
		}
	}
	Path* result = new Path(result_path[number_destination]);
	return result;
}


comparator::comparator(){
}

bool comparator::operator()(const Vertex * lhs, const Vertex * rhs) {
	return lhs->amount > rhs->amount;
}


