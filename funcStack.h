#pragma once
#include <vector>
#include "derror.h"
#include "runStack.h"
#include <string>
#include <iostream>
using namespace std;
class funcRecord{
public:
	string name;
	datatype returnT;
	vector<datatype> paraList;
	funcRecord() {
	}

	funcRecord(string name, datatype rtT) {
		this->name = name;
		this->returnT = rtT;
	}

	funcRecord(string name, datatype rtT, vector<datatype> paraL) {
		this->name = name;
		this->returnT = rtT;
		this->paraList = paraL;
	}
};

static vector<funcRecord> funcs;

funcRecord find_func(string name, vector<datatype> paral) { // ����Ҳ����ú���������
	for (funcRecord func : funcs) {
		if (func.name == name) {
			if (paral.size() != func.paraList.size()) {
				throw myexception('d');
			}
			vector<datatype> funp = func.paraList;
			for (int i = 0; i < paral.size(); i++) {
				if (funp.at(i) != paral.at(i)) {
					throw myexception('e');
				}
			}
			return func;
		}
	}
}

void put_check(funcRecord& fun) {
	try {
		find_func(fun.name, fun.paraList);
	}
	catch (myexception ex) { //�Ҳ����Ŷ�ѽ~~
		funcs.push_back(fun);
		return;
	}
	throw myexception('b');//�����ض���
}