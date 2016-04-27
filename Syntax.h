#ifndef _SYNTAX_H_
#define _SYNTAX_H_

#include <iostream>
#include "Lexical.h"

using namespace std;

class Syntax {
private:
	TokenStream tkn_stream;
	Token lookahead;

public:
	void analyze(TokenStream *tkn_stream);

	void statementList();
	void statement();
	void ifStatement();
	void whileStatement();
	void printStatement();
	void assignmentStatement();
	void compoundStatement();
	void expr();
	void relationalExpr();
	void additiveExpr();
	void multiplicativeExpr();
	void unaryExpr();
	void primaryExpr();

	void nextToken();
	void match(int tkn_type);
	void match(string tkn_symbol);

	void error(string expected);
};

#endif // _SYNTAX_H_