#pragma once
#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdarg>
#include "TypeDeductionHelper.h"
#include "jsonobject.h"
#include "EBaseType.h"
#include "EConceptType.h"

namespace jserialization {

	bool ReadSingleInput(std::ifstream& fs, std::string& data);

	bool Search(std::ifstream& fs, std::list<std::string>& data);

	//atomic function
	template < typename TYPE>
	void input(TYPE& jo, int id) {
		std::ofstream fs(jo.ToStr(), ios::app);
		jo.Serialize(fs, id);
		fs.close();
	}


	//async
	int findOrInsert(const std::string objectSpecifier, const std::list<std::string> query, std::list<std::string> datalist);
}