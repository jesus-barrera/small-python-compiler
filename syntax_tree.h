#ifndef _SYNTAX_TREE_H_INCLUDED_

#define _SYNTAX_TREE_H_INCLUDED_

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include "XMLGenerator.h"
#include "SymbolsTable.h"

using namespace std;

class Node {
protected:
	static void error(string msg);
	static map<string, int> labels; 
	
	static string generateUniqueLabel(string label);
	static string conditionalJump(string op);
public:
	static XMLGenerator xml;
	static SymbolsTable symtab;

	int type;
	string symbol;
	Node *next;

	~Node();
	virtual void display();
	virtual void checkSemantic();
	virtual void generateCode(ostream &output);

	static void generateCode(Node *tree, ostream &output);
	static void displayList(string wrapper_tag, Node*  node);
	static int checkSemanticOnList(Node*  node);
	static void generateCodeOnList(Node *node, ostream &output);
};

class Expression: public Node {
public:
	Expression();
};

class Identifier: public Expression {
public:
	Identifier(string symbol);

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

class Integer: public Expression {
public:
	Integer(string symbol);

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

class Float: public Expression {
public:
	Float(string symbol);

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

class BinaryExpr: public Expression {
public: 
	string op;
	Expression *left;
	Expression *right;
	string tag;

	BinaryExpr(string op, Expression *left, Expression* right, string tag = "EXPRESION");
	~BinaryExpr();
	
	void display();
	void checkSemantic();
};

class EqualityExpr: public BinaryExpr {
public:
	EqualityExpr(string op, Expression *left, Expression* right);
	
	void generateCode(ostream &output);
};

class RelationalExpr: public BinaryExpr {
public:
	RelationalExpr(string op, Expression *left, Expression* right);
	
	void generateCode(ostream &output);
};

class AdditiveExpr: public BinaryExpr {
public:
	AdditiveExpr(string op, Expression *left, Expression* right);

	void generateCode(ostream &output);
};

class MultiplicativeExpr: public BinaryExpr {
public:
	MultiplicativeExpr(string op, Expression *left, Expression* right);

	void generateCode(ostream &output);
};

class UnaryExpr: public Expression {
public:
	string op;
	Expression *expr;

	UnaryExpr(string op, Expression *expr);
	~UnaryExpr();

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

class IfStatement: public Node {
public:
	Expression *expr;
	Node *statement;
	Node *elseStatement;

	IfStatement(Expression *expr, Node *statement, Node *elseStatement);
	~IfStatement();

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

class WhileStatement: public Node {
public:
	Expression *expr;
	Node *statement;

	WhileStatement(Expression *expr, Node *statement);
	~WhileStatement();

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

class PrintStatement: public Node {
public:
	Expression *expr;

	PrintStatement(Expression *expr);
	~PrintStatement();

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

class Assignment: public Node {
public:
	Identifier *id;
	Expression *expr;

	Assignment(Identifier *id, Expression *expr);
	~Assignment();

	void display();
	void checkSemantic();
	void generateCode(ostream &output);
};

#endif