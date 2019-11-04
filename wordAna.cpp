#include<cstdio>
#include<iostream>
#include<string>
#include<fstream>
#include<cctype>
#include "wordAna.h"
#include "derror.h"
using namespace std;

void wordAna::initWordList() {
	//freopen("testfile.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	ifstream infile;
	infile.open("testfile.txt",ios::in);
	char c;
	string buf;
	types mytype;
	int lineno = 1;
	while (infile.get(c)) {
		buf = "";

		if (c == '\n') {
			lineno++;
			continue;
		}
		if (isalpha(c) || c == '_') {
			mytype = IDENFR;
			buf += c;
			infile.get(c);
			while (isalnum(c) || c == '_') {
				buf += c;
				infile.get(c);
			}
			infile.unget();
			mytype = getStringType(buf);
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (isdigit(c)) {
			mytype = INTCON;
			buf += c;
			infile.get(c);
			while (isdigit(c)) {
				buf += c;
				infile.get(c);
			}
			infile.unget();
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '\'') {
			mytype = CHARCON;
			infile.get(c);
			buf += c;
			infile.get(c);
			try {
				if (c != '\'') {
					while (c != '\'' && c != '\n') {
						infile.get(c);
						buf += c;
					}
					throw myexception('a', lineno, "");
				}
			}
			catch (myexception ex) {
				ex.pntMsg();
			}
			infile.unget();
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			infile.get(c);
			continue;
		}

		if (c == '\"') {
			mytype = STRCON;
			infile.get(c);
			while (c != '\"') {
				buf += c;
				infile.get(c);
			}
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '+') {
			mytype = PLUS;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '-') {
			mytype = MINU;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '*') {
			mytype = MULT;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '/') {
			mytype = DIV;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == ';') {
			mytype = SEMICN;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == ',') {
			mytype = COMMA;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '(') {
			mytype = LPARENT;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == ')') {
			mytype = RPARENT;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '[') {
			mytype = LBRACK;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == ']') {
			mytype = RBRACK;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '{') {
			mytype = LBRACE;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}
		if (c == '}') {
			mytype = RBRACE;
			buf += c;
			wordList.push_back(mytype);
			strlist.push_back(buf);
			lnolist.push_back(lineno);
			//cout << type1string[mytype] << " " << buf << endl;
			continue;
		}

		if (c == '<') {
			buf += c;
			infile.get(c);
			if (c == '=') {
				buf += c;
				mytype = LEQ;
				wordList.push_back(mytype);
				strlist.push_back(buf);
				lnolist.push_back(lineno);
				//cout << type1string[mytype] << " " << buf << endl;
				continue;
			}
			else {
				infile.unget();
				mytype = LSS;
				wordList.push_back(mytype);
				strlist.push_back(buf);
				lnolist.push_back(lineno);
				//cout << type1string[mytype] << " " << buf << endl;
				continue;
			}
		}
		if (c == '>') {
			buf += c;
			infile.get(c);
			if (c == '=') {
				buf += c;
				mytype = GEQ;
				wordList.push_back(mytype);
				strlist.push_back(buf);
				lnolist.push_back(lineno);
				//cout << type1string[mytype] << " " << buf << endl;
				continue;
			}
			else {
				infile.unget();
				mytype = GRE;
				wordList.push_back(mytype);
				strlist.push_back(buf);
				lnolist.push_back(lineno);
				//cout << type1string[mytype] << " " << buf << endl;
				continue;
			}
		}

		if (c == '=') {
			buf += c;
			infile.get(c);
			if (c == '=') {
				buf += c;
				mytype = EQL;
				wordList.push_back(mytype);
				strlist.push_back(buf);
				lnolist.push_back(lineno);
				//cout << type1string[mytype] << " " << buf << endl;
				continue;
			}
			else {
				infile.unget();
				mytype = ASSIGN;
				wordList.push_back(mytype);
				strlist.push_back(buf);
				lnolist.push_back(lineno);
				//cout << type1string[mytype] << " " << buf << endl;
				continue;
			}
		}

		if (c == '!') {
			buf += c;
			infile.get(c);
			if (c == '=') {
				buf += c;
				mytype = NEQ;
				wordList.push_back(mytype);
				strlist.push_back(buf);
				lnolist.push_back(lineno);
				//cout << type1string[mytype] << " " << buf << endl;
				continue;
			}
		}
	}

	infile.close();
}