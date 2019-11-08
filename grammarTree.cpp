

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
void* globalAddr;

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
			std::cout<<"uint"<<endl;
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
		std::cout<<"int "<<endl;
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
			myrunStack.pop();
			record newone(strlist.at(pos), CONST, tp);
			try {
				nowenv.writeStack(newone);
			}
			catch (myexception ex) {
				ex.lineno = lnolist.at(begin + 1);
				ex.msg = "变量名重定义";
				ex.pntMsg();
			}
			pos += 2;
			int posbuf = findInt(oriList, pos);
			if (!findit) {
				myexception ex('o', lnolist.at(pos), "常量定义后应该为整数");
				ex.pntMsg();
				myrunStack.push(nowenv);
				while (oriList.at(pos) != rawCOMMA && oriList.at(pos) != rawSEMICN &&
					lnolist.at(pos) == lnolist.at(pos-1)) pos++;
				posbuf = pos;
			}
			pos = posbuf;
			while (oriList.at(pos) == rawCOMMA) {
				pos++;
				if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawASSIGN)
				{
					myrunStack.push(nowenv);
					findit = false;
					return begin;
				}
				record newone(strlist.at(pos), CONST, tp);
				try {
					nowenv.writeStack(newone);
				}
				catch (myexception ex) {
					ex.lineno = lnolist.at(begin + 1);
					ex.msg = "变量名重定义";
					ex.pntMsg();
				}
				pos += 2;
				posbuf = findInt(oriList, pos);
				if (!findit) {
					myexception ex('o', lnolist.at(pos), "常量定义后应该为整数");
					ex.pntMsg();
					myrunStack.push(nowenv);
					while (oriList.at(pos) != rawCOMMA && oriList.at(pos) != rawSEMICN &&
						lnolist.at(pos) == lnolist.at(pos - 1)) pos++;
					posbuf = pos;
				}
				pos = posbuf;
			}
			myrunStack.push(nowenv);
			pos--;//这里pos停留在最后匹配到整数上
			std::cout<<"constDef "<<endl;
			findit = true;
			return pos + 1;
		}
		else if (oriList.at(begin) == rawCHARTK) {
			tp =  datatype::dCHAR;
			pos = begin + 1;
			if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawASSIGN)
			{
				findit = false;
				return begin;
			}
			runStack nowenv = myrunStack.top();
			myrunStack.pop();
			string name = strlist.at(pos);
			record newone(name, CONST, tp);
			try {
				nowenv.writeStack(newone);
			}
			catch (myexception ex) {
				ex.lineno = lnolist.at(begin + 1);
				ex.msg = "变量名重定义";
				ex.pntMsg();
			}
			pos = pos + 2;
			if (oriList.at(pos) != rawCHARCON) {
				myexception ex('o', lnolist.at(pos), "此处必须是字符常量");
				ex.pntMsg();
				while (oriList.at(pos) != rawCOMMA && oriList.at(pos) != rawSEMICN &&
					lnolist.at(pos) == lnolist.at(pos - 1)) pos++;
				pos--;
			}
			pos++;
			while (oriList.at(pos) == rawCOMMA) {
				pos++;
				if (oriList.at(pos) != rawIDENFR || oriList.at(pos + 1) != rawASSIGN)
				{
					myrunStack.push(nowenv);
					findit = false;
					return begin;
				}
				name = strlist.at(pos);
				pos = pos + 2;
				if (oriList.at(pos) != rawCHARCON) {
					myexception ex('o', lnolist.at(pos), "此处必须是字符常量");
					ex.pntMsg();
					while (oriList.at(pos) != rawCOMMA && oriList.at(pos) != rawSEMICN &&
						lnolist.at(pos) == lnolist.at(pos - 1)) pos++;
					pos--;
				}
				pos++;
				record newone(name, CONST, tp);
				try {
					nowenv.writeStack(newone);
				}
				catch (myexception ex) {
					ex.lineno = lnolist.at(begin + 1);
					ex.msg = "变量名重定义";
					ex.pntMsg();
				}
			}
			myrunStack.push(nowenv);
			pos--;//这里pos停留在最后匹配到整数上
			std::cout<<"constDef "<<endl;
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
		if (!findit) return begin;
		if (oriList.at(pos) != rawSEMICN)
		{
			findit = true;
			myexception ex('k', lnolist.at(pos), "应为分号");
			ex.pntMsg();
			pos--;
		}
		pos++;
		int posbuf = findConstTk(oriList, pos);
		while (findit) {
			pos = posbuf;
			pos = findConstDef(oriList, pos);
			if (oriList.at(pos) != rawSEMICN)
			{
				myexception ex('k', lnolist.at(pos), "应为分号");
				ex.pntMsg();
				pos--;
			}
			pos++;
			posbuf = findConstTk(oriList, pos);
		}
		std::cout<<"constDeclare"<<endl;
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
		if (oriList.at(pos) != rawIDENFR || oriList.at(pos+1) == rawLPARENT) {
			findit = false;
			return begin;
		}
		string name = strlist.at(pos);
		pos++;
		myrunStack.pop();
		if (oriList.at(pos) == rawLBRACK) {
			pos++;
			int posbuf = findUint(oriList, pos);
			int len = -1;
			if (!findit) {
				myexception ex('i', lnolist.at(pos), "数组定义中下标必须是整数哦");
				ex.pntMsg();
				pos = posbuf;
				while (oriList.at(pos) != rawRBRACK &&
					oriList.at(pos) != rawCOMMA && 
					oriList.at(pos) != rawSEMICN &&
					lnolist.at(pos) == lnolist.at(pos - 1)) {
					pos++;
				}
			}
			else {
				len = atoi(strlist.at(pos).c_str());
			}
			pos = posbuf;
			if (oriList.at(pos) != rawRBRACK) {
				myexception ex('m', lnolist.at(pos), "应为]");
				ex.pntMsg();
				pos--;
			}
			arrayTem temp(len, 1, tp, globalAddr);
			record newone(name, VAR, dARRAY);
			newone.addr = temp;
			try {
				nowenv.writeStack(newone);
			}
			catch (myexception ex) {
				ex.lineno = lnolist.at(begin + 1);
				ex.msg = "变量名重定义";
				ex.pntMsg();
			}
			pos++;
		}
		else {
			record newone(name, VAR, tp);
			try {
				nowenv.writeStack(newone);
			}
			catch (myexception ex) {
				ex.lineno = lnolist.at(begin + 1);
				ex.msg = "变量名重定义";
				ex.pntMsg();
			}
		}
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			if (oriList.at(pos) != rawIDENFR) {
				myrunStack.push(nowenv);
				findit = false;
				return begin;
			}
			name = strlist.at(pos);
			pos++;
			if (oriList.at(pos) == rawLBRACK) {
				pos++;
				int posbuf = findUint(oriList, pos);
				int len = -1;
				if (!findit) {
					myexception ex('i', lnolist.at(pos), "数组定义中下标必须是整数哦");
					ex.pntMsg();
					pos = posbuf;
					while (oriList.at(pos) != rawRBRACK &&
						oriList.at(pos) != rawCOMMA &&
						oriList.at(pos) != rawSEMICN &&
						lnolist.at(pos) == lnolist.at(pos - 1)) {
						pos++;
					}
				}
				else {
					len = atoi(strlist.at(pos).c_str());
				}
				pos = posbuf;
				if (oriList.at(pos) != rawRBRACK) {
					myexception ex('m', lnolist.at(pos), "应为]");
					ex.pntMsg();
					pos--;
				}
				arrayTem temp(len, 1, tp, globalAddr);
				record newone(name, VAR, dARRAY);
				newone.addr = temp;
				try {
					nowenv.writeStack(newone);
				}
				catch (myexception ex) {
					ex.lineno = lnolist.at(begin + 1);
					ex.msg = "变量名重定义";
					ex.pntMsg();
				}
				pos++;
			}
			else {
				record newone(name, VAR, tp);
				try {
					nowenv.writeStack(newone);
				}
				catch (myexception ex) {
					ex.lineno = lnolist.at(begin + 1);
					ex.msg = "变量名重定义";
					ex.pntMsg();
				}
			}
		}
		myrunStack.push(nowenv);
		std::cout<<"varDef"<<endl;
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
		std::cout<<"复合语句 "<<endl;
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
		if (!findit) return begin;
		if (oriList.at(pos) != rawSEMICN)
		{
			myexception ex('k', lnolist.at(pos), "应为分号");
			ex.pntMsg();
			pos--;
		}
		pos++;
		posbuf = pos;
		pos = findVarDef(oriList, posbuf);
		while (findit) {
			if (oriList.at(pos) != rawSEMICN)
			{
				myexception ex('k', lnolist.at(pos), "应为分号");
				ex.pntMsg();
				pos--;
			}
			pos++;
			posbuf = pos;
			pos = findVarDef(oriList, posbuf);
		}
		std::cout<<"变量声明 "<<endl;
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
		std::cout<<"声明头部 "<<endl;
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
		if (oriList.at(pos) != rawINTTK && oriList.at(pos) != rawCHARTK) {
			newF.paraList = paraL;
			std::cout<<"参数列表"<<endl;
			findit = true;
			return pos ;
		}
		datatype buf = (oriList.at(pos) == rawINTTK) ?  datatype::dINT :  datatype::dCHAR;
		paraL.push_back(buf);
		pos++;
		if (oriList.at(pos) != rawIDENFR) {
			//std::cout << "invalid gramma in paralist, don't know how to deal" << endl;
			findit = false;
			return begin;
		}
		record newrec(strlist.at(pos), VAR, buf);
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
			record newrec(strlist.at(pos), VAR, buf);
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
		std::cout<<"参数表 "<<endl;
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
		newenv.father = &newF;
		pos = findParamList(oriList, pos, newF, newenv); //就算是空的，也必须表示出来吗？？？？？大概是的
		if (!findit) return begin;
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos - 1);
			ex.pntMsg();
		} 
		if (oriList.at(pos) != rawRPARENT) {
			myexception ex('l', lnolist.at(pos), "参数表的右括号出现吧！");
			ex.pntMsg();
			pos--;//为了和原来的代码架构适应
		}
		if (oriList.at(pos + 1) != rawLBRACE) {
			findit = false;
			return begin;
		}
		pos += 2;

		myrunStack.push(newenv);
		pos = findComplexSentence(oriList, pos);
		myrunStack.pop();//不管是不是找到了其语句都要弹出
		try {
			if (newF.rtN == 0) {
				throw myexception('h', lnolist.at(pos - 1), "该函数缺少return 语句");
			}
		}
		catch (myexception ex) {
			ex.pntMsg();
		}
		if (!findit || oriList.at(pos) != rawRBRACE) {
			findit = false;
			return begin;
		}
		
		std::cout<<"有返回值函数定义 "<<endl;
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
		std::cout<<"语句列 "<<endl;
		findit = true;//可以为0次
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
			std::cout<<"因子 "<<endl;
			findit = true;
			return begin + 1;
		}
		else if (oriList.at(begin) == rawIDENFR) {
			int posbuf = findUseFunc(oriList, begin, tp);
			if (!findit) {
				runStack nowenv = myrunStack.top();
				record rebuf;
				try {
					rebuf = nowenv.findRecord(strlist.at(begin));
					tp = rebuf.typeD;
				}
				catch (myexception ex) {
					tp = dINT;
					ex.lineno = lnolist.at(begin);
					ex.msg = "未定义的变量";
					ex.pntMsg();
				}
				
				if (tp == dARRAY) {
					tp = dINT; // 计算返回也没有错,当地址来看
					//throw myexception('c', lnolist.at(begin + 1), "该变量是一个数组，返回dARRAY可能导致出错");
				}
				if (oriList.at(begin + 1) == rawLBRACK) {
					if (rebuf.typeD != dARRAY) {
						/*myexception ex('c', lnolist.at(begin), "虽然名字被定义了，但它不是数组");
						ex.pntMsg();*/
						tp = dINT;
					}
					else {
						tp = rebuf.addr.tp;
					}
					datatype tp2;
					pos = findExpress(oriList, pos + 2, tp2);
					if (!findit) return begin;
					if (tp2 != datatype::dINT) {
						myexception ex('i', lnolist.at(pos - 1), "数组下标不是整形表达式");
						ex.pntMsg();
					}
					if (oriList.at(pos) != rawRBRACK)
					{
						myexception ex('m', lnolist.at(pos), "应为]");
						ex.pntMsg();
						pos--;
					}
					std::cout << "因子 " << endl;
					findit = true;
					return pos + 1;
				}
				else {
					std::cout << "因子" << endl;
					findit = true;
					return pos + 1;
				}
			}
			else {
				if (tp == datatype::dVOID) {
					//error! 调用了void函数作为因子
				}
				std::cout<<"因子 "<<endl;
				findit = true;
				return posbuf ;
			}
		}
		else if (oriList.at(begin) == rawLPARENT) {
			pos = begin + 1;
			pos = findExpress(oriList, pos, tp);
			if (!findit) return begin;

			if (oriList.at(pos) != rawRPARENT)
			{
				myexception ex('l', lnolist.at(pos), "因子缺少）");
				ex.pntMsg();
				pos--;
			}
			std::cout<<"因子 "<<endl;
			findit = true;
			return pos + 1;
		}
		else {
			int posbuf = findInt(oriList, pos);
			if (!findit) {
				return begin;
			}
			else {
				std::cout<<"因子 "<<endl;
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
		if (!findit) {
			return begin;
		}
		while (oriList.at(pos) == rawMULT || oriList.at(pos) == rawDIV) {
			pos++;
			pos = findFactor(oriList, pos, tpbuf);
			if (!findit) return begin;
			tp = (tpbuf ==  datatype::dINT) ?  datatype::dINT : tp;
		}
		std::cout << "item" << endl;
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
				return begin; //默认不存在用+-结尾的表达式
			tp = (tpbuf ==  datatype::dINT) ?  datatype::dINT : tp;
		}
		std::cout<<"项 "<<endl;
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
		pos = findExpress(oriList, pos, tp);
		if (!findit) return begin;
		if (tp != datatype::dINT) {
			myexception ex('f', lnolist.at(pos - 1), "条件中的表达式必须是整形的");
			ex.pntMsg();
		}
		if (oriList.at(pos) >= rawLSS && oriList.at(pos) <= rawNEQ) {
			pos++;
			pos = findExpress(oriList, pos, tp);
			if (!findit) return begin;
			if (tp != datatype::dINT) {
				myexception ex('f', lnolist.at(pos - 1), "条件中的表达式必须是整形的");
				ex.pntMsg();
			}
		}
		std::cout<<"条件 "<<endl;
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
			std::cout<<"步长 "<<endl;
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
		if (!findit) return begin;
		if (oriList.at(pos) != rawRPARENT)
		{
			myexception ex('l', lnolist.at(pos), "if 条件的)");
			ex.pntMsg();
			pos--;
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
		std::cout<<"条件语句"<<endl;
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
		if (!findit) return begin;
		if (oriList.at(pos) != rawRPARENT)
		{
			myexception ex('l', lnolist.at(pos), "while 条件的)");
			ex.pntMsg();
			pos--;
		}
		pos++;
		pos = findSentence(oriList, pos);
		if (!findit) return begin;
		std::cout<<"循环语句 "<<endl;
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
			myexception ex('n', lnolist.at(pos), "do-while缺少while");////////////////////////////????how to jump??
			ex.pntMsg();
			pos--;
		}
		if (oriList.at(pos + 1) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		pos += 2;
		pos = findCondition(oriList, pos);
		if (!findit) return begin;
		if ( oriList.at(pos) != rawRPARENT)
		{
			myexception ex('l', lnolist.at(pos), "do 条件的)");
			ex.pntMsg();
			pos--;
		}
		std::cout<<"循环语句"<<endl;
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
			myexception ex('k', lnolist.at(pos), "应为分号");
			ex.pntMsg();
			pos--;
		}
		pos++;
		pos = findCondition(oriList, pos);
		if (!findit) return false;
		else if (oriList.at(pos) != rawSEMICN) {
			myexception ex('k', lnolist.at(pos), "应为分号");
			ex.pntMsg();
			pos--;
		}
		if (oriList.at(pos + 1) != rawIDENFR
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
		if (!findit) return begin;
		if (oriList.at(pos) != rawRPARENT)
		{
			myexception ex('l', lnolist.at(pos), "for 条件的)");
			ex.pntMsg();
			pos--;
		}
		pos++;
		pos = findSentence(oriList, pos);
		if (!findit)
			return begin;
		std::cout<<"循环语句 "<<endl;
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
			myexception ex('l', lnolist.at(pos), "scanf 的)");
			ex.pntMsg();
			pos--;
		}
		std::cout << "readS" << endl;
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
			std::cout << "strCon" << endl;
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
				myexception ex('l', lnolist.at(pos), "printf 的)");
				ex.pntMsg();
				pos--;
			}
			std::cout << "writeS" << endl;
			findit = true;
			return pos + 1;
		}
		else {
			pos = findExpress(oriList, pos, tp);
			if (oriList.at(pos) != rawRPARENT)
			{
				myexception ex('l', lnolist.at(pos), "printf的)");
				ex.pntMsg();
				pos--;
			}
			std::cout<<"writeS "<<endl;
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
		runStack nowenv = myrunStack.top();
		funcRecord* father = nowenv.father;
		father->rtN++;
		myrunStack.pop();
		myrunStack.push(nowenv);
		int pos = begin + 1;
		if (oriList.at(pos) == rawLPARENT) {
			pos = findExpress(oriList, pos + 1, tp);
			if (!findit)
				return begin; //如果没有表达式就没有括号

			try {
				if (father->returnT == datatype::dVOID) {
					throw myexception('g', lnolist.at(pos - 1), "VOID函数出现了非void返回语句");
				}
				else if (father->returnT != tp) {
					throw myexception('h', lnolist.at(pos - 1), "return函数返回类型不匹配");
				}
			}
			catch (myexception ex) {
				ex.pntMsg();
			}
			if (oriList.at(pos) != rawRPARENT) {
				myexception ex('l', lnolist.at(pos), "return 的)");
				ex.pntMsg();
				pos--;
			}
			
			std::cout<<"返回语句"<<endl;
			findit = true;
			return pos + 1;
		}
		else { //有没有分号都当做有
			tp = dVOID;
			try {
				runStack nowenv = myrunStack.top();
				funcRecord* father = nowenv.father;
				father->rtN++;
				myrunStack.pop();
				myrunStack.push(nowenv);
				if (father->returnT != datatype::dVOID) {
					throw myexception('h', lnolist.at(pos), "返回值函数返回void");
				}
			}
			catch (myexception ex) {
				ex.pntMsg();
			}
			std::cout << "返回语句 " << endl;
			findit = true;
			return pos;
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
				std::cout << "语句" << endl;
			}
			return pos ;
		}
		else if (oriList.at(begin) == rawWHILETK) {
			pos = findWhileS(oriList, pos);
			if (findit)
			{
				std::cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawDOTK) {
			pos = findDoS(oriList, pos);
			if (findit)
			{
				std::cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawFORTK) {
			pos = findForS(oriList, pos);
			if (findit)
			{
				std::cout << "语句" << endl;
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
				std::cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawSCANFTK) {
			pos = findScanfS(oriList, pos);
			if (oriList.at(pos) != rawSEMICN)
			{
				myexception ex('k', lnolist.at(pos), "应为分号");
				ex.pntMsg();
				pos--;
			}
			pos++;
			if (findit)
			{
				std::cout << "语句" << endl;
			}
			return pos ;
		}
		else if (oriList.at(begin) == rawPRINTFTK) {
			pos = findPrintfS(oriList, pos);
			if (oriList.at(pos) != rawSEMICN)
			{
				myexception ex('k', lnolist.at(pos), "应为分号");
				ex.pntMsg();
				pos--;
			}

			pos++;
			if (findit)
			{
				std::cout << "语句" << endl;
			}
			return pos;
		}
		else if (oriList.at(begin) == rawSEMICN) {
			findit = true;
			if (findit)
			{
				std::cout << "语句" << endl;
			}
			return pos + 1;
		}
		else if (oriList.at(begin) == rawRETURNTK) {
			pos = findReturnS(oriList, pos);
			if (!findit)return false;
			if (oriList.at(pos) != rawSEMICN)
			{
				myexception ex('k', lnolist.at(pos), "应为分号");
				ex.pntMsg();
				pos--;
			}
			pos++;
			if (findit)
			{
				std::cout << "语句" << endl;
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
				myexception ex('k', lnolist.at(pos), "应为分号");
				ex.pntMsg();
				pos--;
			}
			pos++;
			if (findit)
			{
				std::cout << "语句" << endl;
			}
			return pos;

		}
		else {//如果真的啥都没有那就当做少了一个分号吧！！
			findit = false;
			return begin;
		}
		
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
				myexception  ex('j', lnolist.at(pos), "尝试为常量" + strlist.at(pos) + "赋值");
				ex.pntMsg();
			}
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos);
			ex.msg = "未定义的变量名";
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
				myexception ex('i', lnolist.at(pos - 1), "赋值语句中数组下标不是整数");
				ex.pntMsg();
			}
			if (oriList.at(pos) != rawRBRACK)
			{
				myexception ex('m', lnolist.at(pos), "这个地方应该是]");
				ex.pntMsg();
				pos--;
			}
			pos++;
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
		std::cout<<"赋值语句 "<<endl;
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
		if (!findit) {
			findit = true;
			return begin;
		}
		paraL.push_back(buft);
		//无论是否找到experss都是对的
		while (oriList.at(pos) == rawCOMMA) {
			pos++;
			pos = findExpress(oriList, pos, buft);
			if (!findit) return begin;
			paraL.push_back(buft);
		}
		std::cout<<"值参数列表 "<<endl;
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
			ex.msg = "没有这样的函数";
			ex.pntMsg();
		}
		if (oriList.at(pos) != rawRPARENT)
		{
			myexception ex('l', lnolist.at(pos), "值参数列表的)");
			ex.pntMsg();
			pos--;
		}
		if (tp == datatype::dVOID) {
			std::cout << "使用不带返回值的函数" << endl;
		}
		else {
			std::cout << "使用返回值的函数" << endl;
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
		newenv.father = &newF;
		pos = findParamList(oriList, pos, newF, newenv);
		if (!findit) return begin;
		if (oriList.at(pos) != rawRPARENT) {
			myexception ex('l', lnolist.at(pos), "参数列表的)");
			ex.pntMsg();
			pos--;
		}
		if (oriList.at(pos + 1) != rawLBRACE) {
			findit = false;
			return begin;
		}
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(pos - 1);
			ex.msg = "void 函数存在重名同参函数";
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
		std::cout << "void函数定义" << endl;
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
			|| oriList.at(begin + 2) != rawLPARENT)
		{
			findit = false;
			return begin;
		}
		int pos = begin + 3;
		if (oriList.at(pos) != rawRPARENT) {
			myexception ex('l', lnolist.at(pos), "main的)");
			ex.pntMsg();
			pos--;
		}
		pos++;
		if (oriList.at(pos) != rawLBRACE) {
			findit = false;
			return begin;
		}
		pos++;
		vector<datatype> paraLnull;
		funcRecord newF("main", datatype::dVOID,paraLnull); //要不要把MAIN也放进函数列表呢？？？先不放
		runStack preenv = myrunStack.top();
		runStack mainenv(&preenv);
		mainenv.father = &newF;
		myrunStack.push(mainenv);
		try {
			put_check(newF);
		}
		catch (myexception ex) {
			ex.lineno = lnolist.at(begin + 1);
			ex.pntMsg();
		}
		pos = findComplexSentence(oriList, pos);
		myrunStack.pop();
		if (findit == false) {
			return begin;
		}
		if (oriList.at(pos) != rawRBRACE) {
			findit = false;
			return begin;
		}
		std::cout << "mainFUN" << endl;
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
		std::cout << "error " << endl;
	}
	else {
		//finalList.insert(finalList.begin() + pos, PROGRAM);
		std::cout << "程序" << endl;
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
