#include "XMLGenerator.h"

void XMLGenerator::setoutput(ostream *output) {
	this->output = output;
}

void XMLGenerator::openTag(string tag_name, string value) {
	string tag = "<" +  tag_name;

	if (value != "") {
		tag += " value=\"" + value +  "\""; 
	}

	tag += ">";

	put(tag);

	tags.push(tag_name);
	tabs++;
}


void XMLGenerator::closeTag() {
	tabs--;

	put("</" + tags.top() + ">");

	tags.pop();
}

void XMLGenerator::oneLineTag(string tag_name, string content) {
	put("<" + tag_name + ">" + content + "</" + tag_name + ">");
}

void XMLGenerator::put(string str) {
	*output << SET_TABS(tabs) << str << endl;
}