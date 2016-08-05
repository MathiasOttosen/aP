#include "JsonParser.h"

using Parser = jserialization::Parser;
using ctype = jserialization::ConceptType;
using btype = jserialization::BaseType;

Parser::Parser(const std::string& filename) {
	try {
		inf.open(filename);
	}
	catch (std::ifstream::failure e) {
		std::cout << "Exception opening/reading file";
		throw e;
	}	
}

Parser::~Parser() {
	inf.close();
}

/*
* ParseNextObject() will identify the next object and
* retrieve it if it is simple (not a subobject or an array)
*/
std::tuple<std::string, std::string, ctype, btype> Parser::ParseNextObject() {
	
	ctype concept_type = ctype::CONTAINER;
	btype base_type = btype::UID;
	std::string tmpstr = "";
	std::string name = "";
	std::string value = "";
	if (NoWSPeek(inf) == '{'|| NoWSPeek(inf) == ',') { //process object or element in list
		inf >> tmpstr;
		ParseSimpleJsonObject(name, value, concept_type, base_type);
	}
	else if (NoWSPeek(inf) == '[') { //process array
		concept_type = ctype::ARRAY;
		base_type = btype::CUSTOM;
	}
	return std::make_tuple(name, value, concept_type, base_type);
}

void Parser::ParseSimpleJsonObject(std::string& name, std::string& value, ctype& concept_type, btype& base_type) {
	std::string tmpstr;
	if (NoWSPeek(inf) == '"') {
		inf >> name;
		name.substr(1, name.size() - 1);
		if (NoWSPeek(inf) == ':') {
			inf >> tmpstr;
			if (NoWSPeek(inf) == '\"') { // string
				concept_type = ctype::STRING;
				inf >> value;
				base_type = btype::STRING;
			}
			else if (NoWSPeek(inf) >= 0 && NoWSPeek(inf) <= 9) { //number
				concept_type = ctype::NUMBER;
				inf >> value;
				jserialization::AttemptNumberFitting(value, base_type);
			}
			else if (NoWSPeek(inf) == '[') { //array
				concept_type = ctype::ARRAY;
				base_type = btype::CUSTOM;
			}
			else if (NoWSPeek(inf) == '{') {  //object 
				concept_type = ctype::CONTAINER;
				base_type = btype::CUSTOM;
			}
			else if (NoWSPeek(inf) == 'f' || NoWSPeek(inf) == 't') {
				concept_type = ctype::NUMBER;
				base_type = btype::BOOL;
			}
		}
	}
}

bool Parser::EndOfObject() {
	bool endofobject = false;
	if (NoWSPeek(inf) == '}') {
		inf.ignore();
		endofobject = true;
		if (NoWSPeek(inf) == ']') {
			inf.ignore();
		}
	}
	return endofobject;
}

char Parser::NoWSPeek(std::ifstream& inf) {
	while (inf.peek() <= 10 || inf.peek() == 32) {
		inf.get();
	}
	return inf.peek();
}

std::string Parser::ReadWord() {
	std::ostringstream os;
	if (NoWSPeek(inf) == '\"') {
		os << inf.get();
	}
	os << ReadTilDelimiter('\"');
	return os.str();
}

std::string Parser::ReadTilDelimiter(const char delimiter) {
	std::ostringstream os;
	auto c = ReadSingleLetter();
	while (c != delimiter) {
		os << c;
		c = ReadSingleLetter();
	}
	os << c;
	return os.str();
}

char Parser::ReadSingleLetter() {
	char c;
	NoWSPeek(inf);
	inf.get(c);	
	return c;
}

