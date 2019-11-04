#pragma once
#include <vector>
#include <string>
using namespace std;

enum types {
	IDENFR,
	INTCON,
	CHARCON,
	STRCON,
	CONSTTK,
	INTTK,
	CHARTK,
	VOIDTK,
	MAINTK,
	IFTK,
	ELSETK,
	DOTK,
	WHILETK,
	FORTK,
	SCANFTK,
	PRINTFTK,
	RETURNTK,
	PLUS,
	MINU,
	MULT,
	DIV,
	LSS,
	LEQ,
	GRE,
	GEQ,
	EQL,
	NEQ,
	ASSIGN,
	SEMICN,
	COMMA,
	LPARENT,
	RPARENT,
	LBRACK,
	RBRACK,
	LBRACE,
	RBRACE
};

static string type1string[] = {
	"IDENFR",
	"INTCON",
	"CHARCON",
	"STRCON",
	"CONSTTK",
	"INTTK",
	"CHARTK",
	"VOIDTK",
	"MAINTK",
	"IFTK",
	"ELSETK",
	"DOTK",
	"WHILETK",
	"FORTK",
	"SCANFTK",
	"PRINTFTK",
	"RETURNTK",
	"PLUS",
	"MINU",
	"MULT",
	"DIV",
	"LSS",
	"LEQ",
	"GRE",
	"GEQ",
	"EQL",
	"NEQ",
	"ASSIGN",
	"SEMICN",
	"COMMA",
	"LPARENT",
	"RPARENT",
	"LBRACK",
	"RBRACK",
	"LBRACE",
	"RBRACE"
};

class wordAna {
public:
	vector<types> wordList;
	vector<string> strlist;
	vector<int> lnolist;
	void initWordList();
private:

	types getStringType(string& s) {
		if (s.compare("const") == 0) {
			return CONSTTK;
		}
		else if (s.compare("int") == 0) {
			return INTTK;
		}
		else if (s.compare("char") == 0) {
			return CHARTK;
		}
		else if (s.compare("void") == 0) {
			return VOIDTK;
		}
		else if (s.compare("main") == 0) {
			return MAINTK;
		}
		else if (s.compare("if") == 0) {
			return IFTK;
		}
		else if (s.compare("else") == 0) {
			return ELSETK;
		}
		else if (s.compare("do") == 0) {
			return DOTK;
		}
		else if (s.compare("while") == 0) {
			return WHILETK;
		}
		else if (s.compare("for") == 0) {
			return FORTK;
		}
		else if (s.compare("scanf") == 0) {
			return SCANFTK;
		}
		else if (s.compare("printf") == 0) {
			return PRINTFTK;
		}
		else if (s.compare("return") == 0) {
			return RETURNTK;
		}
		else {
			return IDENFR;
		}
	}

};
