

#include "wordAna.h"
#include "grammerTree.h"
#include "derror.h"
#include "funcStack.h"
#include "runStack.h"

#include <iostream>
#include<cstdio>
#include <vector>  
#include <stdexcept> 
using namespace std;
vector<string> strlist;
vector<int> lnolist; //Ӧ�����ж�������ԭ���Ķ������뿪

int addN;
int globalAddr = 0;

bool findit;
treetype word2treeType(types ori) {
	if (ori == IDENFR)
		return rawIDENFR;
	else if (ori == INTCON)
		return rawINTCON;
	else if (ori == CHARCON)
		return rawCHARCON;
	else if (ori == STRCON)
		return rawSTRCON;
	else if (ori == CONSTTK)
		return rawCONSTTK;
	else if (ori == INTTK)
		return rawINTTK;
	else if (ori == CHARTK)
		return rawCHARTK;
	else if (ori == VOIDTK)
		return rawVOIDTK;
	else if (ori == MAINTK)
		return rawMAINTK;
	else if (ori == IFTK)
		return rawIFTK;
	else if (ori == ELSETK)
		return rawELSETK;
	else if (ori == DOTK)
		return rawDOTK;
	else if (ori == WHILETK)
		return rawWHILETK;
	else if (ori == FORTK)
		return rawFORTK;
	else if (ori == SCANFTK)
		return rawSCANFTK;
	else if (ori == PRINTFTK)
		return rawPRINTFTK;
	else if (ori == RETURNTK)
		return rawRETURNTK;
	else if (ori == PLUS)
		return rawPLUS;
	else if (ori == MINU)
		return rawMINU;
	else if (ori == MULT)
		return rawMULT;
	else if (ori == DIV)
		return rawDIV;
	else if (ori == LSS)
		return rawLSS;
	else if (ori == LEQ)
		return rawLEQ;
	else if (ori == GRE)
		return rawGRE;
	else if (ori == GEQ)
		return rawGEQ;
	else if (ori == EQL)
		return rawEQL;
	else if (ori == NEQ)
		return rawNEQ;
	else if (ori == ASSIGN)
		return rawASSIGN;
	else if (ori == SEMICN)
		return rawSEMICN;
	else if (ori == COMMA)
		return rawCOMMA;
	else if (ori == LPARENT)
		return rawLPARENT;
	else if (ori == RPARENT)
		return rawRPARENT;
	else if (ori == LBRACK)
		return rawLBRACK;
	else if (ori == RBRACK)
		return rawRBRACK;
	else if (ori == LBRACE)
		return rawLBRACE;
	else if (ori == RBRACE)
		return rawRBRACE;
	else
		return NONE;
}

int findUint(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) == rawINTCON) {
			cout<<"uint"<<endl;
			findit = true;
			return begin + 1;
		}
		findit = false;
		return begin;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findInt(vector<treetype>& oriList, int begin) {
	try {
		int pos = begin;
		if (oriList.at(pos) == rawPLUS || oriList.at(pos) == rawMINU) {
			pos++;
		}
		int posbuf = findUint(oriList, pos);
		if (!findit)
			return begin;
		cout<<"int "<<endl;
		findit = true;
		return posbuf;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findConstTk(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) == rawCONSTTK) {
			findit = true;
			return begin + 1;
		}
		findit = false;
		return begin;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findConstDef(vector<treetype>& oriList, int begin) {
	try {
		datatype tp;
		int pos = begin;
		if (oriList.at(begin) == rawINTTK) {
			tp = datatype::dINT;
			pos = begin + 1;
			if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawASSIGN)
			{
				findit = false;
				return begin;
			}
			runStack nowenv = myrunStack.top();
			
			record newone(strlist.at(pos), CONST, tp, globalAddr++);
			nowenv.writeStack(newone);
			pos += 2;
			int posbuf = findInt(oriList, pos);
			if (!findit)
				return begin;
			pos = posbuf;
			while (oriList.at(pos) == rawCOMMA) {
				pos++;
				if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawASSIGN)
				{
					findit = false;
					return begin;
				}
				record newone(strlist.at(pos), CONST, tp, globalAddr++);
				nowenv.writeStack(newone);
				pos += 2;
				posbuf = findInt(oriList, pos);
				if (!findit)
					return begin;
				pos = posbuf;
			}
			pos--;//����posͣ�������ƥ�䵽������
			if (oriList.at(pos + 1) != rawSEMICN)
			{
				findit = false;
				return begin;
			}
			cout<<"constDef "<<endl;
			findit = true;
			return pos + 1;
		}
		else if (oriList.at(begin) == rawCHARTK) {
			tp =  datatype::dCHAR;
			pos = begin + 1;
			if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawASSIGN
				|| oriList.at(pos + 2) != rawCHARCON)
			{
				findit = false;
				return begin;
			}
			runStack nowenv = myrunStack.top();
			record newone(strlist.at(pos), CONST, tp, globalAddr++);
			nowenv.writeStack(newone);
			pos += 3;
			while (oriList.at(pos) == rawCOMMA) {
				pos++;
				if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawASSIGN ||
					oriList.at(pos + 2) != rawCHARCON)
				{
					findit = false;
					return begin;
				}
				record newone(strlist.at(pos), CONST, tp, globalAddr++);
				nowenv.writeStack(newone);
				pos += 3;
							}
			pos--;//����posͣ�������ƥ�䵽������
			if (oriList.at(pos + 1) != rawSEMICN)
			{
				findit = false;
				return begin;
			}
			cout<<"constDef "<<endl;
			findit = true;
			return pos + 1;
		}
		else
			return begin;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findConstDeclare(vector<treetype>& oriList, int begin) {
	try {
		int pos = findConstTk(oriList, begin);
		if (!findit) {
			return begin;
		}
		pos = findConstDef(oriList, pos);
		if (!findit || oriList.at(pos) != rawSEMICN)
		{
			findit = false;
			return begin;
		}
		pos++;
		int posbuf = findConstTk(oriList, pos);
		while (findit) {
			pos = posbuf;
			pos = findConstDef(oriList, pos);
			if (oriList.at(pos) != rawSEMICN)
			{
				findit = false;
				return begin;
			}
			pos++;
			posbuf = findConstTk(oriList, pos);
		}
		cout<<"constDeclare"<<endl;
		findit = true;
		return pos;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findVarDef(vector<treetype>& oriList, int begin) {
	try {
		runStack nowenv = myrunStack.top();
		datatype tp;
		if (oriList.at(begin) != rawCHARTK && oriList.at(begin) != rawINTTK) {
			findit = false;
			return begin;
		}
		tp = (oriList.at(begin) == rawCHARTK) ? datatype::dCHAR : datatype::dINT;
		int pos = begin + 1;
		if (oriList.at(pos) != rawIDENFR) {
			findit = false;
			return begin;
		}
		string name = strlist.at(pos);
		pos++;
		if (oriList.at(pos) == rawLBRACK) {
			pos++;
			int posbuf = findUint(oriList, pos);
			if (!findit)
				return begin;
			int len = atoi(strlist.at(pos).c_str());
			pos = posbuf;
			if (oriList.at(pos) != rawRBRACK) {
				findit = false;
				return begin;
			}
			arrayTem temp(len, 1, tp, globalAddr++);
			int addrtem = (int)& temp;
			record newone(name, VAR, dARRAY, addrtem);
			nowenv.writeStack(newone);
			pos++;
		}
		else {
			record newone(name, VAR, tp, globalAddr++);
			nowenv.writeStack(newone);
		}
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			if (oriList.at(pos) != rawIDENFR) {
				findit = false;
				return begin;
			}
			name = strlist.at(pos);
			pos++;
			if (oriList.at(pos) == rawLBRACK) {
				pos++;
				int posbuf = findUint(oriList, pos);
				if (!findit)
					return begin;
				int len = atoi(strlist.at(pos).c_str());;
				pos = posbuf;
				if (oriList.at(pos) != rawRBRACK) {
					findit = false;
					return begin;
				}
				arrayTem temp(len, 1, tp, globalAddr++);
				int addrtem = (int)& temp;
				record newone(name, VAR, dARRAY, addrtem);
				nowenv.writeStack(newone);
				pos++;
			}
			else {
				record newone(name, VAR, tp, globalAddr++);
				nowenv.writeStack(newone);
			}
		}
		if (oriList.at(pos) != rawSEMICN)
		{
			findit = false;
			return begin;
		}
		cout<<"varDef"<<endl;
		findit = true;
		return pos;//����ֵ����һ���µĿ�ʼ
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findComplexSentence(vector<treetype>& oriList, int begin) {
	try {
		int pos = findConstDeclare(oriList, begin);
		findit = true;
		pos = findVarDeclare(oriList, pos);
		findit = true;
		pos = findSentenceList(oriList, pos);
		if (!findit) return begin;
		cout<<"������� "<<endl;
		findit = true;
		return pos;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findVarDeclare(vector<treetype>& oriList, int begin) {
	try {
		int posbuf = begin;
		int pos = findVarDef(oriList, begin);
		if (!findit || oriList.at(pos) != rawSEMICN)
		{
			findit = false;
			return begin;
		}
		pos++;
		posbuf = pos;
		pos = findVarDef(oriList, posbuf);
		while (findit) {
			if (oriList.at(pos) != rawSEMICN)
			{
				findit = false;
				return begin;
			}
			pos++;
			posbuf = pos;
			pos = findVarDef(oriList, posbuf);
		}
		cout<<"�������� "<<endl;
		findit = true;
		return pos;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findDeclareHead(vector<treetype>& oriList, int begin, funcRecord& newF) {
	try {
		treetype first = oriList.at(begin);
		if (first != rawINTTK && first != rawCHARTK) {
			findit = false;
			return begin;
		}
		newF.returnT = (first == rawINTTK) ?  datatype::dINT:  datatype::dCHAR;
		if (oriList.at(begin + 1) != rawIDENFR) {
			findit = false;
			return begin;
		}
		newF.name = strlist.at(begin + 1);
		cout<<"����ͷ�� "<<endl;
		findit = true;
		return begin + 2;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findParamList(vector<treetype>& oriList, int begin, funcRecord& newF, runStack& env) {
	try {
		int pos = begin;
		vector<datatype> paraL;
		if (oriList.at(pos) != rawINTTK && oriList.at(pos) != rawCHARTK
			&& oriList.at(pos) == rawRPARENT) {
			newF.paraList = paraL;
			cout<<"�����б�"<<endl;
			findit = true;
			return pos ;
		}
		datatype buf = (oriList.at(pos) == rawINTTK) ?  datatype::dINT :  datatype::dCHAR;
		paraL.push_back(buf);
		pos++;
		if (oriList.at(pos) != rawIDENFR) {
			//cout << "invalid gramma in paralist, don't know how to deal" << endl;
			findit = false;
			return begin;
		}
		record newrec(strlist.at(pos), VAR, buf, globalAddr++);
		try {
			env.writeStack(newrec);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos);
			ex.pntMsg();
		}
		pos++;
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			if (oriList.at(pos) != rawINTTK && oriList.at(pos) != rawCHARTK)
			{
				findit = false;
				return begin;
			}
			buf = (oriList.at(pos) == rawINTTK) ?  datatype::dINT :  datatype::dCHAR;
			paraL.push_back(buf);
			pos++;
			if (oriList.at(pos) != rawIDENFR) {
				findit = false;
				return begin;
			}
			record newrec(strlist.at(pos), VAR, buf, globalAddr++);
			try {
				env.writeStack(newrec);
			}
			catch (myexception ex) {
				ex.lineno = lnolist.at(pos);
				ex.pntMsg();
			}
			pos++;
		}
		newF.paraList = paraL;
		cout<<"������ "<<endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findFuncReturnDef(vector<treetype>& oriList, int begin) {
	try {
		funcRecord newF;
		int pos = findDeclareHead(oriList, begin, newF);
		if (!findit || oriList.at(pos) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		pos++;
		runStack nowenv = myrunStack.top();
		runStack newenv(&nowenv);
		newenv.father = newF;
		pos = findParamList(oriList, pos, newF, newenv); //�����ǿյģ�Ҳ�����ʾ�����𣿣�����������ǵ�
		if (!findit) return begin;
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos - 1);
			ex.pntMsg();
		} 
		if (oriList.at(pos) != rawRPARENT || oriList.at(pos + 1) != rawLBRACE) {
			findit = false;
			return begin;
		}
		pos += 2;

		myrunStack.push(newenv);
		pos = findComplexSentence(oriList, pos);
		myrunStack.pop();//�����ǲ����ҵ�������䶼Ҫ����
		if (newF.rtN == 0) {
			myexception('h', lnolist.at(pos - 1), "�ú���ȱ��return ���");
		}
		if (!findit || oriList.at(pos) != rawRBRACE) {
			findit = false;
			return begin;
		}
		
		cout<<"�з���ֵ�������� "<<endl;
		findit = true;
		return pos + 1;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findSentenceList(vector<treetype>& oriList, int begin) {
	try {
		int pos = findSentence(oriList, begin);
		int posbuf = begin;
		while (pos != posbuf) {
			posbuf = pos;
			pos = findSentence(oriList, posbuf);
		}
		cout<<"����� "<<endl;
		findit = true;//����Ϊ0��
		return pos;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findFactor(vector<treetype>& oriList, int begin, datatype &tp) {
	try {
		int pos = begin;
		if (oriList.at(begin) == rawCHARCON) {
			tp =  datatype::dCHAR;
			cout<<"���� "<<endl;
			findit = true;
			return begin + 1;
		}
		else if (oriList.at(begin) == rawIDENFR) {
			int posbuf = findUseFunc(oriList, begin, tp);
			if (!findit) {
				runStack nowenv = myrunStack.top();
				try {
					record rebuf = nowenv.findRecord(strlist.at(begin));
					tp = rebuf.typeD;
					if (tp == dARRAY && oriList.at(begin+1) != rawLBRACK ) {
						throw myexception('c', lnolist.at(begin + 1), "�ñ�����һ�����飬����dARRAY���ܵ��³���");
					}
					if (oriList.at(begin + 1) == rawLBRACK) {
						datatype tp2;
						pos = findExpress(oriList, pos + 2, tp2);
						if (!findit) return begin;
						if (tp2 != datatype::dINT) {
							myexception ex('i', lnolist.at(pos - 1), "�����±겻�����α��ʽ");
							ex.pntMsg();
						}
						if (oriList.at(pos) != rawRBRACK)
						{
							findit = false;
							return begin;
						}

						cout << "���� " << endl;
						findit = true;
						return pos + 1;
					}
					else {
						tp = rebuf.typeD;
						cout << "����" << endl;
						findit = true;
						return pos + 1;
					}
				}
				catch (myexception ex) {
					ex.lineno = lnolist.at(begin);
					ex.msg = "û�������ı���";
					ex.pntMsg();
					findit = true;
					tp = datatype::dINT;
					pos = begin + 1;
					if (oriList.at(pos) != rawLBRACK) {
						return pos;
					}
					else {
						pos++;
						while (lnolist.at(begin) == lnolist.at(pos) && oriList.at(pos) != rawRBRACK) pos++;
						if (oriList.at(pos) == rawRBRACK)
							return pos + 1;
						else return pos;
					}
				}
			}
			else {
				if (tp == datatype::dVOID) {
					//error! ������void������Ϊ����
				}
				cout<<"���� "<<endl;
				findit = true;
				return posbuf ;
			}
		}
		else if (oriList.at(begin) == rawLPARENT) {
			pos = begin + 1;
			pos = findExpress(oriList, pos, tp);
			if (!findit || oriList.at(pos) != rawRPARENT)
			{
				findit = false;
				return begin;
			}
			cout<<"���� "<<endl;
			findit = true;
			return pos + 1;
		}
		else {
			int posbuf = findInt(oriList, pos);
			if (!findit) {
				return begin;
			}
			else {
				cout<<"���� "<<endl;
				findit = true;
				return posbuf ;
			}
		}
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findItem(vector<treetype>& oriList, int begin, datatype& tp) {
	try {
		datatype tpbuf;
		int pos = findFactor(oriList, begin, tp);
		if (pos == begin) {
			findit = false;
			return begin;
		}
		while (oriList.at(pos) == rawMULT || oriList.at(pos) == rawDIV) {
			pos++;
			pos = findFactor(oriList, pos, tpbuf);
			if (!findit) return begin;
			tp = (tpbuf ==  datatype::dINT) ?  datatype::dINT : tp;
		}
		cout << "item" << endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findExpress(vector<treetype>& oriList, int begin, datatype& tp) {
	try {
		tp =  datatype::dINT;
		datatype tpbuf;
		int pos = begin;
		if (oriList.at(begin) == rawPLUS || oriList.at(begin) == rawMINU) {
			pos++;
		}
		int posbuf = findItem(oriList, pos, tp);
		if (!findit) {
			findit = false;
			return begin;
		}
		pos = posbuf;
		while (oriList.at(pos) == rawPLUS || oriList.at(pos) == rawMINU) {
			pos++;
			pos = findItem(oriList, pos, tpbuf);
			if (findit = false)
				return begin; //Ĭ�ϲ�������+-��β�ı��ʽ
			tp = (tpbuf ==  datatype::dINT) ?  datatype::dINT : tp;
		}
		cout<<"�� "<<endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findCondition(vector<treetype>& oriList, int begin) {
	try {
		 int pos = begin;
		datatype tp;
		try {
			pos = findExpress(oriList, pos, tp);
			if (!findit) return begin;
			if (tp !=  datatype::dINT) {
				throw myexception('f', lnolist.at(pos - 1), "�����еı��ʽ���������ε�");
			}
			if (oriList.at(pos) >= rawLSS && oriList.at(pos) <= rawNEQ) {
				pos++;
				pos = findExpress(oriList, pos, tp);
				if (!findit) return begin;
				if (tp !=  datatype::dINT) {
					throw myexception('f', lnolist.at(pos - 1), "�����еı��ʽ���������ε�");
				}
			}
		}
		catch (myexception ex) {
			ex.pntMsg();
		}
		cout<<"���� "<<endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findStep(vector<treetype>& oriList, int begin) {
	try {
		int pos = begin;
		pos = findUint(oriList, begin);
		if (findit) {
			cout<<"���� "<<endl;
			findit = true;
			return pos ;
		}
		return begin;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findIfS(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) != rawIFTK || oriList.at(begin + 1) != rawLPARENT) {
			findit = false;
			return begin;
		}
		int pos = begin + 2;
		pos = findCondition(oriList, pos);
		
		if (!findit || oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		pos++;
		pos = findSentence(oriList, pos);
		if (!findit)
			return begin;
		if (oriList.at(pos) == rawELSETK) {
			pos++;
		pos = findSentence(oriList, pos);
		if (!findit)
			return begin;
		}
		cout<<"�������"<<endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findWhileS(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) != rawWHILETK || oriList.at(begin + 1) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		int pos = begin + 2;
		pos = findCondition(oriList, pos);
		if (!findit || oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		pos++;
		pos = findSentence(oriList, pos);
		if (!findit) return begin;
		cout<<"ѭ����� "<<endl;
		findit = true;
		return pos;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findDoS(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) != rawDOTK)
		{
			findit = false;
			return begin;
		}
		int pos = begin + 1;
		pos = findSentence(oriList, pos);
		if (!findit)
			return begin;
		if (oriList.at(pos) != rawWHILETK) {
			myexception ex('n', lnolist.at(pos), "do-whileȱ��while");
			ex.pntMsg();
			return pos;
		}
		if (oriList.at(pos + 1) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		pos += 2;
		pos = findCondition(oriList, pos);
		if (!findit || oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		cout<<"ѭ�����"<<endl;
		findit = true;
		return pos + 1;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findForS(vector<treetype>& oriList, int begin) {

	try {
		if (oriList.at(begin) != rawFORTK || oriList.at(begin + 1) != rawLPARENT
			|| oriList.at(begin + 2) != rawIDENFR || oriList.at(begin + 3) != rawASSIGN)
		{
			findit = false;
			return begin;
		}
		runStack nowenv = myrunStack.top();
		try {
			nowenv.findRecord(strlist.at(begin + 2), false);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(begin + 2);
			ex.pntMsg();
		}
		int pos = begin + 4;
		datatype tp;
		pos = findExpress(oriList, pos, tp);
		if (!findit)
			return begin;

		if (oriList.at(pos) != rawSEMICN)
		{
			findit = false;
			return begin;
		}
		pos++;
		pos = findCondition(oriList, pos);
		if (!findit || oriList.at(pos) != rawSEMICN || oriList.at(pos + 1) != rawIDENFR
			|| oriList.at(pos + 2) != rawASSIGN || oriList.at(pos + 3) != rawIDENFR)
		{
			findit = false;
			return begin;
		}
		try {
			nowenv.findRecord(strlist.at(pos + 1), false);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos + 1);
			ex.pntMsg();
		}

		pos += 4;
		if (oriList.at(pos) != rawPLUS && oriList.at(pos) != rawMINU)
		{
			findit = false;
			return begin;
		}
		pos++;
		pos = findStep(oriList, pos); //����ֻ��ָ�����·�����
		if (!findit || oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		pos++;
		pos = findSentence(oriList, pos);
		if (!findit)
			return begin;
		cout<<"ѭ����� "<<endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findScanfS(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) != rawSCANFTK || oriList.at(begin + 1) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		int pos = begin + 2;
		if (oriList.at(pos) != rawIDENFR)
		{
			findit = false;
			return begin;
		}
		runStack nowenv = myrunStack.top();
		try {
			nowenv.findRecord(strlist.at(pos), false);
		}
		catch (myexception ex) {
			ex.lineno = pos;
			ex.pntMsg();
		}
		pos++;
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			if (oriList.at(pos) != rawIDENFR)
			{
				findit = false;
				return begin;
			}
			try {
				nowenv.findRecord(strlist.at(pos), false);
			}
			catch (myexception ex) {
				ex.lineno = pos;
				ex.pntMsg();
			}
			pos++;
		}
		if (oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		cout << "readS" << endl;
		findit = true;
		return pos + 1;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findPrintfS(vector<treetype>& oriList, int begin) {
	try {
		datatype tp;
		if (oriList.at(begin) != rawPRINTFTK || oriList.at(begin + 1) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		int pos = begin + 2;
		if (oriList.at(pos) == rawSTRCON) {
			pos++;
			cout << "strCon" << endl;
			findit = true;
			//pos++;
			if (oriList.at(pos) == rawCOMMA) {
				pos++;
				pos = findExpress(oriList, pos, tp);
				if (!findit)
					return begin;
			}
			if (oriList.at(pos) != rawRPARENT)
			{
				findit = false;
				return begin;
			}
			cout << "writeS" << endl;
			findit = true;
			return pos + 1;
		}
		else {
			pos = findExpress(oriList, pos, tp);
			if (oriList.at(pos) != rawRPARENT)
			{
				findit = false;
				return begin;
			}
			cout<<"writeS "<<endl;
			findit = true;
			return pos + 1;
		}
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findReturnS(vector<treetype>& oriList, int begin) {
	try {
		datatype tp;
		if (oriList.at(begin) != rawRETURNTK) {
			findit = false;
			return begin;
		}
		int pos = begin + 1;
		if (oriList.at(pos) == rawLPARENT) {
			pos = findExpress(oriList, pos + 1, tp);
			if (!findit)
				return begin; //���û�б��ʽ��û������

			try {
				runStack nowenv = myrunStack.top();
				funcRecord father = nowenv.father;
				if (father.returnT == datatype::dVOID) {
					throw myexception('g', lnolist.at(pos - 1), "VOID���������˷�void�������");
				}
				else if (father.returnT != tp) {
					throw myexception('h', lnolist.at(pos - 1), "return�����������Ͳ�ƥ��");
				}
				father.rtN++;
			}
			catch (myexception ex) {
				ex.pntMsg();
			}
			if (oriList.at(pos) != rawRPARENT) {
				findit = false;
				return begin;
			}
			
			cout<<"�������"<<endl;
			findit = true;
			return pos + 1;
		}
		else if (oriList.at(pos) == rawSEMICN) {
			tp = dVOID;
			try {
				runStack nowenv = myrunStack.top();
				funcRecord father = nowenv.father;
				if (father.returnT != datatype::dVOID) {
					myexception('h', lnolist.at(pos), "����ֵ��������void");
				}
			}
			catch (myexception ex) {
				ex.pntMsg();
			}
			cout<<"������� "<<endl;
			findit = true;
			return pos ;
		}
		else {
			findit = false;
			return begin;
		}
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findSentence(vector<treetype>& oriList, int begin) {
	try {
		int pos = begin;
		if (oriList.at(begin) == rawIFTK) {
			pos = findIfS(oriList, pos);
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos ;
		}
		else if (oriList.at(begin) == rawWHILETK) {
			pos = findWhileS(oriList, pos);
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawDOTK) {
			pos = findDoS(oriList, pos);
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawFORTK) {
			pos = findForS(oriList, pos);
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos;
		} // ѭ�����
		else if (oriList.at(begin) == rawLBRACE) {
			pos++;
			pos = findSentenceList(oriList, pos);
			if (!findit)
			{
				return begin;
			}
			if (oriList.at(pos) != rawRBRACE)
			{
				findit = false;
				return begin;
			}
			pos++;
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawSCANFTK) {
			pos = findScanfS(oriList, pos);
			if (oriList.at(pos) != rawSEMICN)
			{
				findit = false;
				return begin;
			}
			pos++;
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos ;
		}
		else if (oriList.at(begin) == rawPRINTFTK) {
			pos = findPrintfS(oriList, pos);
			if (oriList.at(pos) != rawSEMICN)
			{
				findit = false;
				return begin;
			}

			pos++;
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawSEMICN) {
			findit = true;
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos + 1;
		}
		else if (oriList.at(begin) == rawRETURNTK) {
			pos = findReturnS(oriList, pos);
			if (!findit || oriList.at(pos) != rawSEMICN)
			{
				findit = false;
				return begin;
			}
			pos++;
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawIDENFR) {
			int posbuf = pos;
			datatype rt;
			pos = findUseFunc(oriList, posbuf, rt);
			if (!findit) {
				pos = findAssignS(oriList, posbuf);
				if (!findit)
				{
					findit = false;
					return begin;//error happen
				}
			}
			if (oriList.at(pos) != rawSEMICN)
			{
				findit = false;
				return begin;
			}
			pos++;
			if (findit)
			{
				cout << "���" << endl;
			}
			return pos;

		}
		else
			return begin;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findAssignS(vector<treetype>& oriList, int begin) {
	try {
		int pos = begin;
		if (oriList.at(pos) != rawIDENFR)
		{
			findit = false;
			return begin;
		}
		runStack nowenv = myrunStack.top();
		record var;
		try {
			var = nowenv.findRecord(strlist.at(pos));
			if (var.typeC == CONST) {
				myexception  ex('j', lnolist.at(pos), "����Ϊ����" + strlist.at(pos) + "��ֵ");
				ex.pntMsg();
			}

			pos += 1;
			if (oriList.at(pos) == rawLBRACK) {
				pos++;
				datatype tp;
				pos = findExpress(oriList, pos, tp);
				if (findit == false) {
					return begin;
				}
				if (tp != datatype::dINT) {
					myexception ex('i', lnolist.at(pos - 1), "��ֵ����������±겻������");
					ex.pntMsg();
				}
				if (oriList.at(pos) != rawRBRACK)
				{
					findit = false;
					return begin;
				}
				pos++;
			}
		}
		catch (myexception ex) {
			ex.lineno = pos;
			ex.msg = "δ����ı�����";
			ex.pntMsg();
		}
		if (oriList.at(pos) != rawASSIGN)
		{
			findit = false;
			return begin;
		}
		pos++;
		datatype tp2;
		pos = findExpress(oriList, pos, tp2);
		if (findit == false) {
			return begin;
		}
		cout<<"��ֵ��� "<<endl;
		findit = true;
		return pos;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findValueParaList(vector<treetype>& oriList, int begin, vector<datatype>& paraL) {
	try {
		datatype buft;
		int pos = findExpress(oriList, begin, buft);
		if (!findit) return pos;
		paraL.push_back(buft);
		//�����Ƿ��ҵ�experss���ǶԵ�
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			pos = findExpress(oriList, pos, buft);
			if (!findit) return begin;
			paraL.push_back(buft);
		}
		cout<<"ֵ�����б� "<<endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findUseFunc(vector<treetype>& oriList, int begin, datatype& tp) {
	try {
		tp = datatype::dINT;
		int pos = begin;
		if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		string name = strlist.at(pos);
		pos += 2;
		vector<datatype> paraL;
		pos = findValueParaList(oriList, pos, paraL);
		if (findit == false) {
			return begin;
		}
		try {
			funcRecord fun = find_func(name, paraL);
			tp = fun.returnT;
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos - 1);
			ex.msg = "û�������ĺ���";
			ex.pntMsg();
		}
		if (oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		if (tp == datatype::dVOID) {
			cout << "ʹ�ò�������ֵ�ĺ���" << endl;
		}
		else {
			cout << "ʹ�÷���ֵ�ĺ���" << endl;
		}
		findit = true;
		return pos + 1;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}


int findFuncVoidDef(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) != rawVOIDTK || oriList.at(begin + 1) != rawIDENFR
			|| oriList.at(begin + 2) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		runStack preenv = myrunStack.top();
		funcRecord newF = funcRecord(strlist.at(begin+1), dVOID);
		int pos = begin + 3;
		runStack newenv(&preenv);
		newenv.father = newF;
		pos = findParamList(oriList, pos, newF, newenv);
		if (findit == false || oriList.at(pos) != rawRPARENT || oriList.at(pos + 1) != rawLBRACE) {
			findit = false;
			return begin;
		}
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos - 1);
			ex.msg = "void ������������ͬ�κ���";
			ex.pntMsg();
		}
		myrunStack.push(newenv);
		pos += 2;
		pos = findComplexSentence(oriList, pos);
		myrunStack.pop();
		if (findit == false || oriList.at(pos) != rawRBRACE) {
			findit = false;
			return begin;
		}
		cout << "void��������" << endl;
		findit = true;
		return pos + 1;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findFuncDef(vector<treetype>& oriList, int begin) {
	try {
		int buf = begin;
		int pos1 = findFuncReturnDef(oriList, begin);
		int pos2 = findFuncVoidDef(oriList, begin);
		while (pos1 != pos2) { //�����Ҫ��֤���û�ҵ�һ������begin��
			buf = (pos1 > pos2 ? pos1 : pos2);
			pos1 = findFuncReturnDef(oriList, buf);
			pos2 = findFuncVoidDef(oriList, buf);
		}
		findit = true;
		return buf;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findMainFunc(vector<treetype>& oriList, int begin) {
	try {
		if (oriList.at(begin) != rawVOIDTK || oriList.at(begin + 1) != rawMAINTK
			|| oriList.at(begin + 2) != rawLPARENT || oriList.at(begin + 3) != rawRPARENT
			|| oriList.at(begin + 4) != rawLBRACE)
		{
			findit = false;
			return begin;
		}
		vector<datatype> paraLnull;
		funcRecord newF("main", datatype::dVOID,paraLnull); //Ҫ��Ҫ��MAINҲ�Ž������б��أ������Ȳ���
		runStack preenv = myrunStack.top();
		runStack mainenv(&preenv);
		mainenv.father = newF;
		myrunStack.push(mainenv);
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(begin + 1);
			ex.pntMsg();
		}
		int pos = begin + 5;
		pos = findComplexSentence(oriList, pos);
		myrunStack.pop();
		if (findit == false) {
			return begin;
		}
		if (oriList.at(pos) != rawRBRACE) {
			findit = false;
			return begin;
		}
		cout << "mainFUN" << endl;
		findit = true;
		return pos + 1;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

void findProgram(vector<treetype>& finalList, int begin) {
	//create base env
	runStack base;
	myrunStack.push(base);
	findit = true;
	int pos = findConstDeclare(finalList, begin);
	findit = true;
	pos = findVarDeclare(finalList, pos);
	findit = true; //which means i dont care if findit
	pos = findFuncDef(finalList, pos);
	findit = true;
	pos = findMainFunc(finalList, pos);
	if (pos != finalList.size() || findit == false) {
		cout << "error " << endl;
	}
	else {
		//finalList.insert(finalList.begin() + pos, PROGRAM);
		cout << "����" << endl;
		findit = true;
	}

}

int main() {
	freopen("output.txt", "w", stdout);
	freopen("error.txt", "w", stderr);
	wordAna baseWord;
	baseWord.initWordList();

	vector<types> wordlist = baseWord.wordList;
	
	strlist = baseWord.strlist;
	lnolist = baseWord.lnolist;
	
	vector<treetype> finalList;
	for (types oneword : wordlist) {
		finalList.push_back(word2treeType(oneword));
	}
	findit = false;
	findProgram(finalList, 0);
	
	//errList = err.errStrlist;
	//for (string msg : errList) {
		//std::cerr << msg << endl;
	//}
	fclose(stdout);
	fclose(stderr);

	
	return 0;
	
}
