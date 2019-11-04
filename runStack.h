#pragma once
#include <vector>
#include <iostream>
#include "derror.h"
#include <stack>
using namespace std;

using namespace std;
static enum classtype {//�������
	CONST,
	VAR
};

static enum datatype {//��������
	INT,
	CHAR,
	ARRAY,
	VOID
};

class arrayTem {
public:
	int dimn;//���ķ�ֻ֧��һά����
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
	int addr;// �洢��ַ��Ҳ����ָ��ģ���ַ

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
	vector<runStack*> display;//display���洢���ϼ���ָ��
	int ptr;
	//ʡ�Ի���ַָ��,��ѡ����*/&���
	//���ص�ַʡ��
	runStack* preabp;//��һ�����
	vector<record> dataStack;
	funcRecord father;

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
		catch (myexception ex) { // ��ȷ�����okk
			dataStack.push_back(rec);
			return;
		}
		throw myexception('b');//û���쳣˵���������ض���
	}

};

static stack<runStack> myrunStack;
