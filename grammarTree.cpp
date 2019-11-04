

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
vector<int> lnolist; //应该让判断类型与原来的东西分离开

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
			tp = INT;
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
			pos--;//这里pos停留在最后匹配到整数上
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
			tp = CHAR;
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
			pos--;//这里pos停留在最后匹配到整数上
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
		tp = (oriList.at(begin) == rawCHARTK) ? CHAR : INT;
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
			record newone(name, VAR, ARRAY, addrtem);
			nowenv.writeStack(newone);
			pos++;
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
				record newone(name, VAR, ARRAY, addrtem);
				nowenv.writeStack(newone);
				pos++;
			}
		}
		if (oriList.at(pos) != rawSEMICN)
		{
			findit = false;
			return begin;
		}
		cout<<"varDef"<<endl;
		findit = true;
		return pos;//返回值都是一个新的开始
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
		cout<<"复合语句 "<<endl;
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
		cout<<"变量声明 "<<endl;
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
		newF.returnT = (first == rawINTTK) ? INT: CHAR;
		if (oriList.at(begin + 1) != rawIDENFR) {
			findit = false;
			return begin;
		}
		newF.name = strlist.at(begin + 1);
		cout<<"声明头部 "<<endl;
		findit = true;
		return begin + 2;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findParamList(vector<treetype>& oriList, int begin, funcRecord& newF) {
	try {
		int pos = begin;
		vector<datatype> paraL;
		if (oriList.at(pos) != rawINTTK && oriList.at(pos) != rawCHARTK
			&& oriList.at(pos) == rawRPARENT) {
			newF.paraList = paraL;
			cout<<"参数列表"<<endl;
			findit = true;
			return pos ;
		}
		datatype buf = (oriList.at(pos) == rawINTTK) ? INT : CHAR;
		paraL.push_back(buf);
		pos++;
		if (oriList.at(pos) != rawIDENFR) {
			//cout << "invalid gramma in paralist, don't know how to deal" << endl;
			findit = false;
			return begin;
		}
		pos++;
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			if (oriList.at(pos) != rawINTTK && oriList.at(pos) != rawCHARTK)
			{
				findit = false;
				return begin;
			}
			buf = (oriList.at(pos) == rawINTTK) ? INT : CHAR;
			paraL.push_back(buf);
			pos++;
			if (oriList.at(pos) != rawIDENFR) {
				findit = false;
				return begin;
			}
			pos++;
		}
		newF.paraList = paraL;
		cout<<"参数表 "<<endl;
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
		pos = findParamList(oriList, pos, newF); //就算是空的，也必须表示出来吗？？？？？大概是的
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			myexception fex = myexception(ex.etype, lnolist.at(pos - 1), "");
			fex.pntMsg();
		}
		if (!findit || oriList.at(pos) != rawRPARENT || oriList.at(pos + 1) != rawLBRACE) {
			findit = false;
			return begin;
		}
		pos += 2;
		pos = findComplexSentence(oriList, pos);
		if (!findit || oriList.at(pos) != rawRBRACE) {
			findit = false;
			return begin;
		}
		
		cout<<"有返回值函数定义 "<<endl;
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
		cout<<"语句列 "<<endl;
		findit = true;//可以为0次
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findFactor(vector<treetype>& oriList, int begin, datatype &tp) {
	try {
		int pos = begin;
		if (oriList.at(begin) == rawCHARCON) {
			tp = CHAR;
			cout<<"因子 "<<endl;
			findit = true;
			return begin + 1;
		}
		else if (oriList.at(begin) == rawIDENFR) {
			int posbuf = findUseReturnFunc(oriList, begin, tp);
			if (!findit) {
				if (oriList.at(begin + 1) == rawLBRACK) {
					pos = findExpress(oriList, pos + 2, tp);
					if (!findit || oriList.at(pos) != rawRBRACK)
					{
						findit = false;
						return begin;
					}

					cout<<"因子 "<<endl;
					findit = true;
					return pos + 1;
				}
				else {
					runStack nowenv = myrunStack.top();
					record rebuf = nowenv.findRecord(strlist.at(begin));
					tp = (rebuf.typeD == INT) ? INT : tp;
					cout<<"因子"<<endl;
					findit = true;
					return pos + 1;
				}
			}
			else {
				cout<<"因子 "<<endl;
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
			cout<<"因子 "<<endl;
			findit = true;
			return pos + 1;
		}
		else {
			int posbuf = findInt(oriList, pos);
			if (!findit) {
				return begin;
			}
			else {
				cout<<"因子 "<<endl;
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
		int pos = findFactor(oriList, begin, tpbuf);
		tp = (tpbuf == INT) ? INT : CHAR;
		if (pos == begin) {
			findit = false;
			return begin;
		}
		while (oriList.at(pos) == rawMULT || oriList.at(pos) == rawDIV) {
			pos++;
			pos = findFactor(oriList, pos, tpbuf);
			if (!findit) return begin;
			tp = (tpbuf == INT) ? INT : tp;
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
		tp = INT;
		datatype tpbuf;
		int pos = begin;
		if (oriList.at(begin) == rawPLUS || oriList.at(begin) == rawMINU) {
			pos++;
		}
		int posbuf = findItem(oriList, pos, tpbuf);
		tp = (tpbuf == INT) ? INT : CHAR;
		if (!findit) {
			findit = false;
			return begin;
		}
		pos = posbuf;
		while (oriList.at(pos) == rawPLUS || oriList.at(pos) == rawMINU) {
			pos++;
			pos = findItem(oriList, pos, tpbuf);
			if (findit = false)
				return begin; //默认不存在用+-结尾的表达式
			tp = (tpbuf == INT) ? INT : tp;
		}
		cout<<"项 "<<endl;
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
			if (tp != INT) {
				throw myexception('f', lnolist.at(pos - 1), "");
			}
			if (oriList.at(pos) >= rawLSS && oriList.at(pos) <= rawNEQ) {
				pos++;
				pos = findExpress(oriList, pos, tp);
				if (!findit) return begin;
				if (tp != INT) {
					throw myexception('f', lnolist.at(pos - 1), "");
				}
			}
		}
		catch (myexception ex) {
			ex.pntMsg();
		}
		cout<<"条件 "<<endl;
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
			cout<<"步长 "<<endl;
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
		cout<<"条件语句"<<endl;
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
		cout<<"循环语句 "<<endl;
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
		if (oriList.at(pos) != rawWHILETK || oriList.at(pos + 1) != rawLPARENT)
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
		cout<<"循环语句"<<endl;
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
		pos = findStep(oriList, pos); //看来只能指定向下分析！
		if (!findit || oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		pos++;
		pos = findSentence(oriList, pos);
		if (!findit)
			return begin;
		cout<<"循环语句 "<<endl;
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
		pos++;
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			if (oriList.at(pos) != rawIDENFR)
			{
				findit = false;
				return begin;
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
				return begin; //如果没有表达式就没有括号
			try {
				runStack nowenv = myrunStack.top();
				int funaddr = nowenv.dataStack.at(0).addr;
				funcRecord* fatherfunc = (funcRecord*)funaddr;
				if (fatherfunc->returnT != tp) {
					if (fatherfunc->returnT == VOID) {
						throw myexception('g', lnolist.at(pos - 1), "");
					}
					else {
						throw myexception('h', lnolist.at(pos - 1), "");
					}
				}
			}
			catch (myexception ex) {
				ex.pntMsg();
			}
			if (oriList.at(pos) != rawRPARENT) {
				findit = false;
				return begin;
			}
			cout<<"返回语句"<<endl;
			findit = true;
			return pos + 1;
		}
		else if (oriList.at(pos) == rawSEMICN) {
			tp = VOID;
			try {
				runStack nowenv = myrunStack.top();
				int funaddr = nowenv.dataStack.at(0).addr;
				funcRecord* fatherfunc = (funcRecord*)funaddr;
				if (fatherfunc->returnT != tp) {
					if (fatherfunc->returnT != VOID) {
						throw myexception('h', lnolist.at(pos - 1), "non void func return null");
					}
				}
			}
			catch (myexception ex) {
				ex.pntMsg();
			}
			cout<<"返回语句 "<<endl;
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
				cout << "语句" << endl;
			}
			return pos ;
		}
		else if (oriList.at(begin) == rawWHILETK) {
			pos = findWhileS(oriList, pos);
			if (findit)
			{
				cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawDOTK) {
			pos = findDoS(oriList, pos);
			if (findit)
			{
				cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawFORTK) {
			pos = findForS(oriList, pos);
			if (findit)
			{
				cout << "语句" << endl;
			}
			return pos;
		} // 循环语句
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
				cout << "语句" << endl;
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
				cout << "语句" << endl;
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
				cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawSEMICN) {
			findit = true;
			if (findit)
			{
				cout << "语句" << endl;
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
				cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawIDENFR) {
			int posbuf = pos;
			pos = findUseReturnFunc(oriList, posbuf);
			if (!findit) {
				pos = findUseVoidFunc(oriList, posbuf);
				if (!findit) {
					pos = findAssignS(oriList, posbuf);
					if (!findit)
					{
						findit = false;
						return begin;//error happen
					}
					else {
						if (oriList.at(pos) != rawSEMICN)
						{
							findit = false;
							return begin;
						}
						pos++;
						if (findit)
						{
							cout << "语句" << endl;
						}
						return pos ;
					}
				}
				else {
					if (oriList.at(pos) != rawSEMICN)
					{
						findit = false;
						return begin;
					}	
					pos++;
					if (findit)
					{
						cout << "语句" << endl;
					}
					return pos;
				}
			}
			else {
				if (oriList.at(pos) != rawSEMICN)
				{
					findit = false;
					return begin;
				}
				pos++;
				if (findit)
				{
					cout << "语句" << endl;
				}
				return pos;
			}

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
		pos += 1;
		if (oriList.at(pos) == rawLBRACK) {
			pos++;
			pos = findExpress(oriList, pos);
			if (findit == false) {
				return begin;
			}
			if (oriList.at(pos) != rawRBRACK)
			{
				findit = false;
				return begin;
			}
			pos++;
		}
		if (oriList.at(pos) != rawASSIGN)
		{
			findit = false;
			return begin;
		}
		pos++;
		pos = findExpress(oriList, pos);
		if (findit == false) {
			return begin;
		}
		cout<<"赋值语句 "<<endl;
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
		//无论是否找到experss都是对的
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			pos = findExpress(oriList, pos, buft);
			if (!findit) return begin;
			paraL.push_back(buft);
		}
		cout<<"值参数列表 "<<endl;
		findit = true;
		return pos ;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findUseVoidFunc(vector<treetype>& oriList, int begin, datatype& tp) {
	try {
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
		funcRecord fun = find_func(name, paraL);
		if (fun.returnT != VOID)
			return begin;
		tp = fun.returnT;
		if (oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		cout << "使用不带返回值的函数" << endl;
		findit = true;
		return pos + 1;
	}
	catch (out_of_range& exc) {
		return begin;
	}
}

int findUseReturnFunc(vector<treetype>& oriList, int begin, datatype& tp) {
	try {
		int pos = begin;
		if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		string name = strlist.at(pos); 
		funcRecord func;
		vector<datatype> paral;
		pos += 2;
		pos = findValueParaList(oriList, pos, paral);
		if (findit == false) {
			return begin;
		}
		func = find_func(name, paral); // 没有找到会跳出去
		
		if (func.returnT == VOID)
			return begin;
		tp = func.returnT;
		if (oriList.at(pos) != rawRPARENT)
		{
			findit = false;
			return begin;
		}
		cout << "使用带返回值的函数" << endl;
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
		funcRecord newF = funcRecord(strlist.at(begin+1), VOID);
		int pos = begin + 3;
		pos = findParamList(oriList, pos, newF);
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			myexception finalex = myexception(ex.etype, lnolist.at(pos - 1), "");
			finalex.pntMsg();
		}
		runStack newenv();
		if (findit == false || oriList.at(pos) != rawRPARENT || oriList.at(pos + 1) != rawLBRACE) {
			findit = false;
			return begin;
		}
		pos += 2;
		pos = findComplexSentence(oriList, pos);
		if (findit == false || oriList.at(pos) != rawRBRACE) {
			findit = false;
			return begin;
		}
		cout << "void函数定义" << endl;
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
		while (pos1 != pos2) { //这就需要保证如果没找到一定返回begin！
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
		int pos = begin + 5;
		pos = findComplexSentence(oriList, pos);
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
		cout << "程序" << endl;
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
