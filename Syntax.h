#ifndef _SYNTAX_H_
#define _SYNTAX_H_

#include <iostream>
#include "Lexical.h"
#include "syntax_tree.h"

using namespace std;

class Syntax {
private:
	TokenStream tkn_stream;
	Token lookahead;

public:
	Node *analyze(TokenStream *tkn_stream);

	Node *statementList();
	Node *statement();
	IfStatement *ifStatement();
	WhileStatement *whileStatement();
	PrintStatement *printStatement();
	Assignment *assignmentStatement();
	Node *compoundStatement();
	Expression *expression();
	Expression *relationalExpr();
	Expression *additiveExpr();
	Expression *multiplicativeExpr();
	Expression *unaryExpr();
	Expression *primaryExpr();

	void nextToken();
	void match(int tkn_type);
	void match(string tkn_symbol);

	void error(string expected);
};

#endif // _SYNTAX_H_