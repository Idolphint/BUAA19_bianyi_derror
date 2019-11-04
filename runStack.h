#pragma once
#include <vector>
#include <iostream>
#include "derror.h"
#include <stack>
using namespace std;

using namespace std;
static enum classtype {//定义类别
	CONST,
	VAR
};

static enum datatype {//数据类型
	INT,
	CHAR,
	ARRAY,
	VOID
};

class arrayTem {
public:
	int dimn;//本文法只支持一维数组
	int len;
	datatype tp;
	int addr;
	arrayTem(int len, int dim, datatype tp, int addr) : len(len), dimn(dim), tp(tp), addr(addr) {

	}
};

class record {
public:
	string name;
	classtype typeC;
	datatype typeD;
	int addr;// 存储地址，也可以指向模板地址

	record() {
		addr = -1;
	}
	record(string name, classtype c, datatype d, int addr) {
		this->name = name;
		this->typeC = c;
		this->typeD = d;
		this->addr = addr;
	}

};

class runStack {
public:
	vector<runStack*> display;//display区存储它上级的指针
	int ptr;
	//省略基地址指针,我选择用*/&替代
	//返回地址省略
	runStack* preabp;//上一个活动基
	vector<record> dataStack;
	funcRecord father;

	runStack() {
		this->preabp = NULL;
		ptr = -1;//模拟栈指针
	}

	runStack(runStack* pre) {
		this->preabp = pre;
		ptr = -1;
		if (pre == NULL)
			return;
		this->display.push_back(pre);
		runStack tmp = *pre;
		while (tmp.preabp != NULL) {
			this->display.push_back(tmp.preabp);
			tmp = *(tmp.preabp);
		}
	}

	record findRecord(string name, bool write=false) {
		int ptrtmp = ptr;
		while (ptrtmp >= 0) { 
			record tmp = dataStack.at(ptrtmp);
			if (tmp.name == name) {
				return tmp;
			}
		}
		if (!write) {
			for (runStack *bufS : display) {
				int ptrtmp2 = bufS->ptr;
				while (ptrtmp2 >= 0) {
					record tmp = bufS->dataStack.at(ptrtmp2);
					if (tmp.name == name) {
						return tmp;
					}
				}
			}
		}
		throw myexception('c');
	}

	void writeStack(record rec) {
		try {
			record before = findRecord(rec.name, true);
		}
		catch (myexception ex) { // 正确的情况okk
			dataStack.push_back(rec);
			return;
		}
		throw myexception('b');//没有异常说明，名字重定义
	}

};

static stack<runStack> myrunStack;
