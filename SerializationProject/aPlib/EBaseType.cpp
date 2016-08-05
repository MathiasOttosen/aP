#include "EBaseType.h"

bool compareWithBaseEnum(std::string str, jserialization::BaseType base_type) {
	if (jserialization::BASETYPE_STRING.at(base_type).compare(str) == 0) {
		return true;
	}
	return false;
}