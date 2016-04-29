#include <sstream>
#include <cstdlib>
#include "syntax_tree.h"

XMLGenerator Node::xml;

/* Node methods */
void Node::display() {}

void Node::displayList(string wrapper_tag, Node *node) {
	xml.openTag(wrapper_tag);
	while (node) {
		node->display();
		node = node->next;
	}
	xml.closeTag();
}

/* Expression methods */
Expression::Expression() {}

/* Identifier methods */
Identifier::Identifier(string symbol) {
	this->symbol = symbol;
}

void Identifier::display() {
	xml.oneLineTag("ID", this->symbol);
}

/* Integer methods */
Integer::Integer(string symbol) {
	this->symbol = symbol;
}

void Integer::display() {
	xml.oneLineTag("ENTERO", this->symbol);
}

/* Float methods */
Float::Float(string symbol) {
	this->symbol = symbol;
}

void Float::display() {
	xml.oneLineTag("REAL", this->symbol);
}

/* Binary expression methods */
BinaryExpr::BinaryExpr(string op, Expression *left, Expression* right, string tag) {
	this->op = op;
	this->left = left;
	this->right = right;
	this->tag = tag;
}

BinaryExpr::~BinaryExpr() {
	delete(this->left);
	delete(this->right);
}
	
void BinaryExpr::display() {
	xml.openTag(this->tag, this->op);
	this->left->display();
	this->right->display();
	xml.closeTag();
}

/* Equality expression methods */
EqualityExpr::EqualityExpr(string op, Expression *left, Expression* right)
		: BinaryExpr(op, left, right) {}

/* Relational expression methods */
RelationalExpr::RelationalExpr(string op, Expression *left, Expression* right)
		: BinaryExpr(op, left, right) {}

/* Additive expression methods */
AdditiveExpr::AdditiveExpr(string op, Expression *left, Expression* right) 
		: BinaryExpr(op, left, right, "SUMA") {}

/* Multiplicative expression methods */
MultiplicativeExpr::MultiplicativeExpr(string op, Expression *left, Expression* right) 
		: BinaryExpr(op, left, right, "MULT") {}

/* Unary expression methods */
UnaryExpr::UnaryExpr(string op, Expression *expr) {
	this->op = op;
	this->expr = expr;
}

UnaryExpr::~UnaryExpr() {
	delete(expr);
}

void UnaryExpr::display() {
	xml.openTag("SIGNO", this->op);
	this->expr->display();
	xml.closeTag();
}

/* If statement methods */
IfStatement::IfStatement(Expression *expr, Node *statement, Node *elseStatement) {
	this->expr = expr;
	this->statement = statement;
	this->elseStatement = elseStatement;
}

IfStatement::~IfStatement() {
	delete(this->expr);
	delete(this->statement);
	delete(this->elseStatement);
}

void IfStatement::display() {
	xml.openTag("SI");
	this->expr->display();
	displayList("BLOQUE", this->statement);
	if (this->elseStatement) {
		displayList("OTRO", this->elseStatement);
	}
	xml.closeTag();
}

/* While statement methods */
WhileStatement::WhileStatement(Expression *expr, Node *statement) {
	this->expr = expr;
	this->statement = statement;
}

WhileStatement::~WhileStatement() {
	delete(this->expr);
	delete(this->statement);
}

void WhileStatement::display() {
	xml.openTag("MIENTRAS");
	this->expr->display();
	displayList("BLOQUE", this->statement);
	xml.closeTag();
}

/* Print statement methods */
PrintStatement::PrintStatement(Expression *expr) {
	this->expr = expr;
}

PrintStatement::~PrintStatement() {
	delete(expr);
}

void PrintStatement::display() {
	xml.openTag("IMPRIME");
	xml.openTag("EXPRESION");
	this->expr->display();
	xml.closeTag();
	xml.closeTag();
}

/* Assignment statement methods */
Assignment::Assignment(Identifier *id, Expression *expr) {
	this->id = id;
	this->expr = expr;
}

Assignment::~Assignment() {
	delete(this->id);
	delete(this->expr);
}

void Assignment::display() {
	xml.openTag("ASIGNACION");
	this->id->display();
	this->expr->display();
	xml.closeTag();
}