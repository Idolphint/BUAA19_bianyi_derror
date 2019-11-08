#pragma once
#include <vector>
#include <iostream>
#include "funcStack.h"
#include "derror.h"
#include <stack>
using namespace std;

using namespace std;


class arrayTem {
public:
	int dimn;//���ķ�ֻ֧��һά����
	int len;
	datatype tp;
	void* addr;
	arrayTem() {
		tp = dINT;
		len = 1;
		dimn = 1;
	}
	arrayTem(int len, int dim, datatype tp, void* addr) : len(len), dimn(dim), tp(tp), addr(addr) {

	}
};

class record {
public:
	string name;
	classtype typeC;
	datatype typeD;
	arrayTem addr;// �洢��ַ��Ҳ����ָ��ģ���ַ

	record() {
		
	}
	record(string name, classtype c, datatype d) {
		this->name = name;
		this->typeC = c;
		this->typeD = d;
	}

};

class runStack {
public:
	vector<runStack*> display;//display���洢���ϼ���ָ��
	funcRecord* father;
	int ptr;
	//ʡ�Ի���ַָ��,��ѡ����*/&���
	//���ص�ַʡ��
	runStack* preabp;//��һ�����
	vector<record> dataStack;

	runStack() {
		this->preabp = NULL;
		ptr = -1;//ģ��ջָ��
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
			ptrtmp--;
		}
		if (!write) {
			for (runStack *bufS : display) {
				int ptrtmp2 = bufS->ptr;
				while (ptrtmp2 >= 0) {
					record tmp = bufS->dataStack.at(ptrtmp2);
					if (tmp.name == name) {
						return tmp;
					}
					ptrtmp2--;
				}
			}
		}
		throw myexception('c');
	}

	void writeStack(record rec) {
		try {
			record before = findRecord(rec.name, true);
		}
		catch (myexception ex) { // ��ȷ�����okk
			this->dataStack.push_back(rec);
			this->ptr++;
			return;
		}
		throw myexception('b');//û���쳣˵���������ض���
	}

};

static stack<runStack> myrunStack;
