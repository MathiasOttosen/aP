#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cstdlib>
#include "TypeDeductionHelper.h"
#include "BaseValue.h"
#include "EBaseType.h"
#include "EConceptType.h"

namespace jserialization{
	class Parser {
	public:
		using ctype = ConceptType;
		using btype = BaseType;

		Parser(const std::string& filename);

		~Parser();

		std::tuple<std::string, std::string, ctype, btype> ParseNextObject();

		void Parser::ParseSimpleJsonObject(std::string& name, std::string& value, ctype& concept_type, btype& base_type);
		
		bool EndOfObject();

		inline char NoWSPeek(std::ifstream& inf);

		inline std::string ReadWord();

		inline std::string ReadTilDelimiter(const char& delimiter);

		inline std::string ReadDigits();

		inline std::string ReadFreeWord();
		
		inline std::list<char> AttemptReadForward();

		inline char ReadSingleLetter();

	private:
		std::ifstream inf;

		int tempkfCounter = 0;
	};
	
}