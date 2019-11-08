#pragma once
#include <vector>
#include <iostream>
#include<string>

using namespace std;

class myexception : public exception {
public:
	char etype;
	int lineno;
	string msg;

	myexception() {
		this->etype = 'p';
		this->lineno = -1;
	}

	myexception(char etype) {
		this->etype = etype;
		this->lineno = -1;
	}

	myexception(char etype, string msg) {
		this->etype = etype;
		this->msg = msg;
	}

	myexception(char etype, int lineno, string msg) {
		this->etype = etype;
		this->msg = msg;
		this->lineno = lineno;
	}

	void pntMsg() {
		//std::cerr << to_string(lineno) <<"  "<< msg << etype<<"\n";
		std::cerr << to_string(lineno) << " "<< etype << "\n";
	}
};
