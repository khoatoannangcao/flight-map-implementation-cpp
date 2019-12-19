#include "Path.h"



Path::Path(vector<Node> data){
	_path = data;
}

Path::Path(const Path & p){
	_path = p._path;
}

Path * Path::subPath(string source, string des){
	Path* result = new Path();
	bool isIn = 0;
	if (source == des) {
		return result;
	}
	for (int i = 0; i < size(); i++) {
		if (_path[i].first == source) {
			result->_path.push_back(Node(_path[i].first, _path[i].second));
			isIn = 1;
		}
		else if (_path[i].first == des) {
			result->_path.push_back(_path[i]);
			result->_path.erase(result->_path.begin() + result->_path.size() - 1);
			return result;
		}
		else if (isIn == 1) {
			result->_path.push_back(_path[i]);
		}
	}
	result->_path.erase(result->_path.begin() + result->_path.size() - 1);
	return result;
}

Path * Path::operator+(const Path & p){
	Path* result = new Path;
	result->_path = _path;
	result->_path.insert(result->_path.end(), p._path.begin(), p._path.end());
	return result;
}

bool Path::operator==(Path p){
	if (size() != p.size()) return 0;
	for (int i = 0; i < p.size(); i++) if (_path[i].first != p._path[i].first || _path[i].second != p._path[i].second) return 0;
	return 1;
}

Path::Path(){
}

float Path::amount(){
	int result = 0;
	for (int i = 0; i < _path.size(); i++) {
		result += _path[i].second;
	}
	return result;
}



int Path::size()
{
	return _path.size();
}


Path::~Path()
{
}

