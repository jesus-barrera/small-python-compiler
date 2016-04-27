#ifndef _LEXICAL_H_INCLUDED_
#define _LEXICAL_H_INCLUDED_

#include <fstream>
#include <list>
#include <stack>

#define BUFFER_SIZE 4096
#define TAB_SIZE 4

using namespace std;

/* Tokens */
enum {
	TKN_NEWLINE,
	TKN_INDENT,
	TKN_DEDENT,
	TKN_IDENTIFIER, 
	TKN_INTEGER, 
	TKN_FLOAT, 
	TKN_OP_ADD, 
	TKN_OP_MULT, 
	TKN_COLON,
	TKN_LEFT_PAR,
	TKN_RIGHT_PAR,
	TKN_OP_ASSIGN,
	TKN_OP_EQUALITY,
	TKN_OP_RELATIONAL,
	TKN_WHITE_SPACE,
	TKN_EOF,
	TKN_KEYWORD,
	TKN_ERROR, 
	NUM_TOKENS
};

/* States */
enum {
	Q0,
	Q1,
	Q2,
	Q3,
	Q4,
	Q5,
	Q6,
	Q7,
	Q8,
	Q9,
	Q10,
	Q11,
	Q12,
	Q13,
	Q14,
	Q15,
	Q16,
	Q17,
	NUM_STATES,
	K
};

class Token {
public:
	int type;
	string symbol;

	Token();
	Token(int type, string symbol);

	bool is(int type);
	bool is(string symbol);
};

typedef list<Token *> TokenStream;

extern const char* str_tokens[];

class Lexical {
private:
	char buffer[BUFFER_SIZE];
	int buff_pos;
	ifstream input;

	// Extracts the next BUFF_SIZE characters from the input file into the buffer
	void loadBuffer();

	// Extracts a character from the buffer
	char getChar();

	// Peeks the next character to be read from the buffer
	char peekChar();

	// Returns the input associated with the current pointed character in the buffer
	int getInput();

	// Returns true if the symbol is a reserved word
	bool isReservedWord(string &symbol);

	TokenStream *analyze();
	
	// Displays a readable lexical error, throws an exception
	void error(string tkn_symbol);

public:
	// Generates a token stream from the opened file
	TokenStream *analyze(string filename);
	
	// Deletes the token stream
	void destroyTokens(TokenStream *tokens);
};

#endif