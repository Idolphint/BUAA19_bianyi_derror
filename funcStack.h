#pragma once
#include <vector>
#include "derror.h"
#include <string>
#include <iostream>
using namespace std;
static enum classtype {//定义类别
	CONST,
	VAR
};

static enum datatype {//数据类型
	dINT,
	dCHAR,
	dARRAY,
	dVOID
};
class funcRecord{
public:
	string name;
	datatype returnT;
	vector<datatype> paraList;
	vector<string> nameL;
	int rtN;
	funcRecord() {
		this->rtN = 0;
	}

	funcRecord(string name, datatype rtT) {
		this->name = name;
		this->returnT = rtT;
		this->rtN = 0;
	}

	funcRecord(string name, datatype rtT, vector<datatype> paraL) {
		this->name = name;
		this->returnT = rtT;
		this->paraList = paraL;
		this->rtN = 0;
	}
};

static vector<funcRecord> funcs;

funcRecord find_func(string name, vector<datatype> paral) { // 如果找不到该函数，报错
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
	throw myexception('c');//
}

void put_check(funcRecord& fun) {
	try {
		find_func(fun.name, fun.paraList);
	}
	catch (myexception ex) { //找不到才对呀~~
		funcs.push_back(fun);
		return;
	}
	throw myexception('b');//名字重定义
}