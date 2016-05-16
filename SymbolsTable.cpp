#include <iostream>
#include <iomanip>
#include "SymbolsTable.h"

const char *str_types[] = {
	"int",
	"float",
	"void",
	"error"
};

/**
 * Inserts a new symbol in the table
 */
bool SymbolsTable::insert(string &symbol, int type) {
	SymTabRecord record;

	if (table.count(symbol) == 1) {
		// symbol previously inserted
		return false;
	}

	record.symbol = symbol;
	record.type = type;

	table[symbol] = record;

	return true;
}

/**
 * Returns a symbol's type
 */
int SymbolsTable::getType(string &symbol) {
	if (table.count(symbol) == 1) {
		return table[symbol].type;
	} else {
		return TYPE_ERROR;
	}
}

/**
 * Shows table records
 */
void SymbolsTable::display() {
	map<string, SymTabRecord>::iterator i;

	cout << setw(10) << left << "Simbolo";
	cout << setw(10) << left << "Tipo";
	cout << endl;

	for (i = table.begin(); i != table.end(); i++) {
		cout << setw(10) << left << i->first;
		cout << setw(10) << left << str_types[i->second.type];
		cout << endl;
	}
}

/**
 * Deletes all table records
 */
void SymbolsTable::clear() {
	table.clear();
}

/**
 * Generates de MASM code for the variables declaration.
 */
void SymbolsTable::generateCode(ostream &output) {
	map<string, SymTabRecord>::iterator it;

	for (it = table.begin(); it != table.end(); ++it) {
		if (it->second.type == TYPE_INTEGER) {
			output << it->first << " dword 0" << endl;
		}
	}
}