#include "JsonReader.h"

using namespace jserialization::stdvisitor;

JsonReader::JsonReader(std::string filename) :parser(new Parser(filename)) {}

JsonReader::~JsonReader() {
	delete parser;
}