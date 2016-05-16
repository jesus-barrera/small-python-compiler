#include <cstdlib>
#include <exception>

#include "Syntax.h"

using namespace std;

Node *Syntax::analyze(TokenStream *tkn_stream) {
    Node *tree;

    this->tkn_stream = *tkn_stream; // make a copy
    
    nextToken();
    tree = statementList();
    match(TKN_EOF);

    return tree;
}

Node *Syntax::statementList() {
    Node *stmts = NULL;
    
    if (lookahead.is("if") || 
        lookahead.is("while") ||
        lookahead.is("print") ||
        lookahead.is(TKN_IDENTIFIER)) {

        stmts = statement();
        stmts->next = statementList();
    }

    return stmts;
}

Node *Syntax::statement() {
    if (lookahead.is("if")) {
        return ifStatement();
    } else if (lookahead.is("while")) {
        return whileStatement();
    } else if (lookahead.is("print")) {
        return printStatement();
    } else {
        return assignmentStatement();
    }
}

IfStatement *Syntax::ifStatement() {
    Expression *expr;
    Node *stmt;
    Node *elseStmt;
    
    match("if");
    expr = expression();
    match(":");
    
    stmt = compoundStatement();

    if (lookahead.is("else")) {
        match("else");
        match(":");

        elseStmt = compoundStatement();
    } else {
        elseStmt = NULL;
    }

    return new IfStatement(expr, stmt, elseStmt);
}

WhileStatement *Syntax::whileStatement() {
    Expression *expr;
    Node *stmt;

    match("while");
    expr = expression();
    match(":");
    stmt = compoundStatement();

    return new WhileStatement(expr, stmt);
}

PrintStatement *Syntax::printStatement() {
    Expression *expr;

    match("print");
    match("(");
    expr = expression();
    match(")");
    match(TKN_NEWLINE);

    return new PrintStatement(expr);
}

Assignment *Syntax::assignmentStatement() {
    Expression *expr;
    Identifier *id;

    id = new Identifier(lookahead.symbol);

    match(TKN_IDENTIFIER);
    match("=");
    expr = expression();
    match(TKN_NEWLINE);

    return new Assignment(id, expr);
}

Node *Syntax::compoundStatement() {
    Node *stmts;

    match(TKN_NEWLINE);
    match(TKN_INDENT);
    stmts = statement();
    stmts->next = statementList();
    match(TKN_DEDENT);

    return stmts;
}

Expression *Syntax::expression() {
    Expression *expr;
    string op;

    expr = relationalExpr();

    while (lookahead.is(TKN_OP_EQUALITY)) {
        op = lookahead.symbol;

        match(TKN_OP_EQUALITY);
        expr = new EqualityExpr(op, expr, relationalExpr());
    }

    return expr;
}

Expression *Syntax::relationalExpr() {
    Expression *expr;
    string op;
    
    expr = additiveExpr();

    while (lookahead.is(TKN_OP_RELATIONAL)) {
        op = lookahead.symbol;

        match(TKN_OP_RELATIONAL);
        expr = new RelationalExpr(op, expr, additiveExpr());
    }

    return expr;
}

Expression *Syntax::additiveExpr() {
    Expression *expr;
    string op;
    
    expr = multiplicativeExpr();

    while (lookahead.is(TKN_OP_ADD)) {
        op = lookahead.symbol;

        match(TKN_OP_ADD);
        expr = new AdditiveExpr(op, expr, multiplicativeExpr());
    }

    return expr;
}

Expression *Syntax::multiplicativeExpr() {
    Expression *expr;
    string op;
    
    expr = unaryExpr();

    while (lookahead.is(TKN_OP_MULT)) {
        op = lookahead.symbol;

        match(TKN_OP_MULT);
        expr = new MultiplicativeExpr(op, expr, unaryExpr());
    }

    return expr;
}

Expression *Syntax::unaryExpr() {
    Expression *expr;

    if (lookahead.is(TKN_OP_ADD)) {
        string op = lookahead.symbol;

        match(TKN_OP_ADD);
        expr = new UnaryExpr(op, unaryExpr());
    } else {
        expr = primaryExpr();
    }

    return expr;
}

Expression *Syntax::primaryExpr() {
    Expression *expr;
    string symbol;

    symbol = lookahead.symbol;
    
    if (lookahead.is(TKN_INTEGER)) {
        match(TKN_INTEGER);
        expr = new Integer(symbol);
    } else if (lookahead.is(TKN_FLOAT)) {
        match(TKN_FLOAT);
        expr = new Float(symbol);
    } else if (lookahead.is(TKN_IDENTIFIER)) {
        match(TKN_IDENTIFIER); 
        expr = new Identifier(symbol);
    } else {
        match("(");
        expr = expression();
        match(")");
    }

    return expr;
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