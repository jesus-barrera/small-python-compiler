#include "XMLGenerator.h"

void replace(string& str, string old_str, string new_str) {
	size_t pos;

	if ((pos = str.find(old_str)) != string::npos) {
		str.replace(pos, old_str.size(), new_str);
	}
}

void XMLGenerator::setoutput(ostream *output) {
	this->output = output;
}

void XMLGenerator::openTag(string tag_name, string value) {
	string tag = "<" +  tag_name;

	if (value != "") {
		replace(value, "<", "&lt;");
		replace(value, ">", "&gt;");
		
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
	if (enable_tabs) {
		*output << SET_TABS(tabs) << str << endl;
	} else {
		*output << str << endl;
	}
}