#ifndef _SYMBOLS_TABLE_H_INCLUDED_
#define _SYMBOLS_TABLE_H_INCLUDED_

#include <map>
#include <string>
#include <fstream>

using namespace std;

// Data types
enum {
	TYPE_INTEGER,
	TYPE_FLOAT,
	TYPE_VOID,
	TYPE_ERROR
};

struct SymTabRecord {
	string symbol;
	int type;
};

class SymbolsTable {
private:
	map<string, SymTabRecord> table;

public:
	bool insert(string &symbol, int type);
	int getType(string &symbol);
	
	void display();
	void clear();
	
	void generateCode(ostream &output);
};

#endif // _SYMBOLS_TABLE_H_INCLUDED_