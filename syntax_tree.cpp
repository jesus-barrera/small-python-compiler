#include <iostream>
#include <sstream>

#include "syntax_tree.h"

using namespace std;

map<string, int> Node::labels;
SymbolsTable Node::symtab;
XMLGenerator Node::xml;

/* Node methods */
void Node::display() {}

Node::~Node() {
	delete(this->next);
}

void Node::error(string msg) {
	cout << "[Error semantico] " << msg << endl;
}

void Node::checkSemantic() {
	this->type = TYPE_ERROR;
}

void Node::generateCode(fstream &output) {
	output << "; codigo" << endl;
}

void Node::generateCode(Node *tree, fstream &output) {
	// Directivas
	output << ".386" << endl;
	output << ".model flat, stdcall" << endl;
	output << "option casemap:none" << endl;

	// Cabeceras
	output << "include \\masm32\\macros\\macros.asm" << endl;
	output << "include \\masm32\\include\\masm32.inc" << endl;
	output << "include \\masm32\\include\\kernel32.inc" << endl;

	// Librerias
	output << "includelib \\masm32\\lib\\masm32.lib" << endl;
	output << "includelib \\masm32\\lib\\kernel32.lib" << endl;

	// Variables globales
	output << ".data" << endl;
	symtab.generateCode(output);
	output << ".data?" << endl;

	// Codigo
	output << ".code" << endl;
	output << "INICIO:" << endl;

	generateCodeOnList(tree, output);

	output << "exit" << endl;
	output << "end INICIO" << endl;
}

void Node::displayList(string wrapper_tag, Node *node) {
	xml.openTag(wrapper_tag);
	while (node) {
		node->display();
		node = node->next;
	}
	xml.closeTag();
}

int Node::checkSemanticOnList(Node *node) {
	int type = TYPE_VOID;

	while (node) {
		node->checkSemantic();

		if (node->type == TYPE_ERROR) {
			type = TYPE_ERROR;
			break;
		}

		node = node->next;
	}

	return type;
}

void Node::generateCodeOnList(Node *node, fstream &output) {
	while (node) {
		node->generateCode(output);
		node = node->next;
	}
}

string Node::generateUniqueLabel(string label) {
	stringstream unique_label;

	if (labels.count(label)) {
		labels[label] ++;
	} else {
		labels[label] = 0;
	}
	
	unique_label << label << labels[label];
	
	return unique_label.str();
}

string Node::conditionalJump(string op) {
	if (op == ">") {
		return "jg";
	} else if (op == "<") {
		return "jl";
	} else if (op == ">=") {
		return "jge";
	} else if (op == "<=") {
		return "jle";
	} else if (op == "==") {
		return "je";
	} else if (op == "!=") {
		return "jne";
	} else {
		return "";
	}
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

void Identifier::checkSemantic() {
	this->type = symtab.getType(this->symbol);

	if (this->type == TYPE_ERROR) {
		error("Variable " + this->symbol + " no definida");
	}
}

void Identifier::generateCode(fstream &output) {
	output << "push " << this->symbol << endl;
}

/* Integer methods */
Integer::Integer(string symbol) {
	this->symbol = symbol;
}

void Integer::display() {
	xml.oneLineTag("ENTERO", this->symbol);
}

void Integer::checkSemantic() {
	this->type = TYPE_INTEGER;
}

void Integer::generateCode(fstream &output) {
	output << "push " << this->symbol << endl;
}

/* Float methods */
Float::Float(string symbol) {
	this->symbol = symbol;
}

void Float::display() {
	xml.oneLineTag("REAL", this->symbol);
}

void Float::checkSemantic() {
	this->type = TYPE_FLOAT;
}

void Float::generateCode(fstream &output) {
	output << "push " << this->symbol;
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

void BinaryExpr::checkSemantic() {
	this->left->checkSemantic();
	this->right->checkSemantic();

	if (this->left->type == TYPE_ERROR || this->right->type == TYPE_ERROR ||
		this->left->type != this->right->type) {

		this->type = TYPE_ERROR;

		error("Operacion binaria (" +  this->op + ") entre tipos no compatibles");
	} else {
		this->type = this->left->type;
	}
}
/* Equality expression methods */
EqualityExpr::EqualityExpr(string op, Expression *left, Expression* right)
		: BinaryExpr(op, left, right) {}

void EqualityExpr::generateCode(fstream &output) {
	string true_label, end_label;

	true_label  = generateUniqueLabel("VERDADERO");
	end_label   = generateUniqueLabel("FIN");

	output << "; EXPRESION IGUALDAD" << endl;
	
	this->left->generateCode(output);
	this->right->generateCode(output);

	output << "pop ebx" << endl;
	output << "pop eax" << endl;
	output << "cmp eax, ebx" << endl;

	output << conditionalJump(this->op) << " " << true_label << endl;
	output << "push 0" << endl;
	output << "jmp " << end_label << endl;
	output << true_label << ": " << endl;
	output << "push 1" << endl;
	output << end_label << ": " << endl;
}

/* Relational expression methods */
RelationalExpr::RelationalExpr(string op, Expression *left, Expression* right)
		: BinaryExpr(op, left, right) {}

/**
 * TODO: Reuse code from EqualityExpr
 */
void RelationalExpr::generateCode(fstream &output) {
	string true_label, end_label;

	true_label  = generateUniqueLabel("VERDADERO");
	end_label   = generateUniqueLabel("FIN");

	output << "; EXPRESION RELACIONAL" << endl;
	
	this->left->generateCode(output);
	this->right->generateCode(output);

	output << "pop ebx" << endl;
	output << "pop eax" << endl;
	output << "cmp eax, ebx" << endl;

	output << conditionalJump(this->op) << " " << true_label << endl;
	output << "push 0" << endl;
	output << "jmp " << end_label << endl;
	output << true_label << ": " << endl;
	output << "push 1" << endl;
	output << end_label << ": " << endl;
}


/* Additive expression methods */
AdditiveExpr::AdditiveExpr(string op, Expression *left, Expression* right) 
		: BinaryExpr(op, left, right, "SUMA") {}

void AdditiveExpr::generateCode(fstream &output) {
	output << "; EXPRESION ADDITIVA" << endl;
	
	this->left->generateCode(output);
	this->right->generateCode(output);

	output << "pop ebx" << endl;
	output << "pop eax" << endl;

	if (this->op == "+") {
		output << "add eax, ebx" << endl;
	} else if (this->op == "-") {
		output << "sub eax, ebx" << endl;
	}

	output << "push eax" << endl;
}

/* Multiplicative expression methods */
MultiplicativeExpr::MultiplicativeExpr(string op, Expression *left, Expression* right) 
		: BinaryExpr(op, left, right, "MULT") {}

void MultiplicativeExpr::generateCode(fstream &output) {
	output << "; EXPRESION MULTIPLICATIVA" << endl;
	
	this->left->generateCode(output);
	this->right->generateCode(output);

	output << "pop ebx" << endl;
	output << "pop eax" << endl;

	
	if (this->op == "*") {
		output << "imul ebx" << endl;
		output << "push eax" << endl;

	} else if (this->op == "/" || this->op == "%") {
		output << "xor edx, edx" << endl;
		output << "idiv ebx" << endl;

		if (this->op == "/") {
			output << "push eax" << endl;
		} else {
			output << "push edx" << endl;
		}
	}
}

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

void UnaryExpr::checkSemantic() {
	this->expr->checkSemantic();

	this->type = this->expr->type;
}

void UnaryExpr::generateCode(fstream &output) {
	output << "; EXPRESION UNARIA" << endl;

	this->expr->generateCode(output);

	if (this->op == "-") {
		output << "pop eax" << endl;
		output << "neg eax" << endl;
		output << "push eax" << endl;
	}
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

void IfStatement::checkSemantic() {
	this->expr->checkSemantic();
	
	if (this->expr->type == TYPE_ERROR ||
		checkSemanticOnList(this->statement) == TYPE_ERROR ||
		checkSemanticOnList(this->elseStatement) == TYPE_ERROR) {

		this->type = TYPE_ERROR;
	} else {
		this->type = TYPE_VOID;
	}
}

void IfStatement::generateCode(fstream &output) {
	string end_label, else_label;

	else_label = generateUniqueLabel("ELSE_IF");
	end_label = generateUniqueLabel("FIN_IF");
	
	output << "; SENTENCIA IF"  << endl;

	this->expr->generateCode(output);
	output << "pop eax" << endl;
	output << "cmp eax, 0" << endl;
	output << "je " << else_label << endl;
	generateCodeOnList(this->statement, output);
	output << "jmp " << end_label << endl;
	output << else_label << ": " << endl;
	generateCodeOnList(this->elseStatement, output);
	output << end_label << ": " << endl;
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

void WhileStatement::checkSemantic() {
	this->expr->checkSemantic();

	if (this->expr->type == TYPE_ERROR ||
		checkSemanticOnList(this->statement) == TYPE_ERROR) {

		this->type = TYPE_ERROR;
	} else {
		this->type = TYPE_VOID;
	}
}

void WhileStatement::generateCode(fstream &output) {
	string while_label, end_label;

	while_label = generateUniqueLabel("WHILE");
	end_label   = generateUniqueLabel("FIN_WHILE");

	output << "; SENTENCIA WHILE" << endl;

	output << while_label << ": " << endl;
	this->expr->generateCode(output);
	output << "pop eax" << endl;
	output << "cmp eax, 0" << endl;
	output << "je " << end_label << endl;
	generateCodeOnList(this->statement, output);
	output << "jmp " << while_label << endl;
	output << end_label << ": " << endl;
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

void PrintStatement::checkSemantic() {
	this->expr->checkSemantic();

	if (this->expr->type == TYPE_ERROR) {
		this->type = TYPE_ERROR;
	} else {
		this->type = TYPE_VOID;
	}
}

void PrintStatement::generateCode(fstream &output) {
	output << "; SENTENCIA PRINT" << endl;

	this->expr->generateCode(output);
	output << "pop eax" << endl;
	output << "print str$(eax), 10" << endl;
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

void Assignment::checkSemantic() {
	int id_type;

	this->expr->checkSemantic();
	id_type = symtab.getType(this->id->symbol);

	if (this->expr->type != TYPE_ERROR) {
		if (id_type == TYPE_ERROR) {
			symtab.insert(this->id->symbol, this->expr->type);

			this->type = TYPE_VOID;
		} else {
			if (this->expr->type != id_type) {
				this->type = TYPE_ERROR;

				error("Asignacion a variable de distinto tipo");
			} else {
				this->type = TYPE_VOID;
			}
		}
	} else {
		this->type = TYPE_ERROR;
	}
}

void Assignment::generateCode(fstream &output) {
	output << "; SENTENCIA ASIGNACION" << endl;

	this->expr->generateCode(output);

	output << "pop eax" << endl;
	output << "mov " << this->id->symbol << ", eax" << endl;
}