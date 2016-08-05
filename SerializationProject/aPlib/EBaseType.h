#pragma once

#include <string>
#include <map>
namespace jserialization {
	enum class BaseType : int{ 
		CHAR = 0,
		UCHAR = 1,
		INT = 2,
		UINT = 3,
		LONG =4,
		ULONG = 5,
		LONGLONG = 6,
		ULONGLONG = 7,
		FLOAT = 8,
		DOUBLE = 9, 
		LONGDOUBLE = 10,
		BOOL = 11,
		STRING = 12,
		CUSTOM = 13,
		UID = 14
	};

	static const std::map<BaseType, std::string> BASETYPE_STRING = { std::make_pair(BaseType::CHAR, "CHAR") , std::make_pair(BaseType::UCHAR, "UCHAR"),
		std::make_pair(BaseType::INT, "INT"), std::make_pair(BaseType::UINT, "UINT"), std::make_pair(BaseType::LONG, "LONG"), 
		std::make_pair(BaseType::ULONG, "ULONG"), std::make_pair(BaseType::LONGLONG, "LONGLONG"), std::make_pair(BaseType::ULONGLONG, "ULONGLONG"),
		std::make_pair(BaseType::FLOAT, "FLOAT"), std::make_pair(BaseType::DOUBLE, "DOUBLE"), std::make_pair(BaseType::LONGDOUBLE, "LONGDOUBLE"),
		std::make_pair(BaseType::BOOL, "BOOL"),std::make_pair(BaseType::STRING, "STRING"),std::make_pair(BaseType::CUSTOM, "CUSTOM"),std::make_pair(BaseType::UID, "UID")
	};

	bool compareWithBaseEnum(std::string str, BaseType base_type);
}