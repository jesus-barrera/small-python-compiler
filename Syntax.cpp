#include <cstdlib>
#include <exception>

#include "Syntax.h"

using namespace std;

void Syntax::analyze(TokenStream *tkn_stream) {
    this->tkn_stream = *tkn_stream; // make a copy
    nextToken();
    
    statementList();
    match(TKN_EOF);
}

void Syntax::statementList() {
    if (lookahead.is("if") || 
        lookahead.is("while") ||
        lookahead.is("print") ||
        lookahead.is(TKN_IDENTIFIER)) {

        statement();
        statementList();
    }
}

void Syntax::statement() {
    if (lookahead.is("if")) {
        ifStatement();
    } else if (lookahead.is("while")) {
        whileStatement();
    } else if (lookahead.is("print")) {
        printStatement();
    } else {
        assignmentStatement();
    }
}

void Syntax::ifStatement() {
    match("if");
    expr();
    match(":");
    compoundStatement();

    if (lookahead.is("else")) {
        match("else");
        match(":");
        compoundStatement();
    }
}

void Syntax::whileStatement() {
    match("while");
    expr();
    match(":");
    compoundStatement();
}

void Syntax::printStatement() {
    match("print");
    match("(");
    expr();
    match(")");
    match(TKN_NEWLINE);
}

void Syntax::assignmentStatement() {
    match(TKN_IDENTIFIER);
    match("=");
    expr();
    match(TKN_NEWLINE);
}

void Syntax::compoundStatement() {
    match(TKN_NEWLINE);
    match(TKN_INDENT);
    statement();
    statementList();
    match(TKN_DEDENT);
}

void Syntax::expr() {
    relationalExpr();

    while (lookahead.is(TKN_OP_EQUALITY)) {
        match(TKN_OP_EQUALITY);
        relationalExpr();
    }
}

void Syntax::relationalExpr() {
    additiveExpr();

    while (lookahead.is(TKN_OP_RELATIONAL)) {
        match(TKN_OP_RELATIONAL);
        additiveExpr();
    }
}

void Syntax::additiveExpr() {
    multiplicativeExpr();

    while (lookahead.is(TKN_OP_ADD)) {
        match(TKN_OP_ADD);
        multiplicativeExpr();
    }
}

void Syntax::multiplicativeExpr() {
    unaryExpr();

    while (lookahead.is(TKN_OP_MULT)) {
        match(TKN_OP_MULT);
        unaryExpr();
    }
}

void Syntax::unaryExpr() {
    if (lookahead.is(TKN_OP_ADD)) {
        match(TKN_OP_ADD);
        unaryExpr();
    } else {
        primaryExpr();
    }
}

void Syntax::primaryExpr() {
    if (lookahead.is(TKN_INTEGER)) {
        match(TKN_INTEGER);
    } else if (lookahead.is(TKN_FLOAT)) {
        match(TKN_FLOAT);
    } else if (lookahead.is(TKN_IDENTIFIER)) {
        match(TKN_IDENTIFIER); 
    } else {
        match("(");
        expr();
        match(")");
    }
}

void Syntax::nextToken() {
    if (!tkn_stream.empty()) {
        lookahead = *(tkn_stream.front());

        tkn_stream.pop_front();
    }
}

void Syntax::match(int tkn_type) {
    if (lookahead.type == tkn_type) {
        nextToken();
    } else {
        error(str_tokens[tkn_type]);
    }
}

void Syntax::match(string tkn_symbol) {
    if (lookahead.symbol == tkn_symbol) {
        nextToken();
    } else {
        error(tkn_symbol);
    }
}

void Syntax::error(string expected) {
    cout << "[Error de sintaxis] Se esperaba '" << expected << "'" << endl;
    cout << "Se encontro '" << lookahead.symbol << "' (" << str_tokens[lookahead.type] << ")" << endl;
    
    throw 0;    
}