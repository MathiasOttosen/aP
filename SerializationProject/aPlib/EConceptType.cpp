#include "EConceptType.h"
bool jserialization::compareWithConceptEnum(std::string str, ConceptType concept_type) {
	if (CONCEPTTYPE_STRING.at(concept_type).compare(str) == 0) {
		return true;
	}
	return false;
}