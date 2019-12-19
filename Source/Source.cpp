#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "FlightMap.h"
using namespace std;

void readFile(string fileName) {
	string substring = fileName.substr(0, fileName.size() - 4);
	ifstream inputFile(fileName);
	ofstream cityFile(substring + "_city.txt");
	ofstream flightFile(substring + "_flight.txt");

	string tmp;

	vector<string> source_arr;
	string source, des, tmpNumber;
	float number[3];
	int size = 0;
	int s = 0, doubleQuote = 0, firstPos = 0;
	bool isInNumber = 0;
	getline(inputFile, tmp);
	while (getline(inputFile, tmp)) {
		for (int i = 0; i < (int)tmp.size(); i++) {
			if (tmp[i] == '\"') {
				if (doubleQuote == 0) {
					doubleQuote = 1;
					firstPos = i;
				}
				else {
					if (s == 0) {
						source = tmp.substr(firstPos + 1, i - firstPos - 1);
						doubleQuote = 0;
					}
					else {
						des = tmp.substr(firstPos + 1, i - firstPos - 1);
						doubleQuote = 0;
					}
					s++;
				}
			}
			else if ((tmp[i] >= '0' && tmp[i] <= '9') && (tmp[i - 1] == ' ' || tmp[i - 1] == '\t')) {
				isInNumber = 1;
				firstPos = i;
			}
			else if ((tmp[i] == ' ' || tmp[i] == '\t') && isInNumber == 1) {
				number[s - 2] = stof(tmp.substr(firstPos, i - firstPos));
				s++;
				isInNumber = 0;
			}
			
		}
		source_arr.push_back(source);
		source_arr.push_back(des);
		flightFile << source << ",\t" << des << "\t" << (int)number[0] << "\t" << number[1] << "\t" << number[2] << endl;
		size++;
		s = 0;

	}

	bool isDuplicate = 0;
	for (int i = 0; i < (int)source_arr.size(); i++) {
		for (int j = 0; j < i; j++) {
			if (source_arr[i] == source_arr[j]) {
				isDuplicate = 1;
				continue;
			}
		}
		if(!isDuplicate) cityFile << source_arr[i] << endl;
		isDuplicate = 0;
	}
	

}

int menu() {

	system("CLS");
	cout << "FLIGHT MAP LOOK UP!!" << endl;
	cout << "\t0: Get cities" << endl;
	cout << "\t1: Get flights" << endl;
	cout << "\t2: Look up" << endl;
	cout << "\t3: Exit" << endl;
	cout << "Your choice: ";

	char temp;

	do {
		cin >> temp;
	} while (!(temp >= '0' && temp <= '6'));
	return temp - '0';
}

int main() {
	FlightMap k;
	readFile("FlightInfo.txt");
	k.getCities("FlightInfo_city.txt");
	k.getFlights("FlightInfo_flight.txt");
	k.getRequest("request.txt");
	

	string input_str1, input_str2, output_str;
	char temp = 'a';



	while (1) {
		int choice = menu();
		system("CLS");
		if (choice == 0) {
			cout << "Enter your file name: ";
			getline(cin, input_str1);
			getline(cin, input_str1);

			if (input_str1[0] >= 61 && input_str1[0] <= 122) {
				input_str1[0] -= 32;
			}
			k.getCities(input_str1);
		}
		else if (choice == 1) {
			cout << "Enter your file name: ";
			getline(cin, input_str1);
			getline(cin, input_str1);
			k.getFlights(input_str1);
		
		}
		else if (choice == 2) {
			cout << "Enter your file request: ";
			getline(cin, input_str1);
			getline(cin, input_str1);
			k.getRequest(input_str1);
			getline(cin, input_str1);

		}
		else if (choice == 3) {
			return 1;
		}
	}


	return 1;
}