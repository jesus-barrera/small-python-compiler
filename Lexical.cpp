#include <exception>
#include <iostream>
#include <stack>

#include "Lexical.h"


// Transitions
int moves[NUM_STATES][23] = {
/*   a-z  0-9  ' '   \n  +,-  /%*   :    =   <,>   !    (    )    .   EOF    */
    { Q1,  Q2, Q15, Q13,  Q3,  Q4,  Q5,  Q8, Q11, Q10,  Q6,  Q7,   K, Q14,   K},
    { Q1,  Q1,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,  Q2,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K, Q16,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,  Q9,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,  Q9,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K, Q12,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K,   K, Q15,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K, Q17,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
    {  K, Q17,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K}
};

// Final states tokens
int actions[NUM_STATES] = {
    TKN_ERROR,
    TKN_IDENTIFIER,
    TKN_INTEGER,
    TKN_OP_ADD,
    TKN_OP_MULT,
    TKN_COLON,
    TKN_LEFT_PAR,
    TKN_RIGHT_PAR,
    TKN_OP_ASSIGN,
    TKN_OP_EQUALITY,
    TKN_ERROR,
    TKN_OP_RELATIONAL,
    TKN_OP_RELATIONAL,
    TKN_NEWLINE,
    TKN_EOF,
    TKN_WHITE_SPACE,
    TKN_ERROR,
    TKN_FLOAT
};

string reserved_words[] = {
    "if",
    "while",
    "print",
    "else"
};

const char *str_tokens[NUM_TOKENS] = {
    "TKN_NEWLINE",
    "TKN_INDENT",
    "TKN_DEDENT",
    "TKN_IDENTIFIER",
    "TKN_INTEGER",
    "TKN_FLOAT",
    "TKN_OP_ADD",
    "TKN_OP_MULT",
    "TKN_COLON",
    "TKN_LEFT_PAR",
    "TKN_RIGHT_PAR",
    "TKN_OP_ASSIGN",
    "TKN_OP_EQUALITY",
    "TKN_OP_RELATIONAL",
    "TKN_WHITE_SPACE",
    "TKN_EOF",
    "TKN_KEYWORD",
    "TKN_ERROR", 
};

Token::Token(int type, string symbol) {
    this->type = type;
    this->symbol = symbol;
}

Token::Token() {
    this->type = TKN_ERROR;
    this->symbol = "";
}

bool Token::is(int type) {
    return this->type == type;
}

bool Token::is(string symbol) {
    return this->symbol == symbol;
}

void Lexical::loadBuffer() {
    input.read(buffer, BUFFER_SIZE);

    if (input.eof()) {
        buffer[input.gcount()] = EOF;
    }

    buff_pos = 0;
}

char Lexical::getChar() {
    char c;

    c = peekChar();

    if (c != EOF && (++buff_pos == BUFFER_SIZE)) {
        loadBuffer();
    }

    return c;
}

char Lexical::peekChar() {
    return buffer[buff_pos];
}

int Lexical::getInput() {
    char c = peekChar();

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
        return 0;
    } else if (c >= '0' && c <= '9') {
        return 1;
    } else if (c == ' ' || c == '\t' || c == '\r') {
        return 2;
    } else if (c == '\n') {
        return 3;
    } else if (c == '+' || c == '-') {
        return 4;
    } else if (c == '/' || c == '%' || c == '*') {
        return 5;
    } else if (c == ':') {
        return 6;
    } else if (c == '=') {
        return 7;
    } else if (c == '<' || c == '>') {
        return 8;
    } else if (c == '!') {
        return 9;
    } else if (c == '(') {
        return 10;
    } else if (c == ')') {
        return 11;
    } else if (c == '.') {
        return 12;
    } else if (c == EOF) {
        return 13;
    } else {
        return 14;
    }
}

bool Lexical::isReservedWord(string &identifier) {
    for (int i = 0; i < 4; i++) {
        if (identifier == reserved_words[i]) {
            return true;
        }
    }

    return false;
}

TokenStream *Lexical::analyze() {
    char c;

    int state;
    int next_state;

    int indent;
    stack<int> indent_levels;

    int tkn_type;
    string tkn_symbol;

    TokenStream *tokens;

    tokens = new TokenStream();
    loadBuffer();
    indent_levels.push(0);

    do {
        // Check indentation at new line
        if (tokens->empty() || tokens->back()->type == TKN_NEWLINE) {
            c = peekChar();
            indent = 0;

            while (c == '\t' || c == ' ' ||  c == '\n') {            
                if (c == '\t') {
                    indent += TAB_SIZE - indent % TAB_SIZE;
                } else if (c == ' ') {
                    indent++;
                } else {
                    indent = 0;
                }

                getChar();
                c = peekChar();
            }

            if (c == EOF) indent = 0;

            if (indent > indent_levels.top()) {
                tokens->push_back(new Token(TKN_INDENT, ""));
                indent_levels.push(indent);
            } else {
                while (indent < indent_levels.top()) {
                    indent_levels.pop();
                    tokens->push_back(new Token(TKN_DEDENT, ""));
                }

                if (indent != indent_levels.top()) {
                    error("Indentacion inconsistente");
                }
            }
        }

        do {
            state = Q0;
            tkn_symbol = "";

            next_state = moves[state][getInput()];

            while (next_state != K) {
                state = next_state;
                tkn_symbol.append(1, getChar());
                next_state = moves[state][getInput()];
            }

            tkn_type = actions[state];
        } while (tkn_type == TKN_WHITE_SPACE);

        if (tkn_type == TKN_ERROR) {
            tkn_symbol.append(1, peekChar());
            error("Simbolo no reconocido \"" + tkn_symbol + "\"");
        } else {
            if (tkn_type == TKN_EOF) {            
                // Generate pending dedents
                while (indent_levels.top() > 0) {
                    indent_levels.pop();
                    tokens->push_back(new Token(TKN_DEDENT, ""));
                }
            } else if (tkn_type == TKN_IDENTIFIER && isReservedWord(tkn_symbol)) {
                tkn_type = TKN_KEYWORD;
            }

            tokens->push_back(new Token(tkn_type, tkn_symbol));
        }
    } while (tokens->back()->type != TKN_EOF);

    return tokens;
}

void Lexical::error(string msg) {
    cout << "[Error Lexico] " << msg << endl;
    throw 0;
}

TokenStream *Lexical::analyze(string filename) {
    TokenStream *tokens;
    input.open(filename.c_str());

    if (!input.is_open()) {
        error("No se pudo abrir el archivo: \""  + filename + "\"");
    }

    tokens = analyze();

    input.close();
    input.clear();

    return tokens;
}

void Lexical::destroyTokens(TokenStream *tokens) {
    if (tokens) {
        while (!tokens->empty()) {
            delete(tokens->front());

            tokens->pop_front();
        }

        delete(tokens);
    }
}

void Lexical::displayTokens(TokenStream *tokens) {
    TokenStream::iterator i;

    for (i = tokens->begin(); i != tokens->end(); ++i) {
        cout << "<" << str_tokens[(*i)->type] << " " << (*i)->symbol << ">" << endl;
    }
}