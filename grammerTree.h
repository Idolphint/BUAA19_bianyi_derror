#pragma once
#include <vector>
#include "wordAna.h"
using namespace std;

string treestr[] = {
	"rawIDENFR",
	"rawINTCON",
	"rawCHARCON",
	"rawSTRCON",
	"rawCONSTTK",
	"rawINTTK",
	"rawCHARTK",
	"rawVOIDTK",
	"rawMAINTK",
	"rawIFTK",
	"rawELSETK",
	"rawDOTK",
	"rawWHILETK",
	"rawFORTK",
	"rawSCANFTK",
	"rawPRINTFTK",
	"rawRETURNTK",
	"rawPLUS",
	"rawMINU",
	"rawMULT",
	"rawDIV",
	"rawLSS",
	"rawLEQ",
	"rawGRE",
	"rawGEQ",
	"rawEQL",
	"rawNEQ",
	"rawASSIGN",
	"rawSEMICN",
	"rawCOMMA",
	"rawLPARENT",
	"rawRPARENT",
	"rawLBRACK",
	"rawRBRACK",
	"rawLBRACE",
	"rawRBRACE",
	"<字符串>",
	"<程序>",
	"<常量说明>",
	"<常量定义>","<无符号整数>",
//	"<无符号整数>",
	"<整数>",
	"<声明头部>",
	"<变量说明>",
	"<变量定义>",
	"<有返回值函数定义>",
	"<无返回值函数定义>",
	"<复合语句>","<参数表>","<主函数>","<表达式>","<项>",
	//"<参数表>",
	//"<主函数>",
	//"<表达式>",
	//"<项>",
	"<因子>",
	"<语句>",
	"<赋值语句>",
	"<条件语句>",
	"<条件>",
	"<循环语句>",
	"<步长>",
	"<有返回值函数调用语句>",
	"<无返回值函数调用语句>","<值参数表>","<语句列>","<读语句>",
	"<写语句>",
	//"<值参数表>",
	//"<语句列>",
	//"<读语句>",
	//"<写语句>",
	"<返回语句>",
	"NONE"
};

enum treetype {
	rawIDENFR,
	rawINTCON,
	rawCHARCON,
	rawSTRCON,
	rawCONSTTK,
	rawINTTK,
	rawCHARTK,
	rawVOIDTK,
	rawMAINTK,
	rawIFTK,
	rawELSETK,
	rawDOTK,
	rawWHILETK,
	rawFORTK,
	rawSCANFTK,
	rawPRINTFTK,
	rawRETURNTK,
	rawPLUS,
	rawMINU,
	rawMULT,
	rawDIV,
	rawLSS,
	rawLEQ,
	rawGRE,
	rawGEQ,
	rawEQL,
	rawNEQ,
	rawASSIGN,
	rawSEMICN,
	rawCOMMA,
	rawLPARENT,
	rawRPARENT,
	rawLBRACK,
	rawRBRACK,
	rawLBRACE,
	rawRBRACE,
	STRING,
	PROGRAM,
	CONSTDECLARE,
	CONSTDEF,
	UINT,
	INT,
	DECLAREHEAD,
	VARDECLARE,
	VARDEF,
	FUNCRETURNDEF,
	FUNCVOIDDEF,
	COMPLEXSENTENCE,
	PARALIST,
	MAINFUNC,
	EXPRESSION,
	ITEM,
	FACTOR,
	SENTENCE,
	ASSIGNSENTENCE,
	IFSENTENCE,
	CONDITION,
	WHILESENTENCE,
	STEP,
	USEFUNCRETURN,
	USEFUNCVOID,
	VALUEPARALIST,
	SENTENCELIST,
	READSENTENCE,
	WRITESENTENCE,
	RETURNSENTENCE,
	NONE
}; 

int findUint(vector<treetype>& oriList, int begin);
int findInt(vector<treetype>& oriList, int begin);
int findConstTk(vector<treetype>& oriList, int begin);
int findConstDef(vector<treetype>& oriList, int begin);
int findConstDeclare(vector<treetype>& oriList, int begin);
int findVarDef(vector<treetype>& oriList, int begin);
int findComplexSentence(vector<treetype>& oriList, int begin);
int findVarDeclare(vector<treetype>& oriList, int begin);
int findDeclareHead(vector<treetype>& oriList, int begin, funcRecord& newF);
int findParamList(vector<treetype>& oriList, int begin, funcRecord& newF);
int findFuncReturnDef(vector<treetype>& oriList, int begin);
int findSentenceList(vector<treetype>& oriList, int begin);
int findFactor(vector<treetype>& oriList, int begin, datatype& tp);
int findItem(vector<treetype>& oriList, int begin, datatype& tp);
int findExpress(vector<treetype>& oriList, int begin, datatype& tp);
int findCondition(vector<treetype>& oriList, int begin);
int findStep(vector<treetype>& oriList, int begin);
int findIfS(vector<treetype>& oriList, int begin);
int findWhileS(vector<treetype>& oriList, int begin);
int findDoS(vector<treetype>& oriList, int begin);
int findForS(vector<treetype>& oriList, int begin);
int findScanfS(vector<treetype>& oriList, int begin);
int findPrintfS(vector<treetype>& oriList, int begin);
int findReturnS(vector<treetype>& oriList, int begin);
int findSentence(vector<treetype>& oriList, int begin);
int findAssignS(vector<treetype>& oriList, int begin);
int findValueParaList(vector<treetype>& oriList, int begin, vector<datatype>& paraL);
int findUseVoidFunc(vector<treetype>& oriList, int begin);
int findUseReturnFunc(vector<treetype>& oriList, int begin, datatype& tp);
int findFuncVoidDef(vector<treetype>& oriList, int begin);
int findFuncDef(vector<treetype>& oriList, int begin);
int findMainFunc(vector<treetype>& oriList, int begin);

