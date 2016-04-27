#include <iostream>
#include <fstream>	

#include "Lexical.h"

using namespace std;

int main(int argc, char *args[]) {
	string infilename;
	ofstream output("salida.txt");
	
	Lexical lex;
	TokenStream *tokens;

	TokenStream::iterator i;

	if (argc == 2) {
		infilename = args[1];
	} else {
		infilename = "entrada.txt";
	}

	try {
		tokens = lex.analyze(infilename);
		lex.destroyTokens(tokens);

		output << 1;
	} catch (int e) {
		output << 0;
	}

	output.close();
}