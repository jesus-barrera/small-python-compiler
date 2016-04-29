#ifndef _SYNTAX_TREE_H_INCLUDED_

#define _SYNTAX_TREE_H_INCLUDED_

#include <iostream>
#include <string>

#include "XMLGenerator.h"

using namespace std;

class Node {
protected:
	static void error(string msg);
	static void displayList(string wrapper_tag, Node*  node);

public:
	static XMLGenerator xml;

	string symbol;
	Node *next;

	virtual void display();
};

class Expression: public Node {
public:
	Expression();
};

class Identifier: public Expression {
public:
	Identifier(string symbol);

	void display();
};

class Integer: public Expression {
public:
	Integer(string symbol);

	void display();
};

class Float: public Expression {
public:
	Float(string symbol);

	void display();
};

class BinaryExpr: public Expression {
public: 
	string op;
	Expression *left;
	Expression *right;
	string tag;

	BinaryExpr(string op, Expression *left, Expression* right, string tag = "EXPRESION");
	~BinaryExpr();
	
	virtual void display();
};

class EqualityExpr: public BinaryExpr {
public:
	EqualityExpr(string op, Expression *left, Expression* right);
};

class RelationalExpr: public BinaryExpr {
public:
	RelationalExpr(string op, Expression *left, Expression* right);
};

class AdditiveExpr: public BinaryExpr {
public:
	AdditiveExpr(string op, Expression *left, Expression* right);
};

class MultiplicativeExpr: public BinaryExpr {
public:
	MultiplicativeExpr(string op, Expression *left, Expression* right);
};

class UnaryExpr: public Expression {
public:
	string op;
	Expression *expr;

	UnaryExpr(string op, Expression *expr);
	~UnaryExpr();

	void display();
};

class IfStatement: public Node {
public:
	Expression *expr;
	Node *statement;
	Node *elseStatement;

	IfStatement(Expression *expr, Node *statement, Node *elseStatement);
	~IfStatement();

	void display();
};

class WhileStatement: public Node {
public:
	Expression *expr;
	Node *statement;

	WhileStatement(Expression *expr, Node *statement);
	~WhileStatement();

	void display();
};

class PrintStatement: public Node {
	Expression *expr;

	PrintStatement(Expression *expr);
	~PrintStatement();

	void display();
};

class Assignment: public Node {
public:
	Identifier *id;
	Expression *expr;

	Assignment(Identifier *id, Expression *expr);
	~Assignment();

	void display();
};

#endif