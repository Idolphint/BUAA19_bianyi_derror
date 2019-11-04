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

	}

	myexception(char etype) {
		this->etype = etype;
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
		std::cerr << to_string(lineno) << " " << etype;
	}
};
