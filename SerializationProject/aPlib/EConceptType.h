#pragma once
#include <string>
#include <map>

namespace jserialization {
	enum class ConceptType {
		NUMBER,
		STRING,
		ARRAY,
		INITIALIZERLIST,
		CONTAINER
	};

	static const std::map<ConceptType, std::string> CONCEPTTYPE_STRING = {
		std::make_pair(ConceptType::NUMBER, "NUMBER"), std::make_pair(ConceptType::STRING, "STRING"), 
		std::make_pair(ConceptType::ARRAY, "ARRAY"), std::make_pair(ConceptType::INITIALIZERLIST, "INITIALIZERLIST"),
		std::make_pair(ConceptType::CONTAINER, "CONTAINER")

	};

	bool compareWithConceptEnum(std::string str, ConceptType concept_type);
}