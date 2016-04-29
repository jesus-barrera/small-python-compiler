#ifndef _XMLGENERATOR_H_INCLUDED_
#define _XMLGENERATOR_H_INCLUDED_

#include <iomanip>
#include <stack>
#include <ostream>

#define SET_TABS(N) setw(N*4)<<""

using namespace std;

class XMLGenerator {
private:
	ostream *output;
	stack<string> tags;
	int tabs;
	bool single_line;

public:	
	void setoutput(ostream *output);
	void openTag(string tag_name, string value = "");
	void closeTag();
	void oneLineTag(string tag_name, string content);
	void put(string str);
};

#endif