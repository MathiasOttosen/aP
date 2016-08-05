#include <iostream>
#include <list>
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
		ReadSingleLetter();
		ParseSimpleJsonObject(name, value, concept_type, base_type);
	}
	else if (NoWSPeek(inf) == '[') { //process array
		ReadSingleLetter();
		concept_type = ctype::ARRAY;
		base_type = btype::CUSTOM;
	}
	return std::make_tuple(name, value, concept_type, base_type);
}

void Parser::ParseSimpleJsonObject(std::string& name, std::string& value, ctype& concept_type, btype& base_type) {
	std::string tmpstr;
	if (NoWSPeek(inf) == '"') {
		name = ReadWord();
		name = name.substr(1, name.size() - 2);
		std::cout << "Name: " << name << std::endl;
		if (name.compare("country") == 0) {
			++tempkfCounter;
			std::cout << tempkfCounter << std::endl;
		}
		if (tempkfCounter >= 7818) {
			concept_type = ctype::STRING;
		}
		if (NoWSPeek(inf) == ':') {
			tmpstr = ReadSingleLetter();
			if (NoWSPeek(inf) == '\"') { // string
				concept_type = ctype::STRING;
				value = ReadWord();
				base_type = btype::STRING;
			}
			else if (NoWSPeek(inf) >= '0' && NoWSPeek(inf) <= '9' || NoWSPeek(inf) == '-') { //number
				concept_type = ctype::NUMBER;
				value = ReadDigits();
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
				value = ReadFreeWord();
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
	if (NoWSPeek(inf) == '%')
		endofobject = true;
	return endofobject;
}

char Parser::NoWSPeek(std::ifstream& inf) {
	while (inf.peek() <= 10 || inf.peek() == 32) {
		if (inf.peek() == EOF)
			break;
		inf.get();
	}
	if (inf.peek() == EOF) {
		return '%';
	}
	return inf.peek();
}

std::string Parser::ReadWord() {
	std::ostringstream os;
	if (NoWSPeek(inf) == '\"') {
		os << ReadSingleLetter();
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

std::string Parser::ReadDigits() {
	std::ostringstream os;
	auto c = ReadSingleLetter();
	while ((NoWSPeek(inf) >= '0' && NoWSPeek(inf) <= '9') || NoWSPeek(inf) == '.' || NoWSPeek(inf) == '-') {
		os << c;
		c = ReadSingleLetter();
	}
	os << c;
	return os.str();
}

std::string Parser::ReadFreeWord() {
	std::ostringstream os;
	auto c = ReadSingleLetter();
	while (NoWSPeek(inf) >= 'a' && NoWSPeek(inf) <= 'z') {
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

