#include <iostream>
#include <fstream>  

#include "Lexical.h"
#include "Syntax.h"

using namespace std;

int main(int argc, char *args[]) {
    string infilename;
    fstream output("salida.txt", ios::out);
    Node *tree;
    
    Lexical lex;
    Syntax parser;
    TokenStream *tokens;

    if (argc == 2) {
        infilename = args[1];
    } else {
        infilename = "entrada.txt";
    }

    try {
        tokens = lex.analyze(infilename);       

        tree = parser.analyze(tokens);
        
        if (Node::checkSemanticOnList(tree) == TYPE_ERROR) {
            throw 0;
        }
        
        Node::generateCode(tree, output);

        lex.destroyTokens(tokens);
        delete(tree);
    } catch (int e) {
        output << 0;
    }

    output.close();
}